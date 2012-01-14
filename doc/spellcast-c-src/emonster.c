#include <stdio.h>
#include <strings.h>

#include "handwave.h"
#include "internal.h"

static char bigbuf[2048];
static char bigbuf2[2048];

static void exec_monster_attack();

extern void InitBeing();

void kuruk_name()
{
#define NUMKURUK (23)

    static char kuruk_syl[NUMKURUK][5] = {"kur", "ak", "ral", "ki", "rel", "uk",
	"kor", "kul", "kas", "lok", "luk", "las", "mak", "mok", "mas", "mos", "ga",
	"tha", "gul", "lug", "mag", "mog", "ug"};

    int ix, new, old=(-1);
    int numsyl = random()%3+2;

    strcpy(bigbuf, "");

    for (ix=0; ix<numsyl; ix++) {
	do 
	    new = random()%NUMKURUK;
	while (new==old);
	strcat(bigbuf, kuruk_syl[new]);
	old = new;
    }

    if (random()%3==0 && numsyl<4) {
	strcat(bigbuf, " ");
	numsyl = random()%3+1;
	for (ix=0; ix<numsyl; ix++) {
	    do 
		new = random()%NUMKURUK;
	    while (new==old);
	    strcat(bigbuf, kuruk_syl[new]);
	    old = new;
	}
    }

    for (ix=0; bigbuf[ix]; ix++) 
	if ((ix==0 || bigbuf[ix-1]==' ') && islower(bigbuf[ix]))
	    bigbuf[ix] = toupper(bigbuf[ix]);
}

void snaffi_name()
{
#define NUMSNAFFI (18)

    static char snaffi_syl[NUMSNAFFI][5] = {"sni", "sna", "fer", "fi", "fir",
	"por", "per", "snu", "al", "an", "erl", "lep", "fru", "fri", "ig", "eg",
	"thi", "tha"};

    int ix, new, old=(-1);
    int numsyl = random()%3+1;

    strcpy(bigbuf, "");

    for (ix=0; ix<numsyl; ix++) {
	do 
	    new = random()%NUMSNAFFI;
	while (new==old);
	strcat(bigbuf, snaffi_syl[new]);
	old = new;
    }

    if (numsyl==1 || (random()%3==0 && numsyl<3)) {
	strcat(bigbuf, " ");
	numsyl = random()%2+1;
	for (ix=0; ix<numsyl; ix++) {
	    do 
		new = random()%NUMSNAFFI;
	    while (new==old);
	    strcat(bigbuf, snaffi_syl[new]);
	    old = new;
	}
    }

    for (ix=0; bigbuf[ix]; ix++) 
	if ((ix==0 || bigbuf[ix-1]==' ') && islower(bigbuf[ix]))
	    bigbuf[ix] = toupper(bigbuf[ix]);
}

void create_creature(self, type, creator)
struct realgame *self;
int type;
int creator;
{
    struct creature *thud;

    if (self->numcres+1 >= self->cre_size) {
	self->cre_size *= 2;
	self->cre = (struct creature *)realloc(self->cre, sizeof(struct creature) * self->cre_size);
    }
    thud = &self->cre[self->numcres];
    self->numcres++;

    InitBeing(thud);

    thud->nocorpse = 0;
    thud->gender = Gender_MALE;
    thud->type = type;
    thud->owner = creator;
    thud->last_target = (-1);
    thud->last_targettype = 0;
    thud->nowm_spell = -1;

    switch (type) {
	case Creature_GOBLIN:
	    thud->max_hitpoints = 1;
	    snaffi_name();
	    strcat(bigbuf, " the Goblin");
	    break;
	case Creature_OGRE:
	    thud->max_hitpoints = 2;
	    kuruk_name();
	    strcat(bigbuf, " the Ogre");
	    break;
	case Creature_TROLL:
	    thud->max_hitpoints = 3;
	    kuruk_name();
	    strcat(bigbuf, " the Troll");
	    break;
	case Creature_GIANT:
	    thud->max_hitpoints = 4;
	    kuruk_name();
	    strcat(bigbuf, " the Giant");
	    break;
	case Creature_FIREL:
	    thud->max_hitpoints = 3;
	    thud->nocorpse = 1;
	    kuruk_name();
	    strcat(bigbuf, " the Fiery");
	    break;
	case Creature_ICEL:
	    thud->max_hitpoints = 3;
	    thud->nocorpse = 1;
	    kuruk_name();
	    strcat(bigbuf, " the Icy");
	    break;
    }
    thud->hitpoints = thud->max_hitpoints;
    thud->name = (char *)malloc(strlen(bigbuf)+1);
    strcpy(thud->name, bigbuf);
}

void execute_monsters(self)
struct realgame *self;
{
    int ix, cx, jx, kx, numattacks, qtype, qnum;
    int target, targettype;
    struct creature *thud;

    erase_queries(self);

