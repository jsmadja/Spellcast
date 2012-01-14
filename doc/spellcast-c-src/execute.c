#include <stdio.h>
#include <strings.h>

#include "handwave.h"
#include "internal.h"

char exbuf[2048], exbuf2[2048], exbuf3[2048];
static char smallbuf[512];
static int summonelq;

static void exec_counters(), exec_summons(), check_elements(), exec_cancels(), exec_protects();
extern void exec_enchants(), exec_attacks(), exec_heals();

void clear_round(fred)
union being *fred;
{
    int ix;

    fred->both.enchant_caster = (-1);
    fred->both.enchant_ppend = 0;
    fred->both.raisedead_caster = (-1);
    fred->both.fl_resist_heat = 0;
    fred->both.fl_resist_cold = 0;
    fred->both.fl_resist_icestorm = 0;

    for (ix=0; ix<NUMSPELLS; ix++)
	fred->both.zaplist[ix] = 0;
}

/* add a zaplist entry. If the spell is a control spell, set the enchant_caster also; this can only store one value, but that's ok, since if there's more than one control spell they all fail. If the spell is raise dead, set raisedead_caster (only used when raising dead monsters) */
static void set_zap_flag(self, spel, spelnum)
struct realgame *self;
struct castspell *spel;
int spelnum;
{
    int set_enchanter=0, set_ppend=0;
    struct permstats *perm = NULL;
    int *ppend = NULL;

    if (spelnum==(-1))
	spelnum = spel->spellnum;

    if (spelnum==SP__PARALYSIS || spelnum==SP__CHARM_PERSON || spelnum==SP__CHARM_MONSTER) {
	set_enchanter=1;
    }

    if (spelnum==SP__PARALYSIS || spelnum==SP__CHARM_PERSON || spelnum==SP__CHARM_MONSTER || spelnum==SP__FEAR || spelnum==SP__AMNESIA || spelnum==SP__CONFUSION) {
	set_ppend=1;
    }

    switch (spel->targettype) {
	case QuVal_Target_Wizard:
	    perm = &(self->wiz[spel->target]->perm);
	    ppend = &(self->wiz[spel->target]->enchant_ppend);
	    self->wiz[spel->target]->zaplist[spelnum]++;
	    if (set_enchanter) 
		self->wiz[spel->target]->enchant_caster = spel->caster;
	    if (set_ppend)
		*ppend = spel->permanent;
	    if (spelnum==SP__RAISE_DEAD)
		self->wiz[spel->target]->raisedead_caster = spel->caster;
	    break;
	case QuVal_Target_Creature:
	    perm = &(self->cre[spel->target].perm);
	    ppend = &(self->cre[spel->target].enchant_ppend);
	    self->cre[spel->target].zaplist[spelnum]++;
	    if (set_enchanter) 
		self->cre[spel->target].enchant_caster = spel->caster;
	    if (set_ppend)
		*ppend = spel->permanent;
	    if (spelnum==SP__RAISE_DEAD)
		self->cre[spel->target].raisedead_caster = spel->caster;
	    break;
	default: /* up-in-air or area-effect */
	    break;
    }

    if (spel->permanent && perm) {
	struct wizard *wiz = self->wiz[spel->target];
	switch (spelnum) {
	    case SP__AMNESIA:
	    case SP__FEAR:
	    case SP__CHARM_PERSON:
	    case SP__CHARM_MONSTER:
	    case SP__PARALYSIS:
	    case SP__CONFUSION:
		break;
	    case SP__HASTE:
		perm->fl_haste = 1;
		break;
	    case SP__PROTECTION_FROM_EVIL:
		perm->fl_prot_evil = 1;
		break;
	    case SP__RESIST_HEAT:
		perm->fl_resist_heat = 1;
		break;
	    case SP__RESIST_COLD:
		perm->fl_resist_cold = 1;
		break;
	    case SP__INVISIBILITY:
		perm->fl_invisibility = 1;
		break;
	    case SP__BLINDNESS:
		perm->fl_blindness = 1;
		break;
	    default:
		printf("ERROR: do not know how to permanent %d.\n", spelnum);
		break;
	}
    }
}

char *pro_himself(gender)
int gender;
{
    switch (gender) {
	case Gender_NONE:
	    return "itself";
	case Gender_MALE:
	    return "himself";
	case Gender_FEMALE:
	    return "herself";
	case Gender_NEUTER:
	    return "hirself";
	default:
	    return "ERROR-himself";
    }
}

char *pro_him(gender)
int gender;
{
    switch (gender) {
	case Gender_NONE:
	    return "it";
	case Gender_MALE:
	    return "him";
	case Gender_FEMALE:
	    return "her";
	case Gender_NEUTER:
	    return "hir";
	default:
	    return "ERROR-him";
    }
}

char *pro_he(gender)
int gender;
{
    switch (gender) {
	case Gender_NONE:
	    return "it";
	case Gender_MALE:
	    return "he";
	case Gender_FEMALE:
	    return "she";
	case Gender_NEUTER:
	    return "ke";
	default:
	    return "ERROR-he";
    }
}

char *pro_his(gender) /* his head, not the head of his */
int gender;
{
    switch (gender) {
	case Gender_NONE:
	    return "its";
	case Gender_MALE:
	    return "his";
	case Gender_FEMALE:
	    return "her";
	case Gender_NEUTER:
	    return "hir";
	default:
	    return "ERROR-his";
    }
}

