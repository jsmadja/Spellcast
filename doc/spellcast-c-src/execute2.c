#include <stdio.h>
#include <strings.h>

#include "handwave.h"
#include "internal.h"

extern char exbuf[2048], exbuf2[2048];
extern char *number_name();

void exec_enchants(self, fred, cnum, wizflag) /* no deaders */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    int handnum;
    int *zapl = fred->both.zaplist;
    struct permstats *perm = &(fred->both.perm);
    /* counterspell, dispel magic, magic mirror have already been taken care of. */

    if (zapl[SP__AMNESIA]) {
	fred->both.mind_spell = SP__AMNESIA;
	/* fred->both.mind_caster = unused */
	if (fred->both.enchant_ppend)
	    perm->mind_spell = SP__AMNESIA;
	sprintf(exbuf2, "The Amnesia spell curls around %s's mind.\n", fred->both.name);
	if (wizflag)
	    PrintMsg2(cnum, "The Amnesia spell curls around your mind.\n", exbuf2);
	else
	    PrintMsg(exbuf2);
    }

    if (zapl[SP__CONFUSION]) {
	fred->both.mind_spell = SP__CONFUSION;
	fred->both.mind_caster = 0;
	if (fred->both.enchant_ppend) {
	    perm->mind_spell = SP__CONFUSION;
	    perm->mind_detail = (-1);
	    fred->both.mind_caster = 1;
	}
	sprintf(exbuf2, "The Confusion spell curls around %s's mind.\n", fred->both.name);
	if (wizflag)
	    PrintMsg2(cnum, "The Confusion spell curls around your mind.\n", exbuf2);
	else
	    PrintMsg(exbuf2);
    }

    if (zapl[SP__FEAR]) {
	fred->both.mind_spell = SP__FEAR;
	/* fred->both.mind_caster = unused */
	if (fred->both.enchant_ppend)
	    perm->mind_spell = SP__FEAR;
	sprintf(exbuf2, "The Fear spell curls around %s's mind.\n", fred->both.name);
	if (wizflag)
	    PrintMsg2(cnum, "The Fear spell curls around your mind.\n", exbuf2);
	else
	    PrintMsg(exbuf2);
    }

    if (zapl[SP__PARALYSIS]) {
	if (wizflag) {
	    /* cast on a wizard */
	    if (fred->both.enchant_caster == (-1)) {
		sprintf(exbuf2, "Not knowing which of %s's hands to strike, the Paralysis spell fizzles.\n", fred->both.name);
		PrintMsg2(cnum, "Not knowing which of your hands to strike, the Paralysis spell fizzles.\n", exbuf2);
	    }
	    else {
		fred->both.mind_spell = SP__PARALYSIS;

		if (fred->wiz.hand_paralyzed==(-1)) {
		    erase_queries(self);
		    add_query(self, fred->both.enchant_caster, Qu_ParalysisHand, cnum);
		    Queries(self->numqueries, self->querylist);
		    handnum = self->querylist[0].answer;
		}
		else
		    handnum = fred->wiz.hand_paralyzed;

		fred->both.mind_caster = handnum;
		if (fred->both.enchant_ppend) {
		    perm->mind_spell = SP__PARALYSIS;
		    perm->mind_detail = handnum;
		}

		sprintf(exbuf, "The Paralysis spell curls around your %s hand.\n", (handnum?"right":"left"));
		sprintf(exbuf2, "The Paralysis spell curls around %s's %s hand.\n", fred->both.name, (handnum?"right":"left"));
		PrintMsg2(cnum, exbuf, exbuf2);
	    }
	}
	else {
	    /* cast on a monster */
	    fred->both.mind_spell = SP__PARALYSIS;
	    /* fred->both.mind_caster = unused */
	    if (fred->both.enchant_ppend) 
		perm->mind_spell = SP__PARALYSIS;
	    sprintf(exbuf2, "The Paralysis spell curls around %s's mind.\n", fred->both.name);
	    PrintMsg(exbuf2);
	}
    }

    if (zapl[SP__CHARM_MONSTER]) {
	if (wizflag) {
	    /* cast on a wizard */
	    sprintf(exbuf2, "The Charm Monster spell swirls around %s's mind, but cannot sink in.\n", fred->both.name);
	    PrintMsg2(cnum, "The Charm Monster spell swirls around your mind, but cannot sink in.\n", exbuf2);
	}
	else {
	    /* cast on a monster */
	    if (fred->both.enchant_caster == (-1)) {
		sprintf(exbuf, "The Charm Monster spell strikes %s, and then seems to freeze. Referees stomp into the arena and glare at the spell. Then they glare at each other. Then they glare at the spell again. They huddle together, muttering and glowering. Eventually they pick the spell up bodily and carry it off the field.\n", fred->both.name);
		PrintMsg(exbuf);
	    }
	    else {
		fred->both.mind_spell = SP__CHARM_MONSTER;
		fred->both.mind_caster = fred->both.enchant_caster;
		if (fred->both.enchant_ppend) {
		    perm->mind_spell = SP__CHARM_MONSTER;
		    perm->mind_detail = fred->both.enchant_caster;
		}
		fred->cre.owner = fred->both.enchant_caster;
		sprintf(exbuf2, "The Charm Monster spell curls around %s, and %s smiles dopily at %s.\n", fred->both.name, pro_he(fred->both.gender), self->wiz[fred->cre.owner]->name);
		PrintMsg(exbuf2);
	    }
	}
    }

    if (zapl[SP__CHARM_PERSON]) {
	if (wizflag) {
	    /* cast on a wizard */
	    if (fred->both.enchant_caster == (-1)) {
		sprintf(exbuf2, "Not knowing which of %s's hands to strike, the Charm Person spell fizzles.\n", fred->both.name);
		PrintMsg2(cnum, "Not knowing which of your hands to strike, the Charm Person spell fizzles.\n", exbuf2);
	    }
	    else {
		fred->both.mind_spell = SP__CHARM_PERSON;

		erase_queries(self);
		add_query(self, fred->both.enchant_caster, Qu_CharmHand, cnum);
		Queries(self->numqueries, self->querylist);
		handnum = self->querylist[0].answer;

		fred->both.mind_caster = fred->both.enchant_caster + 128*handnum;
		if (fred->both.enchant_ppend) {
		    perm->mind_spell = SP__CHARM_PERSON;
		    perm->mind_detail = (-1);
		    fred->both.mind_caster |= 1024;
		}

		sprintf(exbuf, "The Charm Person spell curls around your %s hand.\n", (handnum?"right":"left"));
		sprintf(exbuf2, "The Charm Person spell curls around %s's %s hand.\n", fred->both.name, (handnum?"right":"left"));
		PrintMsg2(cnum, exbuf, exbuf2);
	    }
	}
	else {
	    /* cast on a monster */
	    sprintf(exbuf2, "The Charm Person spell swirls around %s's mind, but cannot sink in.\n", fred->both.name);
	    PrintMsg(exbuf2);
	}
    }

    /* do PERMANENT spells */
    if (fred->both.mind_spell == (-1)) {
	switch (perm->mind_spell) {
	    case SP__AMNESIA:
		fred->both.mind_spell = SP__AMNESIA;
		sprintf(exbuf2, "%s is still afflicted with Amnesia.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "You are still afflicted with Amnesia.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
		break;
	    case SP__FEAR:
		fred->both.mind_spell = SP__FEAR;
		sprintf(exbuf2, "%s is still afflicted with Fear.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "You are still afflicted with Fear.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
		break;
	    case SP__CHARM_MONSTER:
		if (!wizflag) {
		    fred->both.mind_spell = SP__CHARM_MONSTER;
		    fred->both.mind_caster = perm->mind_detail;
		    fred->cre.owner = perm->mind_detail;
		    sprintf(exbuf2, "%s is still enamoured of %s.\n", fred->both.name, self->wiz[fred->cre.owner]->name);
		    PrintMsg(exbuf2);
		}
		break;
	    case SP__CONFUSION:
		fred->both.mind_spell = SP__CONFUSION;
		fred->both.mind_caster = 1;
		sprintf(exbuf2, "%s is still afflicted with Confusion.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "You are still afflicted with Confusion.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
		break;
	    case SP__CHARM_PERSON:
		if (wizflag) {
		    fred->both.mind_spell = SP__CHARM_PERSON;
		    fred->both.mind_caster = 1024;
		    sprintf(exbuf2, "%s is still Charmed.\n", fred->both.name);
		    PrintMsg2(cnum, "You are still Charmed.\n", exbuf2);
		}
		break;
	    case SP__PARALYSIS:
		fred->both.mind_spell = SP__PARALYSIS;
		fred->both.mind_caster = perm->mind_detail;
		if (wizflag) {
		    sprintf(exbuf2, "%s's %s hand is still Paralyzed.\n", fred->both.name, ((perm->mind_detail)?"right":"left"));
		    sprintf(exbuf, "Your %s hand is still Paralyzed.\n", ((perm->mind_detail)?"right":"left"));
		    PrintMsg2(cnum, exbuf, exbuf2);
		}
		else {
		    sprintf(exbuf2, "%s is still afflicted with Paralysis.\n", fred->both.name);
		    PrintMsg(exbuf2);
		}
		break;
	    case (-1):
		/* no permanent mind spell */
		break;
	    default:
		printf("ERROR: unknown perm->mind_spell %d\n", perm->mind_spell);
		break;
	}
    }

    if (zapl[SP__DISEASE]) {
	if (fred->both.disease_time == (-1)) {
	    /* not yet diseased */
	    fred->both.disease_time = 6;
	    sprintf(exbuf2, "%s sneezes loudly.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "You sneeze loudly.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }

    if (zapl[SP__POISON]) {
	if (fred->both.poison_time == (-1)) {
	    /* not yet poisoned */
	    fred->both.poison_time = 6;
	    sprintf(exbuf2, "%s suddenly looks somewhat pale.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "A flash of pain lances along your bones, and then fades away.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }

    if (zapl[SP__ANTI_SPELL]) {
	if (zapl[SP__REMOVE_ENCHANTMENT]) {
	    /* cancelled */
	    sprintf(exbuf2, "The Anti-Spell fizzes away into nothing as it approaches %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Anti-Spell fizzes away into nothing as it approaches you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    if (!wizflag) {
		sprintf(exbuf, "The Anti-Spell whizzes through %s without any effect.\n", fred->both.name);
		PrintMsg(exbuf);
	    }
	    else {
		struct wizard *wiz = &(fred->wiz);
		sprintf(exbuf2, "The Anti-Spell jolts %s, causing the magical energies around %s hands to flicker.\n", wiz->name, pro_his(wiz->gender));
		PrintMsg2(cnum, "The Anti-Spell jolts you, interrupting the flow of magical energy in your hands.\n", exbuf2);

		write_gestures(self, wiz, Gesture_ANTISPELL, Gesture_ANTISPELL);
	    }
	}
    }

    if (zapl[SP__DELAYED_EFFECT]) {
	if (zapl[SP__REMOVE_ENCHANTMENT]) {
	    /* cancelled */
	    sprintf(exbuf2, "The Delayed Effect fizzes away into nothing as it approaches %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Delayed Effect fizzes away into nothing as it approaches you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    if (!wizflag) {
		sprintf(exbuf, "The Delayed Effect whizzes through %s without any effect.\n", fred->both.name);
		PrintMsg(exbuf);
	    }
	    else {
		struct wizard *wiz = &(fred->wiz);
		sprintf(exbuf2, "The Delayed Effect spreads out around %s.\n", wiz->name);
		PrintMsg2(cnum, "The Delayed Effect spreads out in a cloud around you, waiting for you to complete a spell.\n", exbuf2);
		wiz->delay_time = 4;
	    }
	}
    }

    if (zapl[SP__PERMANENCY]) {
	if (zapl[SP__REMOVE_ENCHANTMENT]) {
	    /* cancelled */
	    sprintf(exbuf2, "The Permanency fizzes away into nothing as it approaches %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Permanency fizzes away into nothing as it approaches you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    if (!wizflag) {
		sprintf(exbuf, "The Permanency whizzes through %s without any effect.\n", fred->both.name);
		PrintMsg(exbuf);
	    }
	    else {
		struct wizard *wiz = &(fred->wiz);
		sprintf(exbuf2, "The Permanency spreads out around %s.\n", wiz->name);
		PrintMsg2(cnum, "The Permanency spreads out in a cloud around you, waiting for you to complete a spell.\n", exbuf2);
		wiz->perm_time = 4;
	    }
	}
    }

    if (zapl[SP__BLINDNESS] || perm->fl_blindness) {
	if (fred->both.blindness == 0) {
	    /* not yet blind */
	    sprintf(exbuf2, "%s blinks, then stumbles slightly as %s eyes turn milky.\n", fred->both.name, pro_his(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "A haze suddenly appears before your eyes. The world dims rapidly, and you are shortly enveloped in total darkness.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.blindness = 4;
	}
	else {
	    sprintf(exbuf2, "%s is still blind.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "You are still blind.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.blindness = 4;
	}
    }

    if (fred->both.blindness) {
	fred->both.blindness--;
	if (fred->both.blindness==0) {
	    sprintf(exbuf2, "The milky cast fades from %s's eyes.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The dark veil falls away from your eyes.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }

    if (zapl[SP__INVISIBILITY] || perm->fl_invisibility) {
	if (fred->both.invisibility == 0) {
	    /* not yet invisible */
	    sprintf(exbuf2, "%s turns quietly translucent, and fades from view.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The world becomes strangely misted in front of your eyes. Looking down, you see that your hands are no longer visible.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.invisibility = 4;
	}
	else {
	    sprintf(exbuf2, "%s is still invisible.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "You are still invisible.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.invisibility = 4;
	}
    }

    if (fred->both.invisibility) {
	fred->both.invisibility--;
	if (fred->both.invisibility==0) {
	    sprintf(exbuf2, "A %s-shaped shadow appears, and %s rapidly becomes fully visible.\n", fred->both.name, pro_he(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "Your limbs have become visible again.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }

    if (zapl[SP__HASTE] || perm->fl_haste) {
	if (fred->both.haste == 0) {
	    /* not yet hasted */
	    fred->both.haste = 5;
	}
	else {
	    fred->both.haste = 4;
	}
    }

    if (zapl[SP__TIME_STOP]) {
	fred->both.timestop = 2;
	/* no message -- that's done later */
    }

    if (zapl[SP__REMOVE_ENCHANTMENT] || self->fl_dispelmagic) {
	/* remove enchantments is obviously last. takes out mind_* also, and nowm_* for monsters. */

	if (zapl[SP__REMOVE_ENCHANTMENT]) {
	    sprintf(exbuf2, "The Remove Enchantment whirls its dissipative energies around %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Remove Enchantment whirls its dissipative energies around you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}

	if (fred->both.mind_spell != (-1)) {
	    sprintf(exbuf, "You are freed of the %s.\n", spelllist[fred->both.mind_spell].name);
	    sprintf(exbuf2, "%s is freed of the %s.\n", fred->both.name, spelllist[fred->both.mind_spell].name);
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.mind_spell = (-1);
	}
	if (!wizflag) {
	    struct creature *thud = &(fred->cre);
	    if (thud->nowm_spell != (-1)) {
		sprintf(exbuf2, "%s is freed of the %s.\n", thud->name, spelllist[thud->nowm_spell].name);
		PrintMsg(exbuf2);
		thud->nowm_spell = (-1);
	    }
	}

	if (fred->both.resistant_heat) {
	    sprintf(exbuf, "Your blue aura of Resist Heat fades away.\n");
	    sprintf(exbuf2, "%s's blue aura of Resist Heat fades away.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	fred->both.resistant_heat = 0;
	fred->both.fl_resist_heat = 0;

	if (fred->both.resistant_cold) {
	    sprintf(exbuf, "Your pink aura of Resist Cold fades away.\n");
	    sprintf(exbuf2, "%s's pink aura of Resist Cold fades away.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	fred->both.resistant_cold = 0;
	fred->both.fl_resist_cold = 0;

	if (fred->both.disease_time != (-1)) {
	    sprintf(exbuf2, "%s seems to have stopped shivering, and %s skin is no longer flushed and damp.\n", fred->both.name, pro_his(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "Your fever suddenly abates. You feel much better.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.disease_time = (-1);
	}

	if (fred->both.poison_time != (-1)) {
	    sprintf(exbuf2, "%s's color improves dramatically.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The numbness and heaviness falls from your limbs. You feel much better.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.poison_time = (-1);
	}

	if (wizflag) {
	    if (fred->wiz.delay_time != 0) {
		sprintf(exbuf2, "The Delayed Effect spell hovering around %s disintegrates.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Delayed Effect spell hovering around you disintegrates.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
		fred->wiz.delay_time = 0;
	    }
	    /* and kill any banked spell */
	    if (fred->wiz.delay_bank != (-1)) {
		sprintf(exbuf, "The %s spell that you had caught in a Delayed Effect is lost.\n", spelllist[fred->wiz.delay_bank].name);
		sprintf(exbuf2, "The %s spell that %s had caught in a Delayed Effect is lost.\n", spelllist[fred->wiz.delay_bank].name, fred->wiz.name);
		PrintMsg2(cnum, exbuf, exbuf2);
		fred->wiz.delay_bank = (-1);
	    }
	}

	if (wizflag) {
	    if (fred->wiz.perm_time != 0) {
		sprintf(exbuf2, "The Permanency spell hovering around %s disintegrates.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Permanency spell hovering around you disintegrates.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
		fred->wiz.perm_time = 0;
	    }
	}

	if (fred->both.prot_from_evil) {
	    sprintf(exbuf2, "%s's circle of Protection flickers out.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "Your circle of Protection flickers out.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.prot_from_evil = 0;
	}

	if (fred->both.zaplist[SP__SHIELD] >= 64) {
	    fred->both.zaplist[SP__SHIELD] -= 64;
	}

	if (fred->both.haste) {
	    if (fred->both.haste!=5) {
		sprintf(exbuf, "%s slows down to normal speed.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "Everyone comes back up to normal speed.\n", exbuf);
		else
		    PrintMsg(exbuf);
	    }
	    else {
		sprintf(exbuf, "The Haste on %s is nulled.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Haste on you is nulled.\n", exbuf);
		else
		    PrintMsg(exbuf);
	    }
	    fred->both.haste = 0;
	}

	if (fred->both.timestop) {
	    sprintf(exbuf, "The Time Stop on %s is nulled.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Time Stop on you is nulled.\n", exbuf);
	    else
		PrintMsg(exbuf);
	    fred->both.timestop = 0;
	}

	if (fred->both.invisibility) {
	    sprintf(exbuf2, "%s becomes visible again, rather suddenly.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "Your body tingles and becomes visible again.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.invisibility = 0;
	}

	if (fred->both.blindness) {
	    sprintf(exbuf2, "%s's eyes abruptly clear.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "Sight bursts in upon you again.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.blindness = 0;
	}

	clear_permstats(&(fred->both.perm));

    }
}

void exec_attacks(self, fred, cnum, wizflag) /* no deaders */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    int *zapl = fred->both.zaplist;
    /* counterspell, dispel magic, magic mirror have already been taken care of. */

    if (zapl[SP__STAB]) {
	if (zapl[SP__SHIELD]) {
	    /* shield protects */
	    if (zapl[SP__STAB]==1) {
		sprintf(exbuf2, "The knife slides off of %s's Shield.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The knife slides off of your Shield.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf2, "The knives slide off of %s's Shield.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The knives slide off of your Shield.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	}
	else {
	    /* hit */
	    if (zapl[SP__STAB]==1) {
		sprintf(exbuf2, "The knife strikes %s.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The knife strikes you.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "The %s knives strike you.\n", number_name(zapl[SP__STAB]));
		sprintf(exbuf2, "The %s knives strike %s.\n", number_name(zapl[SP__STAB]), fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    fred->both.hitpoints -= 1*zapl[SP__STAB];
	}
    }

    if (zapl[SP__MISSILE]) {
	if (zapl[SP__SHIELD]) {
	    /* shield protects */
	    if (zapl[SP__MISSILE]==1) {
		sprintf(exbuf2, "The Missile shatters on %s's Shield.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Missile shatters on your Shield.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf2, "The Missiles shatter on %s's Shield.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Missiles shatter on your Shield.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	}
	else {
	    /* hit */
	    if (zapl[SP__MISSILE]==1) {
		sprintf(exbuf2, "The Missile strikes %s.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Missile strikes you.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "The %s Missiles strike you.\n", number_name(zapl[SP__MISSILE]));
		sprintf(exbuf2, "The %s Missiles strike %s.\n", number_name(zapl[SP__MISSILE]), fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    fred->both.hitpoints -= 1*zapl[SP__MISSILE];
	}
    }

    if (zapl[SP__CAUSE_LIGHT_WOUNDS]) {
	/* hit */
	if (zapl[SP__CAUSE_LIGHT_WOUNDS]==1) {
	    sprintf(exbuf2, "The Cause Light Wounds spell tears into %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Cause Light Wounds spell tears into you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    sprintf(exbuf, "The %s Cause Light Wounds spells tear into you.\n", number_name(zapl[SP__CAUSE_LIGHT_WOUNDS]));
	    sprintf(exbuf2, "The %s Cause Light Wounds spells tear into %s.\n", number_name(zapl[SP__CAUSE_LIGHT_WOUNDS]), fred->both.name); 
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	fred->both.hitpoints -= 2*zapl[SP__CAUSE_LIGHT_WOUNDS];
    }

    if (zapl[SP__CAUSE_HEAVY_WOUNDS]) {
	/* hit */
	if (zapl[SP__CAUSE_HEAVY_WOUNDS]==1) {
	    sprintf(exbuf2, "The Cause Heavy Wounds spell tears into %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Cause Heavy Wounds spell tears painfully into you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    sprintf(exbuf, "The %s Cause Heavy Wounds spells tear painfully into you.\n", number_name(zapl[SP__CAUSE_HEAVY_WOUNDS]));
	    sprintf(exbuf2, "The %s Cause Heavy Wounds spells tear into %s.\n", number_name(zapl[SP__CAUSE_HEAVY_WOUNDS]), fred->both.name); 
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	fred->both.hitpoints -= 3*zapl[SP__CAUSE_HEAVY_WOUNDS];
    }

    if (zapl[SP__LIGHTNING_BOLT]) {
	/* hit */
	if (zapl[SP__LIGHTNING_BOLT]==1) {
	    sprintf(exbuf2, "The Lightning Bolt staggers %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Lightning Bolt strikes you, staggering and nearly deafening you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    sprintf(exbuf, "The %s Lightning Bolts crash into you; the concussions nearly knock you off your feet.\n", number_name(zapl[SP__LIGHTNING_BOLT]));
	    sprintf(exbuf2, "The %s Lightning Bolts strike %s, staggering %s.\n", number_name(zapl[SP__LIGHTNING_BOLT]), fred->both.name, pro_him(fred->both.gender)); 
	    if (wizflag)
		PrintMsg2(cnum, exbuf, exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	fred->both.hitpoints -= 5*zapl[SP__LIGHTNING_BOLT];
    }

    if (zapl[SP__FIREBALL]) {
	if (fred->both.fl_resist_heat) {
	    /* heat-resistant */
	    if (zapl[SP__FIREBALL]==1) {
		sprintf(exbuf2, "The Fireball strikes %s, but the flames slide off %s Resist Heat aura.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "The Fireball strikes you, but you feel only a slight warmth.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf2, "The Fireballs strike %s, but the flames slide off %s Resist Heat aura.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "The Fireballs strike you, but you feel only a slight warmth.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	}
	else {
	    /* hit */
	    if (zapl[SP__FIREBALL]==1) {
		sprintf(exbuf2, "The Fireball strikes %s.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "The Fireball strikes you with a roar of flame.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "The %s Fireballs blast into you, surrounding you with blinding flame.\n", number_name(zapl[SP__FIREBALL]));
		sprintf(exbuf2, "The %s Fireballs strike %s.\n", number_name(zapl[SP__FIREBALL]), fred->both.name); 
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    fred->both.hitpoints -= 5*zapl[SP__FIREBALL];
	}
    }

    if (self->fl_firestorm) {
	if (zapl[SP__COUNTER_SPELL]) {
	    /* counterspelled */
	    sprintf(exbuf2, "The Counter-Spell around %s cancels the Fire Storm near %s.\n", fred->both.name, pro_him(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "The Counter-Spell cancels the Fire Storm in your vicinity.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else if (fred->both.fl_resist_heat) {
	    /* heat-resistant */
	    sprintf(exbuf2, "The Fire Storm washes over %s, who remains unharmed inside %s blue aura.\n", fred->both.name, pro_his(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "The Fire Storm washes over you, but you feel only a gentle warmth.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    /* hit */
	    sprintf(exbuf2, "The Fire Storm washes over %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Fire Storm washes over you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.hitpoints -= 5;
	}
    }

    if (self->fl_icestorm) {
	if (zapl[SP__COUNTER_SPELL]) {
	    /* counterspelled */
	    sprintf(exbuf2, "The Counter-Spell around %s cancels the Ice Storm near %s.\n", fred->both.name, pro_him(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "The Counter-Spell cancels the Ice Storm in your vicinity.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else if (fred->both.fl_resist_icestorm) {
	    /* resistant due to fireball hit -- message already printed */
	}
	else if (fred->both.fl_resist_cold) {
	    /* cold-resistant */
	    sprintf(exbuf2, "The Ice Storm washes over %s, who remains unharmed inside %s pink aura.\n", fred->both.name, pro_his(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "The Ice Storm washes over you, but you feel only a gentle coolness.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
	else {
	    /* hit */
	    sprintf(exbuf2, "The Ice Storm lashes at %s.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "The Ice Storm lashes at you.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.hitpoints -= 5;
	}
    }

    if (zapl[SP__FINGER_OF_DEATH]) {
	/* hit */
	sprintf(exbuf2, "%s turns very pale and drops to %s knees, clutching at %s chest....\n", fred->both.name, pro_his(fred->both.gender), pro_his(fred->both.gender));
	if (wizflag)
	    PrintMsg2(cnum, "You feel a sudden pressure in your chest, as if an iron band was squeezing your heart. You fall to your knees as the world dims....\n", exbuf2);
	else
	    PrintMsg(exbuf2);
	fred->both.hitpoints = (-100);
    }


}
 
void exec_heals(self, fred, cnum, wizflag) /* deaders ARE allowed */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    int *zapl = fred->both.zaplist;
    int orighp;
    /* counterspell, dispel magic, magic mirror have already been taken care of. */

    if (fred->both.alive) {
	orighp = fred->both.hitpoints;

	if (zapl[SP__CURE_LIGHT_WOUNDS]) {
	    if (zapl[SP__CURE_LIGHT_WOUNDS]==1) {
		sprintf(exbuf2, "The Cure Light Wounds spell weaves around %s, and %s wounds fade.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "The Cure Light Wounds spell weaves around you, and the pain of your wounds lessens.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "The %s Cure Light Wounds spells weave around you, and the pain of your wounds lessens.\n", number_name(zapl[SP__CURE_LIGHT_WOUNDS]));
		sprintf(exbuf2, "The %s Cure Light Wounds spells weave around %s, and %s wounds fade.\n", number_name(zapl[SP__CURE_LIGHT_WOUNDS]), fred->both.name, pro_his(fred->both.gender)); 
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    fred->both.hitpoints += 1*zapl[SP__CURE_LIGHT_WOUNDS];

	}

	if (zapl[SP__CURE_HEAVY_WOUNDS]) {
	    if (zapl[SP__CURE_HEAVY_WOUNDS]==1) {
		sprintf(exbuf2, "The Cure Heavy Wounds spell weaves around %s, and %s wounds heal.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "The Cure Heavy Wounds spell weaves around you, and your wounds heal considerably.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "The %s Cure Heavy Wounds spells weave around you, and your wounds heal considerably.\n", number_name(zapl[SP__CURE_HEAVY_WOUNDS]));
		sprintf(exbuf2, "The %s Cure Heavy Wounds spells weave around %s, and %s wounds heal.\n", number_name(zapl[SP__CURE_HEAVY_WOUNDS]), fred->both.name, pro_his(fred->both.gender)); 
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    fred->both.hitpoints += 2*zapl[SP__CURE_HEAVY_WOUNDS];

	    /* cure disease (not poison) */
	    if (fred->both.disease_time != (-1)) {
		sprintf(exbuf2, "%s seems to have stopped shivering, and %s skin is no longer flushed and damp.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "Your fever suddenly abates. You feel much better.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
		fred->both.disease_time = (-1);
	    }
	}

	if (zapl[SP__RAISE_DEAD]) {
	    if (zapl[SP__RAISE_DEAD]==1) {
		sprintf(exbuf2, "The Raise Dead spell burns through %s. Although %s is not dead, the spell has a powerful healing effect.\n", fred->both.name, pro_he(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "The Raise Dead spell burns through you. Although you aren't dead, the spell has a powerful healing influence.\n", exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    else {
		sprintf(exbuf, "The %s Raise Dead spells burn through you. Although you aren't dead, the spells have a powerful healing influence.\n", number_name(zapl[SP__RAISE_DEAD]));
		sprintf(exbuf2, "The %s Raise Dead spells burn through %s. Although %s is not dead, the spells have a powerful healing effect.\n", number_name(zapl[SP__RAISE_DEAD]), fred->both.name, pro_he(fred->both.gender)); 
		if (wizflag)
		    PrintMsg2(cnum, exbuf, exbuf2);
		else
		    PrintMsg(exbuf2);
	    }
	    fred->both.hitpoints += 5*zapl[SP__RAISE_DEAD];

	}

	if ((fred->both.hitpoints > fred->both.max_hitpoints)
	    || (fred->both.hitpoints == fred->both.max_hitpoints && fred->both.hitpoints != orighp)) {
	    fred->both.hitpoints = fred->both.max_hitpoints;
	    sprintf(exbuf2, "%s is in perfect health again.\n", fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, "You are in perfect health again.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	}
    }
    else {
	/* not alive */
	if (zapl[SP__RAISE_DEAD]) {
	    sprintf(exbuf2, "%s twitches, glows, and then jumps to %s feet! Except for a bit of incipient mold, %s is in perfect health again.\n", fred->both.name, pro_his(fred->both.gender), pro_he(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, "You are suddenly aware of a blast of energy running down every nerve! This is something of a shock, since you had just gotten used to being dead. Oh, well -- you'll be back eventually.\n", exbuf2);
	    else
		PrintMsg(exbuf2);
	    fred->both.alive = 1;
	    fred->both.hitpoints = fred->both.max_hitpoints;
	    clear_out_spells(fred, wizflag);
	    if (!wizflag) {
		fred->cre.nocorpse = 0;
		fred->cre.owner = fred->both.raisedead_caster;
	    }
	    else {
	    }
	}
    }
}