    for (ix=0; ix<self->numcres; ix++) {
	thud = &(self->cre[ix]);
	if (thud->alive) {
	    /* at this point, thud->haste is 5 if haste just cast this round;
		   3,2,1 if in effect; 4 if just cast but also previously in effect */
	    switch (self->turntype) {
		case Turn_TIMESTOP:
		    if (thud->timestop==1)
			numattacks = 1;
		    else
			numattacks = 0;
		    break;
		case Turn_NORMAL:
		    if (thud->haste>=1 && thud->haste<=4)
			numattacks = 2;
		    else
			numattacks = 1;
		    break;
		default:
		    PrintMsg("ERROR: wrong type of turn in execute_monsters()\n");
		    break;
	    }
	    for (jx=0; jx<numattacks; jx++) {
		if (thud->type==Creature_FIREL || thud->type==Creature_ICEL)
		    qtype = Qu_NoQuery;
		else
		    qtype = Qu_MonsterTarget;

		switch (thud->nowm_spell) {
		    case SP__CONFUSION:
			qtype = Qu_NoQuery;
			if (jx==0) {
			    sprintf(bigbuf, "%s suddenly looks confused.\n", thud->name);
			    PrintMsg(bigbuf);
			}
			break;
		    case SP__PARALYSIS:
			qtype = Qu_NoQuery;
			if (jx==0) {
			    sprintf(bigbuf, "%s is paralyzed and cannot attack.\n", thud->name);
			    PrintMsg(bigbuf);
			}
			break;
		    case SP__AMNESIA:
			qtype = Qu_NoQuery;
			if (jx==0) {
			    sprintf(bigbuf, "%s looks around blankly.\n", thud->name);
			    PrintMsg(bigbuf);
			}
			break;
		    default:
			break;
		}
		if (thud->owner<0 || thud->owner>=self->numplayers ||
				!self->wiz[thud->owner]->alive) {
		    qtype = Qu_NoQuery;
		}

		add_query(self, thud->owner, qtype, ix + 256*(jx+numattacks));
	    }
	}
    }
    if (self->numqueries)
	Queries(self->numqueries, self->querylist);

    for (qnum=0; qnum<self->numqueries; qnum++) {
	jx = (int)(self->querylist[qnum].rock);
	numattacks = jx / 256;
	ix = jx % 256;
	thud = &(self->cre[ix]);
	if (thud->type==Creature_FIREL || thud->type==Creature_ICEL) {
	    /* elementals are not affected by paralysis */
	    for (cx=0; cx<self->numplayers; cx++) {
		if (self->wiz[cx]->alive)
		    exec_monster_attack(self, self->wiz[cx], cx, 1, thud);
	    }
	    for (cx=0; cx<self->numcres; cx++) {
		if (self->cre[cx].alive)
		    exec_monster_attack(self, &self->cre[cx], cx, 0, thud);
	    }
	}
	else {
	    targettype = 0;
	    target = (-1);
	    if (thud->nowm_spell==SP__PARALYSIS) {
		/* no attack */
		targettype = 0;
	    }
	    else if (thud->nowm_spell==SP__CONFUSION) {
		int numtwiz, numtcre;
		if (thud->mind_caster==1 && thud->perm.mind_spell==SP__CONFUSION &&
				thud->perm.mind_detail!=(-1)) {
		    /* grab from detail */
		    targettype = thud->perm.mind_detail & QuVal_Hand_MASK;
		    target = thud->perm.mind_detail & (~QuVal_Hand_MASK);
		}
		else {
		    /* grab randomly */
		    numtwiz = NumberOfTargets(self, QuVal_Target_Wizard);
		    numtcre = NumberOfTargets(self, QuVal_Target_Creature);
		    kx = random() % (numtwiz + numtcre);
		    if (kx<numtwiz) {
			targettype = QuVal_Target_Wizard;
			target = IndexOfTarget((game *)self, targettype, kx) &
					(~QuVal_Target_MASK);
		    }
		    else {
			targettype = QuVal_Target_Creature;
			kx -= numtwiz;
			target = IndexOfTarget((game *)self, targettype, kx) &
					(~QuVal_Target_MASK);
		    }
		}
		if (thud->mind_caster==1 && thud->perm.mind_spell==SP__CONFUSION) {
		    /* set detail */
		    thud->perm.mind_detail = targettype | target;
		}
	    }
	    else if (thud->nowm_spell==SP__AMNESIA) {
		targettype = thud->last_targettype;
		target = thud->last_target;
	    }
	    else {
		int ival;
		int tart = self->querylist[qnum].answer & QuVal_Target_MASK;
		if (tart==0
		    || (thud->owner<0 || thud->owner>=self->numplayers ||
				!self->wiz[thud->owner]->alive)) {
		    targettype = 0;
		}
		else {
		    kx = self->querylist[qnum].answer & (~QuVal_Target_MASK);
		    ival = IndexOfTarget((game *)self, tart, kx);
		    target = ival & (~QuVal_Target_MASK);
		    targettype = ival & QuVal_Target_MASK;
		}
	    }
	    /*printf("### attack of %s: %d, %d\n", thud->name, targettype, target);*/
	    thud->last_targettype = targettype;
	    thud->last_target = target;
	    switch (targettype) {
		case QuVal_Target_Wizard:
		    if (self->wiz[target]->alive)
			exec_monster_attack(self, self->wiz[target], target, 1, thud);
		    break;
		case QuVal_Target_Creature:
		    if (self->cre[target].alive)
			exec_monster_attack(self, &self->cre[target], target, 0, thud);
		    break;
		default:
		    break;
	    }
	}
    }