char *number_name(num)
int num;
{
    static char buf[16];

    switch (num) {
	case 0:
	    return "zero";
	case 1:
	    return "one";
	case 2:
	    return "two";
	case 3:
	    return "three";
	case 4:
	    return "four";
	case 5:
	    return "five";
	case 6:
	    return "six";
	case 7:
	    return "seven";
	case 8:
	    return "eight";
	case 9:
	    return "nine";
	case 10:
	    return "ten";
	default:
	    sprintf(buf, "%d", num);
	    return buf;
    }
}

static void list_mind_spells(buf, zapl)
char *buf;
int *zapl;
{
    int any = 0;
    int ix, spel;
    static int spelist[6] = {SP__AMNESIA, SP__CONFUSION, SP__CHARM_PERSON, SP__CHARM_MONSTER, SP__PARALYSIS, SP__FEAR};
    strcpy(buf, "");

    for (ix=0; ix<6; ix++) {
	spel = spelist[ix];
	if (zapl[spel]) {
	    if (any)
		strcat(buf, ", ");
	    any = 1;
	    if (zapl[spel]==1) {
		strcat(buf, spelllist[spel].name);
	    }
	    else {
		sprintf(smallbuf, "%s %s", number_name(zapl[spel]), spelllist[spel].name);
		strcat(buf, smallbuf);
	    }
	}
    }
}

static int check_visibility(self, mcaster, mtarget, ctmp)
struct realgame *self;
struct wizard *mcaster;
union being *mtarget;
struct castspell *ctmp;
{
    if (!mtarget) 
	return 1; /* you can always see up-in-the-air */

    if ((union being *)mcaster != mtarget
	&& (mtarget->both.invisibility || mcaster->blindness)) {
	sprintf(exbuf, "You are unable to see %s; your %s goes wild.\n", mtarget->both.name, spelllist[ctmp->spellnum].name);
	sprintf(exbuf2, "%s is unable to see you; %s %s goes wild.\n", mcaster->name, pro_his(mcaster->gender), spelllist[ctmp->spellnum].name);
	sprintf(exbuf3, "%s is unable to see %s; %s %s goes wild.\n", mcaster->name, mtarget->both.name, pro_his(mcaster->gender), spelllist[ctmp->spellnum].name);
	if (ctmp->targettype==QuVal_Target_Wizard)
	    PrintMsg3(ctmp->caster, ctmp->target, exbuf, exbuf2, exbuf3);
	else
	    PrintMsg2(ctmp->caster, exbuf, exbuf3);
	return 0;
    }
    return 1;
}

void execute_spells(self)
struct realgame *self;
{
    struct castspell *ctmp, **pt;
    int ix, jx, wiznum;
    int *zapl;
    union being *mtarget;
    struct wizard *mcaster;

    for (ctmp = self->castlist; ctmp; ctmp = ctmp->next) {
	char *handage;
	switch (ctmp->handage) {
	    case MASK_LEFT:
		handage = "with the left hand";
		break;
	    case MASK_RIGHT:
		handage = "with the right hand";
		break;
	    case MASK_TWOHAND:
		handage = "with both hands";
		break;
	    case 0:
		handage = "from a Delayed Effect";
		break;
	    case MASK_LEFT|MASK_RIGHT:
		handage = "ERROR:BOTH??? hands";
		break;
	    default:
		handage = "ERROR:NEITHER??? hand";
		break;
	}
	switch (ctmp->targettype) {
	    case -1:
		strcpy(smallbuf, "over the arena");
		break;
	    case 0:
		strcpy(smallbuf, "up into the air");
		break;
	    default:
		if (ctmp->targettype==QuVal_Target_Wizard && ctmp->target==ctmp->caster) {
		    strcpy(smallbuf, "at "); 
		    strcat(smallbuf, pro_himself(self->wiz[ctmp->caster]->gender)); 
		}
		else {
		    switch (ctmp->targettype) {
			case QuVal_Target_Wizard:
			    mtarget = (union being *)self->wiz[ctmp->target];
			    break;
			case QuVal_Target_Creature:
			    mtarget = (union being *)&(self->cre[ctmp->target]);
			    break;
			default:
			    mtarget = NULL;
			    break;
		    }
		    sprintf(smallbuf, "at %s", mtarget->both.name);
		}
		break;
	}
	if (ctmp->spellnum == SP__STAB)
	    sprintf(exbuf, "%s stabs (%s) %s.\n", self->wiz[ctmp->caster]->name, handage, smallbuf);
	else
	    sprintf(exbuf, "%s casts %s%s (%s) %s.\n", self->wiz[ctmp->caster]->name, ((ctmp->permanent)?"Permanent ":""), spelllist[ctmp->spellnum].name, handage, smallbuf);
	PrintMsg(exbuf);
    }

    /* CLEAR ROUND DATA */
    self->fl_icestorm = 0;
    self->fl_firestorm = 0;
    self->fl_dispelmagic = 0;
    for (ix=0; ix<self->numplayers; ix++) 
	clear_round(self->wiz[ix]);
    for (ix=0; ix<self->numcres; ix++) 
	clear_round(&self->cre[ix]);

    /* check DISPEL MAGIC, MAGIC MIRROR, COUNTER_SPELL */
    for (ctmp = self->castlist; ctmp; ctmp = ctmp->next) {

	switch (ctmp->targettype) {
	    case QuVal_Target_Wizard:
		mtarget = (union being *)self->wiz[ctmp->target];
		break;
	    case QuVal_Target_Creature:
		mtarget = (union being *)&(self->cre[ctmp->target]);
		break;
	    default:
		mtarget = NULL;
		break;
	}
	mcaster = self->wiz[ctmp->caster];

	/* convert counter_spell2 to counter_spell */
	if (ctmp->spellnum==SP__COUNTER_SPELL2)
	    ctmp->spellnum = SP__COUNTER_SPELL;

	if (ctmp->spellnum==SP__DISPEL_MAGIC) {
	    if (check_visibility(self, mcaster, mtarget, ctmp)) {
		self->fl_dispelmagic = 1;
		set_zap_flag(self, ctmp, SP__SHIELD); /* auto-shield */
	    }
	}
	else if (ctmp->spellnum==SP__COUNTER_SPELL) {
	    if (check_visibility(self, mcaster, mtarget, ctmp)) {
		set_zap_flag(self, ctmp, SP__COUNTER_SPELL);
	    }
	}
	else if (ctmp->spellnum==SP__MAGIC_MIRROR) {
	    if (check_visibility(self, mcaster, mtarget, ctmp)) {
		set_zap_flag(self, ctmp, SP__MAGIC_MIRROR);
	    }
	}
    }

    if (self->fl_dispelmagic)
	PrintMsg("The magical energies in the arena fade away.\n");

    /* EXECUTE DISPEL MAGIC, MAGIC MIRROR, COUNTER_SPELL : all beings and corpses. This sets the zapl[] for those spells. */
    for (ix=0; ix<self->numplayers; ix++) {
	exec_counters(self, self->wiz[ix], ix, 1);
    }
    for (ix=0; ix<self->numcres; ix++) {
	exec_counters(self, &self->cre[ix], ix, 0);
    }