    for (ix=0; ix<self->numcres; ix++) {
	thud = &(self->cre[ix]);
	if (thud->alive) {
	    /*printf("### cycling %s: %d => %d\n", thud->name, thud->mind_spell,
			thud->nowm_spell);*/
	    if (self->turntype==Turn_TIMESTOP && !thud->timestop) {
		/* do not cycle */
	    }
	    else {
		thud->nowm_spell  = thud->mind_spell; 
		thud->nowm_caster = thud->mind_caster; 
		thud->mind_spell  = (-1); 
		thud->mind_caster = (-1);
	    }
	}
    }
}

static void exec_monster_attack(self, fred, cnum, wizflag, thud) /* dead not allowed */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
struct creature *thud;
{
    int ix, jx;
    int *zapl;

    zapl = fred->both.zaplist;

    if (fred==(union being *)thud) {
	if (thud->type==Creature_FIREL || thud->type==Creature_ICEL) {
	    /* say nothing */
	}
	else {
	    sprintf(bigbuf, "%s refuses to attack %s.\n", thud->name,
			pro_himself(fred->both.gender));
	    PrintMsg(bigbuf);
	}
	return;
    }

    if (!(thud->type==Creature_FIREL || thud->type==Creature_ICEL)
	&& fred->both.invisibility) {
	sprintf(bigbuf, "%s tries to attack you, but cannot see you.\n", thud->name);
	sprintf(bigbuf2, "%s tries to attack %s, but cannot see %s.\n", thud->name,
			fred->both.name, pro_him(fred->both.gender));
	if (wizflag)
	    PrintMsg2(cnum, bigbuf, bigbuf2);
	else
	    PrintMsg(bigbuf2);
	return;
    }

    if (zapl[SP__SHIELD]) {
	sprintf(bigbuf, "The attack of %s is blocked by your Shield.\n", thud->name);
	sprintf(bigbuf2, "The attack of %s is blocked by %s's Shield.\n", thud->name,
			fred->both.name);
	if (wizflag)
	    PrintMsg2(cnum, bigbuf, bigbuf2);
	else
	    PrintMsg(bigbuf2);
	return;
    }

    if (thud->type==Creature_FIREL) {
	if (fred->both.fl_resist_heat) {
	    sprintf(bigbuf, "%s reaches towards you, but you feel only a warm breeze.\n",
			thud->name);
	    sprintf(bigbuf2, "%s reaches toward %s, but cannot penetrate %s blue aura.\n",
			thud->name, fred->both.name, pro_his(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, bigbuf, bigbuf2);
	    else
		PrintMsg(bigbuf2);
	}
	else {
	    sprintf(bigbuf, "%s hurls tongues of fire at you.\n", thud->name);
	    sprintf(bigbuf2, "%s hurls fire at %s.\n", thud->name, fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, bigbuf, bigbuf2);
	    else
		PrintMsg(bigbuf2);
	    fred->both.hitpoints -= 3;
	}
    }
    else if (thud->type==Creature_ICEL) {
	if (fred->both.fl_resist_cold) {
	    sprintf(bigbuf, "%s reaches towards you, but you feel only a chill breeze.\n",
			thud->name);
	    sprintf(bigbuf2, "%s reaches toward %s, but cannot penetrate %s pink aura.\n",
			thud->name, fred->both.name, pro_his(fred->both.gender));
	    if (wizflag)
		PrintMsg2(cnum, bigbuf, bigbuf2);
	    else
		PrintMsg(bigbuf2);
	}
	else {
	    sprintf(bigbuf, "%s hurls splinters of ice at you.\n", thud->name);
	    sprintf(bigbuf2, "%s hurls ice at %s.\n", thud->name, fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, bigbuf, bigbuf2);
	    else
		PrintMsg(bigbuf2);
	    fred->both.hitpoints -= 3;
	}
    }
    else {
	/* other creature types */
	if (fred==(union being *)thud) {
	    sprintf(bigbuf, "%s attacks %s!\n", thud->name,
			pro_himself(fred->both.gender));
	    PrintMsg(bigbuf);
	}
	else {
	    sprintf(bigbuf, "%s attacks you.\n", thud->name);
	    sprintf(bigbuf2, "%s attacks %s.\n", thud->name, fred->both.name);
	    if (wizflag)
		PrintMsg2(cnum, bigbuf, bigbuf2);
	    else
		PrintMsg(bigbuf2);
	}
	switch (thud->type) {
	    case Creature_GOBLIN:
		fred->both.hitpoints -= 1;
		break;
	    case Creature_OGRE:
		fred->both.hitpoints -= 2;
		break;
	    case Creature_TROLL:
		fred->both.hitpoints -= 3;
		break;
	    case Creature_GIANT:
		fred->both.hitpoints -= 4;
		break;
	}
    }
}