    /* set up spell tables */
    for (ctmp = self->castlist; ctmp; ctmp = ctmp->next) {
	
	switch (ctmp->targettype) {
	    case QuVal_Target_Wizard:
		mtarget = (union being *)self->wiz[ctmp->target];
		break;
	    case QuVal_Target_Creature:
		mtarget = (union being *)&(self->cre[ctmp->target]);
		break;
	    default:
		mtarget = NULL;
		break;
	}
	mcaster = self->wiz[ctmp->caster];

	switch (ctmp->spellnum) {
	    case SP__SHIELD:
	    case SP__REMOVE_ENCHANTMENT:
	    case SP__RAISE_DEAD:
	    case SP__CURE_LIGHT_WOUNDS:
	    case SP__CURE_HEAVY_WOUNDS:
	    case SP__SUMMON_GOBLIN:
	    case SP__SUMMON_OGRE:
	    case SP__SUMMON_TROLL:
	    case SP__SUMMON_GIANT:
	    case SP__SUMMON_ELEMENTAL:
	    case SP__PROTECTION_FROM_EVIL:
	    case SP__RESIST_HEAT:
	    case SP__RESIST_COLD:
	    case SP__INVISIBILITY:
	    case SP__HASTE:
	    case SP__TIME_STOP:
	    case SP__DELAYED_EFFECT:
	    case SP__PERMANENCY:
		/* spells not affected by magic mirror */

		/* check for dispel magic */
		if (self->fl_dispelmagic) {
		    sprintf(exbuf, "The %s is dispelled.\n", spelllist[ctmp->spellnum].name);
		    PrintMsg(exbuf);
		    break;
		}

		if (!mtarget) {
		    /* up in air, or (shouldn't happen) area-effect */
		    break;
		}

		/* sight check */
		if (!check_visibility(self, mcaster, mtarget, ctmp)) {
		    break;
		}

		/* check for counterspell */
		if (mtarget->both.zaplist[SP__COUNTER_SPELL]) {
		    sprintf(exbuf, "The %s is destroyed by your Counter-Spell.\n", spelllist[ctmp->spellnum].name);
		    sprintf(exbuf2, "The %s is destroyed by the Counter-Spell around %s.\n", spelllist[ctmp->spellnum].name, mtarget->both.name);
		    if (ctmp->targettype==QuVal_Target_Wizard)
			PrintMsg2(ctmp->target, exbuf, exbuf2);
		    else
			PrintMsg(exbuf2);
		    break;
		}
		set_zap_flag(self, ctmp, -1);
		break;

	    case SP__STAB:
		/* not affected by magic mirror or dispel magic */
		if (!mtarget) {
		    /* up in air, or (shouldn't happen) area-effect */
		    break;
		}
		/* sight check */
		if (!check_visibility(self, mcaster, mtarget, ctmp)) {
		    break;
		}

		set_zap_flag(self, ctmp, -1);
		break;

	    case SP__MISSILE:
	    case SP__FINGER_OF_DEATH:
	    case SP__LIGHTNING_BOLT:
	    case SP__LIGHTNING_BOLT2:
	    case SP__CAUSE_LIGHT_WOUNDS:
	    case SP__CAUSE_HEAVY_WOUNDS:
	    case SP__FIREBALL:
	    case SP__AMNESIA:
	    case SP__CONFUSION:
	    case SP__CHARM_PERSON:
	    case SP__CHARM_MONSTER:
	    case SP__PARALYSIS:
	    case SP__FEAR:
	    case SP__ANTI_SPELL:
	    case SP__DISEASE:
	    case SP__POISON:
	    case SP__BLINDNESS:
		/* spells affected by magic mirror */

		/* check for repeat of short lightning bolt */
		if (ctmp->spellnum==SP__LIGHTNING_BOLT2) {
		    if (!mcaster->fl_cast_lightning) {
			mcaster->fl_cast_lightning = 1;
			ctmp->spellnum = SP__LIGHTNING_BOLT;
		    }
		    else {
			sprintf(exbuf2, "%s's Lightning Bolt fizzles.\n", mcaster->name);
			PrintMsg2(ctmp->caster, "Since you have already cast Lightning Bolt with that formulation, the spell fizzles.\n", exbuf2);
			break;
		    }
		}

		/* check for dispel magic */
		if (self->fl_dispelmagic) {
		    sprintf(exbuf, "The %s is dispelled.\n", spelllist[ctmp->spellnum].name);
		    PrintMsg(exbuf);
		    break;
		}

		if (!mtarget) {
		    /* up in air, or (shouldn't happen) area-effect */
		    break;
		}

		/* sight check */
		if (!check_visibility(self, mcaster, mtarget, ctmp)) {
		    break;
		}

		/* check for counterspell */
		if (ctmp->spellnum != SP__FINGER_OF_DEATH) {
		    if (mtarget->both.zaplist[SP__COUNTER_SPELL]) {
			sprintf(exbuf, "The %s is destroyed by your Counter-Spell.\n", spelllist[ctmp->spellnum].name);
			sprintf(exbuf2, "The %s is destroyed by the Counter-Spell around %s.\n", spelllist[ctmp->spellnum].name, mtarget->both.name);
			if (ctmp->targettype==QuVal_Target_Wizard)
			    PrintMsg2(ctmp->target, exbuf, exbuf2);
			else
			    PrintMsg(exbuf2);
			break;
		    }
		}

		ix = mtarget->both.zaplist[SP__MAGIC_MIRROR];
		/* if the caster is the target, ignore magic mirror. */
		if (ix && ((union being *)mcaster!=mtarget)) {
		    /* magic mirror -- terrific */
		    /* we assume that the caster is a wizard. */
		    
		    /* check to see if caster has mirror too */
		    if (mcaster->zaplist[SP__MAGIC_MIRROR]) {
			sprintf(exbuf, "Your %s is reflected back and forth between %s's Magic Mirror and your own! It rapidly decays and dissipates.\n", spelllist[ctmp->spellnum].name, mtarget->both.name);
			sprintf(exbuf2, "%s's %s is reflected back and forth between your Magic Mirror and %s own! It rapidly decays and dissipates.\n", mcaster->name, spelllist[ctmp->spellnum].name, pro_his(mcaster->gender)); 
			sprintf(exbuf3, "%s's %s is reflected back and forth between %s's Magic Mirror and %s own! It rapidly decays and dissipates.\n", mcaster->name, spelllist[ctmp->spellnum].name, mtarget->both.name, pro_his(mcaster->gender));
			if (ctmp->targettype==QuVal_Target_Wizard)
			    PrintMsg3(ctmp->caster, ctmp->target, exbuf, exbuf2, exbuf3);
			else
			    PrintMsg2(ctmp->caster, exbuf, exbuf3);
			break; /* spell is lost */
		    }

		    sprintf(exbuf, "Your %s is reflected from %s's Magic Mirror back at you.\n", spelllist[ctmp->spellnum].name, mtarget->both.name);
		    sprintf(exbuf2, "%s's %s reflects from your Magic Mirror back at %s.\n", mcaster->name, spelllist[ctmp->spellnum].name, pro_him(mcaster->gender)); 
		    sprintf(exbuf3, "%s's %s reflects from %s's Magic Mirror back at %s.\n", mcaster->name, spelllist[ctmp->spellnum].name, mtarget->both.name, pro_him(mcaster->gender));
		    if (ctmp->targettype==QuVal_Target_Wizard)
			PrintMsg3(ctmp->caster, ctmp->target, exbuf, exbuf2, exbuf3);
		    else
			PrintMsg2(ctmp->caster, exbuf, exbuf3);
		    /* reverse spell */
		    ix = ctmp->caster;
		    if (ctmp->targettype==QuVal_Target_Wizard)
			ctmp->caster = ctmp->target;
		    else
			ctmp->caster = (-1);
		    ctmp->targettype = QuVal_Target_Wizard;
		    ctmp->target = ix;
		    /* now, check AGAIN for counterspell */
		    if (ctmp->spellnum != SP__FINGER_OF_DEATH) {
			if (mcaster->zaplist[SP__COUNTER_SPELL]) {
			    sprintf(exbuf, "The reflected %s is destroyed by your Counter-Spell.\n", spelllist[ctmp->spellnum].name);
			    sprintf(exbuf2, "The reflected %s is destroyed by the Counter-Spell around %s.\n", spelllist[ctmp->spellnum].name, mcaster->name);
			    if (ctmp->targettype==QuVal_Target_Wizard)
				PrintMsg2(ctmp->target, exbuf, exbuf2);
			    else
				PrintMsg(exbuf2);
			    break;
			}
		    }
		    set_zap_flag(self, ctmp, -1);
		}
		else {
		    set_zap_flag(self, ctmp, -1);
		}
		break;

	    case SP__ICE_STORM:
		/* area-effect spell; mtarget is NULL */
		/* check for dispel magic */
		 if (self->fl_dispelmagic) {
		     sprintf(exbuf, "The %s is dispelled.\n", spelllist[ctmp->spellnum].name);
		     PrintMsg(exbuf);
		     break;
		 }
		self->fl_icestorm++;
		break;
	    case SP__FIRE_STORM:
		/* area-effect spell; mtarget is NULL */
		/* check for dispel magic */
		if (self->fl_dispelmagic) {
		    sprintf(exbuf, "The %s is dispelled.\n", spelllist[ctmp->spellnum].name);
		    PrintMsg(exbuf);
		    break;
		}
		self->fl_firestorm++;
		break;

	    case SP__SURRENDER:
		PrintMsg("ERROR: Surrender got through to spell table setup\n");
		break;

	    case SP__COUNTER_SPELL:
	    case SP__COUNTER_SPELL2:
	    case SP__DISPEL_MAGIC:
	    case SP__MAGIC_MIRROR:
		/* already taken care of */
		break;

	    default:
		PrintMsg("ERROR: Unknown spell in spell table setup\n");
		break;
	}
    }

    /* before summoning, do any queries for elemental type. */
    erase_queries(self);
    for (ix=0; ix<self->numplayers; ix++)
	if (self->wiz[ix]->alive) {
	    for (jx=0; jx<self->wiz[ix]->zaplist[SP__SUMMON_ELEMENTAL]; jx++) {
		add_query(self, ix, Qu_ElementalType, 0);
	    }
	}
    if (self->numqueries)
	Queries(self->numqueries, self->querylist);

    /* SUMMON SPELLS : live wizards */
    summonelq = 0; /* counter for queries */
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive)
	    exec_summons(self, self->wiz[ix], ix, 1);
    }

    /* CHECK FOR ELEMENTAL / STORM CANCELS */
    check_elements(self);

    /* CHECK FOR CANCELS : live beings */
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive)
	    exec_cancels(self, self->wiz[ix], ix, 1);
    }
    for (ix=0; ix<self->numcres; ix++) {
	if (self->cre[ix].alive)
	    exec_cancels(self, &self->cre[ix], ix, 0);
    }
   
    /* CHECK FOR PROTECTS : live beings */
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive)
	    exec_protects(self, self->wiz[ix], ix, 1);
    }
    for (ix=0; ix<self->numcres; ix++) {
	if (self->cre[ix].alive)
	    exec_protects(self, &self->cre[ix], ix, 0);
    }
   
    /* CHECK FOR ENCHANTMENTS : live beings */
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive)
	    exec_enchants(self, self->wiz[ix], ix, 1);
    }
    for (ix=0; ix<self->numcres; ix++) {
	if (self->cre[ix].alive)
	    exec_enchants(self, &self->cre[ix], ix, 0);
    }

    /* DAMAGE SPELLS : live beings */
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive)
	    exec_attacks(self, self->wiz[ix], ix, 1);
    }
    for (ix=0; ix<self->numcres; ix++) {
	if (self->cre[ix].alive)
	    exec_attacks(self, &self->cre[ix], ix, 0);
    }

    /* HEALING SPELLS : all beings */
    for (ix=0; ix<self->numplayers; ix++) {
	exec_heals(self, self->wiz[ix], ix, 1);
    }
    for (ix=0; ix<self->numcres; ix++) {
	exec_heals(self, &self->cre[ix], ix, 0);
    }

}

static void exec_counters(self, fred, cnum, wizflag) /* dead allowed */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    struct castspell *ctmp, **pt;
    int ix, jx, wiznum;
    int *zapl;

    zapl = fred->both.zaplist;

    /*{
	for (jx=0; jx<NUMSPELLS; jx++)
	    putchar('0'+zapl[jx]);
	printf(" : %s\n", fred->both.name);
    }*/

    if (self->fl_dispelmagic) {
	/* dispel magic in effect: neutralize counter_spells and magic mirrors */
	if (zapl[SP__COUNTER_SPELL]) {
	    zapl[SP__COUNTER_SPELL] = 0;
	    sprintf(exbuf, "The Counter-Spell on %s is dispelled.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Counter-Spell on you is dispelled.\n", exbuf);
	    else
		PrintMsg(exbuf);
	}
	if (zapl[SP__MAGIC_MIRROR]) {
	    zapl[SP__MAGIC_MIRROR] = 0;
	    sprintf(exbuf, "The Magic Mirror on %s is dispelled.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Magic Mirror on you is dispelled.\n", exbuf);
	    else
		PrintMsg(exbuf);
	}

	if (!wizflag) {
	    struct creature *cre = (struct creature *)fred;
	    if (cre->alive) {
		sprintf(exbuf, "The Dispel Magic starts to tear %s apart.\n", cre->name);
		PrintMsg(exbuf);
		cre->hitpoints = (-100);
		cre->nocorpse = 1;
	    }
	    else if (!cre->nocorpse) {
		sprintf(exbuf, "The corpse of %s disintegrates, destroyed by the Dispel Magic.\n", cre->name);
		PrintMsg(exbuf);	
		cre->nocorpse = 1;
	    }
	}
    }
    else {
	/* no dispel magic */
	if (zapl[SP__COUNTER_SPELL]) {
	    zapl[SP__SHIELD]++; /* auto-shield */

	    sprintf(exbuf, "A Counter-Spell flares around %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "Your magical senses go numb as a Counter-Spell surrounds you.\n", exbuf);
	    else
		PrintMsg(exbuf);

	    if (zapl[SP__MAGIC_MIRROR]) {
		/* counterspell beats mirror */
		zapl[SP__MAGIC_MIRROR] = 0;
		sprintf(exbuf, "The Magic Mirror on %s is destroyed by the Counter-Spell.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Magic Mirror on you is destroyed by the Counter-Spell.\n", exbuf);
		else
		    PrintMsg(exbuf);
	    }

	}

	if (zapl[SP__MAGIC_MIRROR]) {

	    sprintf(exbuf, "A Magic Mirror swirls around %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The bright haze of a Magic Mirror surrounds you.\n", exbuf);
	    else
		PrintMsg(exbuf);

	}
    }
}

static void exec_summons(self, fred, cnum, wizflag) /* no deaders or monsters */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    int ix, jx, spelnum;
    int *zapl = fred->both.zaplist;
    char *elm;
    /* counterspell, dispel magic, magic mirror have already been taken care of. */
    static int summonspells[4] = {SP__SUMMON_GOBLIN, SP__SUMMON_OGRE, SP__SUMMON_TROLL, SP__SUMMON_GIANT};
    static char *summonnames[4] = {"Goblin", "Ogre", "Troll", "Giant"};

    for (ix=0; ix<4; ix++) {
	spelnum = summonspells[ix];
	if (zapl[spelnum]) {
	    if (zapl[spelnum]==1) {
		sprintf(exbuf, "Your %s materializes by your side.\n", summonnames[ix]);
		sprintf(exbuf2, "%s's %s appears by %s side.\n", fred->both.name, summonnames[ix], pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "Your %s %ss materialize by your side.\n", number_name(zapl[spelnum]), summonnames[ix]);
		sprintf(exbuf2, "%s's %s %ss appear by %s side.\n", fred->both.name, number_name(zapl[spelnum]), summonnames[ix], pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    for (jx=0; jx<zapl[spelnum]; jx++)
		create_creature(self, ix+1, cnum);
	}
    }

    for (jx=0; jx<self->wiz[cnum]->zaplist[SP__SUMMON_ELEMENTAL]; jx++) {
	ix = self->querylist[summonelq].answer; /* 0 fire, 1 ice */
	summonelq++;
	if (!ix) {
	    sprintf(exbuf, "A blazing Fire Elemental materializes above you.\n");
	    sprintf(exbuf2, "A blazing Fire Elemental materializes above %s.\n", fred->both.name);
	}
	else {
	    sprintf(exbuf, "A glittering Ice Elemental materializes above you.\n");
	    sprintf(exbuf2, "A glittering Ice Elemental materializes above %s.\n", fred->both.name);
	}
	if (wizflag)
	    PrintMsg2(cnum, exbuf, exbuf2);
	else
	    PrintMsg(exbuf2);
	create_creature(self, 6-ix, cnum);
    }

}

/* actually, check for cancellation of creatures for any cause. */
static void check_elements(self)
struct realgame *self;
{
    int num_fire, num_ice, firel, icel;
    int ix, nukit;
    int *zapl;

    num_fire = 0;
    num_ice = 0;
    for (ix=0; ix<self->numcres; ix++) 
	if (self->cre[ix].alive) {
	    if (self->cre[ix].type==Creature_FIREL) {
		num_fire++;
		if (num_fire == 1) {
		    firel = ix;
		}
		else {
		    self->cre[firel].hitpoints = 3; /* max strength */
		    self->cre[ix].nocorpse = 1;
		    self->cre[ix].alive = 0;
		}
	    }
	    if (self->cre[ix].type==Creature_ICEL) {
		num_ice++;
		if (num_ice == 1) {
		    icel = ix;
		}
		else {
		    self->cre[icel].hitpoints = 3; /* max strength */
		    self->cre[ix].nocorpse = 1;
		    self->cre[ix].alive = 0;
		}
	    }
	}
    if (num_fire>1) {
	sprintf(exbuf, "The %s Fire Elementals merge into single raging form.\n", number_name(num_fire));
	PrintMsg(exbuf);
    }
    if (num_ice>1) {
	sprintf(exbuf, "The %s Ice Elementals merge into single whirling form.\n", number_name(num_ice));
	PrintMsg(exbuf);
    }

    ix = (num_fire ? 1 : 0)
      | (num_ice ? 2 : 0)
      | (self->fl_firestorm ? 4 : 0)
      | (self->fl_icestorm ? 8 : 0);

    switch (ix) {
	case 5: /* 0101 */
	    PrintMsg("The Fire Elemental is dispersed into the Fire Storm.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    break;
	case 10: /* 1010 */
	    PrintMsg("The Ice Elemental is dispersed into the Ice Storm.\n");
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    break;
	case 12: /* 1100 */
	    PrintMsg("The Fire and Ice Storms tear each other into shreds of vapor.\n");
	    self->fl_firestorm = 0;
	    self->fl_icestorm = 0;
	    break;
	case 3: /* 0011 */
	    PrintMsg("The Fire and Ice Elementals tear into each other, and whirl into a column of steam that dissipates in moments.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    break;
	case 6: /* 0110 */
	    PrintMsg("The Ice Elemental is dispersed into the Fire Storm, destroying both.\n");
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    self->fl_firestorm = 0;
	    break;
	case 9: /* 1001 */
	    PrintMsg("The Fire Elemental is dispersed into the Ice Storm, destroying both.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    self->fl_icestorm = 0;
	    break;
	case 13: /* 1101 */
	    PrintMsg("The Fire and Ice Storms tear each other into shreds of vapor, destroying the Fire Elemental as well.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    self->fl_firestorm = 0;
	    self->fl_icestorm = 0;
	    break;
	case 14: /* 1110 */
	    PrintMsg("The Fire and Ice Storms tear each other into shreds of vapor, destroying the Ice Elemental as well.\n");
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    self->fl_firestorm = 0;
	    self->fl_icestorm = 0;
	    break;
	case 11: /* 1011 */
	    PrintMsg("The Fire and Ice Elementals tear into each other, and whirl into a column of steam that dissipates in moments. The Ice Storm is absorbed as well.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    self->fl_icestorm = 0;
	    break;
	case 7: /* 0111 */
	    PrintMsg("The Fire and Ice Elementals tear into each other, and whirl into a column of steam that dissipates in moments. The Fire Storm is absorbed as well.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    self->fl_firestorm = 0;
	    break;
	case 15: /* 1111 */
	    PrintMsg("The Fire and Ice Elementals whirl into each other, screaming in fury, and the Fire and Ice storms shriek in response. The column of Elemental energy rises to meet the cloud of vapor that descends over the arena. You are buffeted by the conflicting energies; but within moments, nothing remains.\n");
	    self->cre[firel].nocorpse = 1;
	    self->cre[firel].alive = 0;
	    self->cre[icel].nocorpse = 1;
	    self->cre[icel].alive = 0;
	    self->fl_icestorm = 0;
	    self->fl_firestorm = 0;
	    break;
    }

    for (ix=0; ix<self->numcres; ix++) 
	if (self->cre[ix].alive) {
	    zapl = self->cre[ix].zaplist;
	    nukit = 0;

	    if (self->cre[ix].type==Creature_FIREL) {
		if (!nukit && zapl[SP__RESIST_HEAT]) {
		    sprintf(exbuf, "%s gutters and flickers out under the influence of the Resist Heat.\n", self->cre[ix].name);
		    PrintMsg(exbuf);
		    nukit = 1;
		}
	    }
	    if (self->cre[ix].type==Creature_ICEL) {
		if (!nukit && zapl[SP__RESIST_COLD]) {
		    sprintf(exbuf, "%s melts away under the influence of the Resist Cold.\n", self->cre[ix].name);
		    PrintMsg(exbuf);
		    nukit = 1;
		}
		if (!nukit && zapl[SP__FIREBALL]) {
		    sprintf(exbuf, "%s is vaporized by the Fireball.\n", self->cre[ix].name);
		    PrintMsg(exbuf);
		    nukit = 1;
		}
	    }
	    if (!nukit && zapl[SP__BLINDNESS]) {
		sprintf(exbuf, "Under the stress of the Blindness, %s shivers and disintegrates into random energies.\n", self->cre[ix].name);
		PrintMsg(exbuf);
		nukit = 1;
	    }
	    if (!nukit && zapl[SP__INVISIBILITY]) {
		sprintf(exbuf, "Under the stress of the Invisibility, %s shivers and disintegrates into random energies.\n", self->cre[ix].name);
		PrintMsg(exbuf);
		nukit = 1;
	    }

	    if (nukit) {
		self->cre[ix].alive = 0;
		self->cre[ix].nocorpse = 1;
	    }

	    /* dispel magic was taken care of earlier */
	    if (self->cre[ix].alive && zapl[SP__REMOVE_ENCHANTMENT]) {
		sprintf(exbuf, "The Remove Enchantment starts to tear %s apart.\n", self->cre[ix].name);
		PrintMsg(exbuf);
		self->cre[ix].hitpoints = (-100);
		self->cre[ix].nocorpse = 1;
	    }
	}
}

static void exec_cancels(self, fred, cnum, wizflag) /* no deaders */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    int ix;
    int *zapl = fred->both.zaplist;
    struct permstats *perm = &(fred->both.perm);
    /* counterspell, dispel magic, magic mirror have already been taken care of. */

    if (zapl[SP__RAISE_DEAD] && zapl[SP__FINGER_OF_DEATH]) {
	sprintf(exbuf2, "The Finger of Death and Raise Dead spells aimed at %s cancel each other.\n", fred->both.name);
	if (wizflag)
	    PrintMsg2(cnum, "For a moment, you feel a sudden pressure in your chest, as if an iron band was squeezing your heart. Then it fades, as the Raise Dead spell burns through you.\n", exbuf2);
	else
	    PrintMsg(exbuf2);
	zapl[SP__RAISE_DEAD]=0;
	zapl[SP__FINGER_OF_DEATH]=0;
    }

    if (zapl[SP__FIREBALL] && self->fl_icestorm) {
	sprintf(exbuf2, "The Fireball hurtling towards %s is snuffed by the Ice Storm, leaving %s enveloped in a cloud of steam.\n", fred->both.name, pro_him(fred->both.gender));
	if (wizflag)
	    PrintMsg2(cnum, "The Fireball hurtling towards you is snuffed by the Ice Storm, leaving you enveloped in warm steam.\n", exbuf2);
	else
	    PrintMsg(exbuf2);
	zapl[SP__FIREBALL]=0;
	fred->both.fl_resist_icestorm = 1;
    }

    ix = (zapl[SP__AMNESIA] ? 1 : 0)
      + (zapl[SP__CONFUSION] ? 1 : 0)
      + (zapl[SP__CHARM_PERSON] ? 1 : 0)
      + (zapl[SP__CHARM_MONSTER] ? 1 : 0)
      + (zapl[SP__PARALYSIS] ? 1 : 0)
      + (zapl[SP__FEAR] ? 1 : 0);

    if (ix>1
	|| (zapl[SP__PARALYSIS]>1 && (wizflag && fred->wiz.hand_paralyzed==(-1))) 
	|| zapl[SP__CHARM_PERSON]>1
	|| zapl[SP__CHARM_MONSTER]>1) {
	/* all mind spells cancel. */
	list_mind_spells(exbuf3, zapl);

	sprintf(exbuf, "The mind-control spells (%s) aimed at you interfere with each other and fizzle, leaving you with a pounding headache.\n", exbuf3);
	sprintf(exbuf2, "The mind-control spells (%s) aimed at %s interfere with each other and fizzle.\n", exbuf3, fred->both.name);
	if (wizflag)
	    PrintMsg2(cnum, exbuf, exbuf2);
	else
	    PrintMsg(exbuf2);

	zapl[SP__AMNESIA] = 0;
	zapl[SP__CONFUSION] = 0;
	zapl[SP__CHARM_PERSON] = 0;
	zapl[SP__CHARM_MONSTER] = 0;
	zapl[SP__PARALYSIS] = 0;
	zapl[SP__FEAR] = 0;
	fred->both.enchant_ppend = 0;
    }
    /* now only a legal set of mind spells remain, and at most one control spell, whose caster is in enchant_caster. (Which may be -1, if the spell mirrored off a creature.) */

}

static void exec_protects(self, fred, cnum, wizflag) /* no deaders */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    int *zapl = fred->both.zaplist;
    struct permstats *perm = &(fred->both.perm);
    /* counterspell, dispel magic, magic mirror have already been taken care of. */

    /* don't bother checking permanency -- it doesn't change anything */
    if (zapl[SP__RESIST_HEAT]) {
	if (!fred->both.resistant_heat) {
	    sprintf(exbuf2, "The Resist Heat shrouds %s in a cool blue veil.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Resist Heat wraps you in a pleasant blue coolness.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.resistant_heat = 1;
	}
	else {
	    sprintf(exbuf2, "%s's veil of Resist Heat continues to glow coolly.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "Your veil of Resist Heat continues to glow coolly.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }

    /* don't bother checking permanency -- it doesn't change anything */
    if (zapl[SP__RESIST_COLD]) {
	if (!fred->both.resistant_cold) {
	    sprintf(exbuf2, "The Resist Cold shrouds %s in a warm pink veil.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Resist Cold wraps you in a pleasant pink warmth.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.resistant_cold = 1;
	}
	else {
	    sprintf(exbuf2, "%s's veil of Resist Cold continues to glow warmly.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "Your veil of Resist Cold continues to glow warmly.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }

    if (zapl[SP__PROTECTION_FROM_EVIL] || perm->fl_prot_evil) {
	if (!fred->both.prot_from_evil) {
	    sprintf(exbuf, "A white circle of Protection from Evil springs up around you. You feel the Shield aura cloaking your skin.\n");
	    sprintf(exbuf2, "A white circle of Protection from Evil springs up around %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    sprintf(exbuf, "Your circle of Protection from Evil flares back to full strength.\n");
	    sprintf(exbuf2, "%s's circle of Protection from Evil flares back to full strength.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	fred->both.prot_from_evil = 4;
    }

    if (!(self->turntype==Turn_TIMESTOP && fred->both.timestop!=1)) {
	fred->both.fl_resist_heat = (fred->both.resistant_heat);
	fred->both.fl_resist_cold = (fred->both.resistant_cold);

	if (fred->both.prot_from_evil>0) {
	    int doit = 1;
	    switch (fred->both.prot_from_evil) {
		case 4:
		default:
		    doit = 0;
		    /*sprintf(exbuf, "Your circle of Protection from Evil is still glowing strongly.\n");
		    sprintf(exbuf2, "%s's circle of Protection from Evil is still glowing strongly.\n", fred->both.name);*/
		    break;
		case 3:
		    sprintf(exbuf, "Your circle of Protection from Evil is beginning to fade.\n");
		    sprintf(exbuf2, "%s's circle of Protection from Evil is beginning to fade.\n", fred->both.name);
		    break;
		case 2:
		    sprintf(exbuf, "Your circle of Protection from Evil is dimmer now.\n");
		    sprintf(exbuf2, "%s's circle of Protection from Evil is dimmer now.\n", fred->both.name);
		    break;
		case 1:
		    sprintf(exbuf, "Your circle of Protection from Evil is nearly gone.\n");
		    sprintf(exbuf2, "%s's circle of Protection from Evil is nearly gone.\n", fred->both.name);
		    break;
	    }
	    if (doit)
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);

	    fred->both.prot_from_evil--;
	    zapl[SP__SHIELD] += 64; /* auto-shield, special mark */
	}
    }
    else {
	/* the protections are timestopped and have no effect. */
    }
}

