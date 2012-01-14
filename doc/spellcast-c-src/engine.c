#include <stdio.h>
#include <strings.h>

#include "handwave.h"
#include "internal.h"

static char bigbuf[2048], bigbuf2[2048];
static char smallbuf[512];

extern void find_castspells();
extern void execute_spells(), execute_monsters();
extern char *pro_him(), *pro_himself(), *pro_his();

static void restrict_gestures(), build_cast_list(), pass_time();
static int analyze_endgame();
static void setup_turnactive();

void erase_queries(self)
struct realgame *self;
{
    /* free any rocks or questions that require it. At present, none do. */

    self->numqueries = 0;
}

void add_query(self, player, qtype, rock)
struct realgame *self;
int player;
int qtype;
char *rock;
{
    int qnum = self->numqueries;
    self->numqueries++;

    if (self->numqueries >= self->querylist_size) {
	self->querylist_size *= 2;
	self->querylist = (struct query *)realloc(self->querylist, sizeof(struct query) * self->querylist_size);
    }

    self->querylist[qnum].player = player;
    self->querylist[qnum].qtype = qtype;
    self->querylist[qnum].rock = rock;
}

void clear_permstats(ps)
struct permstats *ps;
{
    ps->mind_spell = -1;
    ps->fl_haste = 0;
    ps->fl_prot_evil = 0;
    ps->fl_resist_heat = 0;
    ps->fl_resist_cold = 0;
    ps->fl_blindness = 0;
    ps->fl_invisibility = 0;
}

void clear_out_spells(fred, wizflag)
union being *fred;
int wizflag;
{
    fred->both.mind_spell = (-1);
    if (!wizflag) {
	fred->cre.nowm_spell = (-1);
    }
    else {
	fred->wiz.delay_time = 0;
	fred->wiz.perm_time = 0;
	if (fred->wiz.delay_bank != (-1)) {
	    fred->wiz.delay_bank = (-1);
	}
    }
    fred->both.resistant_heat = 0;
    fred->both.fl_resist_heat = 0;
    fred->both.resistant_cold = 0;
    fred->both.fl_resist_cold = 0;
    fred->both.fl_resist_icestorm = 0;
    fred->both.mind_spell = -1;
    fred->both.disease_time = (-1);
    fred->both.poison_time = (-1);
    fred->both.prot_from_evil = 0;
    fred->both.haste = 0;
    fred->both.timestop = 0;
    fred->both.invisibility = 0;
    fred->both.blindness = 0;
    clear_permstats(&(fred->both.perm));
}

void InitBeing(fred)
union being *fred;
{
    fred->both.alive = 1;
    fred->both.gender = Gender_NONE;
    fred->both.name = NULL;
    fred->both.hitpoints = 777;
    fred->both.max_hitpoints = 888;
    fred->both.resistant_heat = 0;
    fred->both.resistant_cold = 0;
    fred->both.prot_from_evil = 0;
    fred->both.haste = 0;
    fred->both.timestop = 0;
    fred->both.invisibility = 0;
    fred->both.blindness = 0;
    fred->both.poison_time = -1;
    fred->both.disease_time = -1;
    fred->both.mind_spell = -1;
    clear_permstats(&(fred->both.perm));
    clear_round(fred);
}

char *gesture_name(val)
int val;
{
    switch (val) {
	case Gesture_DIGIT:
	    return "digit";
	    break;
	case Gesture_FINGERS:
	    return "fingers";
	    break;
	case Gesture_PALM:
	    return "palm";
	    break;
	case Gesture_WAVE:
	    return "wave";
	    break;
	case Gesture_CLAPHALF:
	    return "clap";
	    break;
	case Gesture_SNAP:
	    return "snap";
	    break;
	case Gesture_KNIFE:
	    return "stab";
	    break;
	case Gesture_NOTHING:
	    return "nothing";
	    break;
	default:
	    return "ERROR:UNKNOWN";
	    break;
    }
}

int write_gestures(self, wiz, vall, valr)
struct realgame *self;
struct wizard *wiz;
int vall, valr;
{
    int gnum;

    if (wiz->numgests+3 >= wiz->gests_size) {
	wiz->gests_size *= 2;
	wiz->gests = (struct wizgesture *)realloc(wiz->gests, wiz->gests_size * sizeof(struct wizgesture));
    }
    gnum = wiz->numgests;
    wiz->numgests++;

    wiz->gests[gnum].turnnum = self->turn;
    wiz->gests[gnum].log_hp = wiz->hitpoints;

    wiz->gests[gnum].did[0] = vall;
    wiz->gests[gnum].did[1] = valr;
    wiz->gests[gnum].invisible = (wiz->invisibility || (self->turntype==Turn_TIMESTOP));
    wiz->gests[gnum].blind = self->blind_array;

    return gnum;
}

int previous_gesture(wiz, gnum, hand)
struct wizard *wiz;
int gnum, hand;
{
    while (1) {
	gnum--;
	if (gnum<0)
	    return Gesture_NOTHING;
	if (wiz->gests[gnum].did[hand] != Gesture_ANTISPELL)
	    return wiz->gests[gnum].did[hand];
    }
}

game *BeginGame(numplayers, names, genders, callbacks, rock)
int numplayers;
char **names; /* do not free this */
int *genders; /* do not free this */
struct interface *callbacks; /* do not free this */
char *rock;
{
    struct realgame *self = (struct realgame *)malloc(sizeof(struct realgame));
    int ix;

    srandom(getpid() + rock + numplayers);

    if (!self)
	return NULL;

    self->rock = rock;
    self->callbacks = (*callbacks); /* copy structure */
    self->numplayers = numplayers;
    self->turn = 0;
    self->turntype = Turn_NORMAL;

    init_transcript(self);

    if (numplayers > MAXPLAYERS) {
	PrintMsg("Too many players requested.\n");
	return NULL;
    }

    for (ix=0; ix<self->numplayers; ix++) {
	self->wiz[ix] = (struct wizard *)malloc(sizeof(struct wizard));

	InitBeing(self->wiz[ix]);

	self->wiz[ix]->hitpoints = 15;
	self->wiz[ix]->max_hitpoints = 15;
	self->wiz[ix]->name = names[ix];
	self->wiz[ix]->gender = genders[ix];

	self->wiz[ix]->numgests = 0;
	self->wiz[ix]->gests_size = 20;
	self->wiz[ix]->gests = (struct wizgesture *)malloc(self->wiz[ix]->gests_size * sizeof(struct wizgesture));

	self->wiz[ix]->surrendered = 0;
	self->wiz[ix]->fl_cast_lightning = 0;
	self->wiz[ix]->hand_paralyzed = (-1);
	self->wiz[ix]->perm_time = 0;
	self->wiz[ix]->delay_time = 0;
	self->wiz[ix]->delay_bank = (-1);

	sprintf(bigbuf, "%s strides defiantly into the arena. The referee casts the formal Dispel Magic and Anti-Spell on %s....\n", self->wiz[ix]->name, pro_him(self->wiz[ix]->gender));
	PrintMsg2(ix, "You advance confidently into the arena. The referee casts the formal Dispel Magic and Anti-Spell on you....\n", bigbuf);
    }

    self->cre_size = 4;
    self->cre = (struct creature *)malloc(sizeof(struct creature) * self->cre_size);
    self->numcres = 0;

    self->querylist_size = 8;
    self->querylist = (struct query *)malloc(sizeof(struct query) * self->querylist_size);

    self->targetlist_size[0] = 0;
    self->targetlist_size[1] = 0;
    self->targetlist_size[2] = 0;
    self->targetlist[0] = (struct target *)malloc(4);
    self->targetlist[1] = (struct target *)malloc(4);
    self->targetlist[2] = (struct target *)malloc(4);

    self->castlist = NULL;
    self->hastelist = NULL;

#if 0 /* to test name-generation */
    {
	ix = 1;
	while (1) {
	    create_creature(self, ix, 0);
	    printf("%s\n", self->cre[self->numcres-1].name);
	    ix++;
	    if (ix==7) ix=1;
	}
    }
#endif

    setup_targetlist(self);
    setup_turnactive(self);

    return (game *)self;
}

/* returns -1 if game is still going; playernumber if someone won; MAXPLAYERS for a draw */
int RunTurn(pgame, moves)
game *pgame;
int *moves;
/* moves contains two ints for each player (left, right). The encoding is with Gesture_*. */
{
    struct realgame *self = (struct realgame *)pgame;
    int ix, jx, kx, gnum;
    int val, vall, valr;
    int numdead, numsurr, numlive;
    int *foundlist;
    struct castspell **cpt;

    self->blind_array = 0;
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->blindness)
	    self->blind_array |= (1<<ix);
    }

    /* CHOOSE GESTURES */
    for (ix=0; ix<self->numplayers; ix++)
	if (self->turnactive[ix]) {
	    struct wizard *wiz = self->wiz[ix];

	    vall = moves[ix*2];
	    if (vall<0 || vall>=NUMGESTURES)
		vall = Gesture_NOTHING;
	    valr = moves[ix*2+1];
	    if (valr<0 || valr>=NUMGESTURES)
		valr = Gesture_NOTHING;

	    gnum = write_gestures(self, wiz, vall, valr);

	    wiz->hand_paralyzed = (-1);
	    switch (wiz->mind_spell) {
		case SP__AMNESIA:
		    wiz->gests[gnum].did[0] = previous_gesture(wiz, gnum, 0);
		    wiz->gests[gnum].did[1] = previous_gesture(wiz, gnum, 1);
		    sprintf(bigbuf, "In a fit of absent-mindedness, %s repeats %s gestures from last turn: %s with the left hand, %s with the right.\n", wiz->name, pro_his(wiz->gender), gesture_name(wiz->gests[gnum].did[0]), gesture_name(wiz->gests[gnum].did[1]));
		    PrintMsg(bigbuf);
		    break;
		case SP__CONFUSION:
		    if (wiz->mind_caster==1 && wiz->perm.mind_spell==SP__CONFUSION && wiz->perm.mind_detail!=(-1)) {
			jx = (wiz->perm.mind_detail & QuVal_Hand_Left) ? 0 : 1;
			wiz->gests[gnum].did[jx] = wiz->perm.mind_detail & (~QuVal_Hand_MASK);
		    }
		    else {
			jx = random() % 2;
			wiz->gests[gnum].did[jx] = (random() % 6)+1;
		    }
		    if (wiz->mind_caster==1 && wiz->perm.mind_spell==SP__CONFUSION) {
			wiz->perm.mind_detail = wiz->gests[gnum].did[jx] | (jx ? QuVal_Hand_Right : QuVal_Hand_Left);
		    }
		    sprintf(bigbuf, "%s accidentally makes a %s with %s %s hand.\n", wiz->name, gesture_name(wiz->gests[gnum].did[jx]), pro_his(wiz->gender), (jx?"right":"left"));
		    PrintMsg(bigbuf);
		    break;
		case SP__CHARM_PERSON:
		    if ((wiz->mind_caster & 1024) && wiz->perm.mind_spell==SP__CHARM_PERSON && wiz->perm.mind_detail!=(-1)) {
			/* grab detail */
			jx = (wiz->perm.mind_detail & QuVal_Hand_Left) ? 0 : 1;
			wiz->gests[gnum].did[jx] = wiz->perm.mind_detail & (~QuVal_Hand_MASK);
		    }
		    else {
			/* ask */
			int tmpcast = wiz->mind_caster & (~1024);
			if (tmpcast >= 128) {
			    jx = 1;
			    kx = tmpcast - 128;
			}
			else {
			    jx = 0;
			    kx = tmpcast;
			};
			erase_queries(self);
			val = (ix+jx*128);
			add_query(self, kx, Qu_CharmGesture, (char *)val);
			Queries(self->numqueries, self->querylist);
			wiz->gests[gnum].did[jx] = self->querylist[0].answer;
		    }
		    if ((wiz->mind_caster & 1024) && wiz->perm.mind_spell==SP__CHARM_PERSON) {
			wiz->perm.mind_detail = wiz->gests[gnum].did[jx] | (jx ? QuVal_Hand_Right : QuVal_Hand_Left);
		    }
		    sprintf(bigbuf, "%s mysteriously makes a %s with %s %s hand.\n", wiz->name, gesture_name(wiz->gests[gnum].did[jx]), pro_his(wiz->gender), (jx?"right":"left"));
		    PrintMsg(bigbuf);
		    break;
		case SP__FEAR:
		    jx=0;
		    val = wiz->gests[gnum].did[0];
		    if (val==Gesture_NOTHING || val==Gesture_DIGIT || val==Gesture_FINGERS || val==Gesture_SNAP || val==Gesture_CLAPHALF) {
			wiz->gests[gnum].did[0] = Gesture_NOTHING;
			jx += 1;
		    }
		    val = wiz->gests[gnum].did[1];
		    if (val==Gesture_NOTHING || val==Gesture_DIGIT || val==Gesture_FINGERS || val==Gesture_SNAP || val==Gesture_CLAPHALF) {
			wiz->gests[gnum].did[1] = Gesture_NOTHING;
			jx += 2;
		    }
		    switch (jx) {
			case 1:
			    sprintf(bigbuf, "Terrified, %s does nothing with %s left hand.\n", wiz->name, pro_his(wiz->gender));
			    break;
			case 2:
			    sprintf(bigbuf, "Terrified, %s does nothing with %s right hand.\n", wiz->name, pro_his(wiz->gender));
			    break;
			case 3:
			    sprintf(bigbuf, "Terrified, %s does nothing with either hand.\n", wiz->name);
			    break;
			case 0:
			    sprintf(bigbuf, "%s is terrified, but manages %s gestures anyway.\n", wiz->name, pro_his(wiz->gender));
			    break;
		    }
		    PrintMsg(bigbuf);
		    break;
		case SP__PARALYSIS:
		    jx = wiz->mind_caster;
		    switch (previous_gesture(wiz, gnum, jx)) {
			case Gesture_PALM:
			case Gesture_WAVE:
			    val = Gesture_PALM;
			    break;
			case Gesture_DIGIT:
			case Gesture_SNAP:
			    val = Gesture_DIGIT;
			    break;
			case Gesture_FINGERS:
			case Gesture_CLAPHALF:
			    val = Gesture_FINGERS;
			    break;
			case Gesture_KNIFE:
			    val = Gesture_KNIFE;
			    break;
			case Gesture_NOTHING:
			    val = Gesture_NOTHING;
			    break;
			default:
			    PrintMsg("ERROR: paralysis previous_gesture returned bad value.\n");
			    val = Gesture_NOTHING;
			    break;
		    }
		    wiz->gests[gnum].did[jx] = val;
		    wiz->hand_paralyzed = jx;
		    sprintf(bigbuf, "%s's %s hand is frozen in a %s.\n", wiz->name, (jx?"right":"left"), gesture_name(wiz->gests[gnum].did[jx]));
		    PrintMsg(bigbuf);
		    break;
		default:
		    break;
	    }
	    if (self->turntype != Turn_HASTE)
		wiz->mind_spell = (-1); /* for wizards, it's over, man */
	    else {
		/* spell will affect him for the second half of his hasted turn, so don't wipe it. */
	    }

	    /* move this down? */
	    if (wiz->gests[gnum].did[0] == Gesture_KNIFE
		&& wiz->gests[gnum].did[1] == Gesture_KNIFE) {
		PrintMsg2(ix, "You cannot stab with both hands!\n", NULL);
		wiz->gests[gnum].did[1] = Gesture_NOTHING;
	    }
	} /* end if self->turnactive[ix] */

    log_round_header(self);
    erase_queries(self);

    /* CHECK FOR SPELLS */
    for (ix=0; ix<self->numplayers; ix++) 
	if (self->turnactive[ix]) {
	    foundlist = self->wiz[ix]->foundlist;

	    find_castspells(foundlist, self->wiz[ix]->numgests, self->wiz[ix]->gests);

	    /* CHECK SURRENDER */
	    if (foundlist[SP__SURRENDER]) {
		foundlist[SP__SURRENDER] = 0;
		self->wiz[ix]->surrendered = 1;
		sprintf(bigbuf, "%s makes the gesture of surrender!\n", self->wiz[ix]->name);
		PrintMsg2(ix, "Oh, dear. You seem to have surrendered.\n", bigbuf);
	    }

	    restrict_gestures(self, ix);
	} /* end if self->turnactive[ix] */

    Queries(self->numqueries, self->querylist); /* "what spell do you want ot cast with your * hand?" questions. */
    build_cast_list(self);
    if (self->turntype == Turn_HASTE) {
	self->hastelist = self->castlist;
	self->castlist = NULL;
	/* self->turn is not incremented */
	self->turntype = Turn_NORMAL;
	setup_targetlist(self);
	setup_turnactive(self);
	return (-1);
    }
    /* else self->turntype == Turn_NORMAL or Turn_TIMESTOP */
    if (self->hastelist) {
	for (cpt = (&(self->castlist)); (*cpt); cpt = (&((*cpt)->next)));
	(*cpt) = self->hastelist;
	self->hastelist = NULL;
    }
    execute_spells(self);
    execute_monsters(self);
    pass_time(self);

    /* CHECK FOR END-OF-GAME */
    numlive = 0;
    numdead = 0;
    numsurr = 0;
    for (ix=0; ix<self->numplayers; ix++) {
	if (!self->wiz[ix]->alive)
	    numdead++;
	else if (self->wiz[ix]->surrendered)
	    numsurr++;
	else
	    numlive++;
    }
    if (numlive <= 1) {
	setup_targetlist(self); /* well, just because */
	return analyze_endgame(self, numlive, numdead, numsurr);
    }

    /* SURRENDERED WIZARDS ARE NOW EFFECTIVELY DEAD */
    for (ix=0; ix<self->numplayers; ix++) 
	if (self->wiz[ix]->surrendered)
	    self->wiz[ix]->alive = 0;

    /* FIGURE OUT WHAT KIND OF TURN IS NEXT */
    if (self->turntype==Turn_NORMAL)
	self->turn++;
    self->turntype = Turn_NORMAL;

    jx = 0;
    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive && self->wiz[ix]->timestop) {
	    self->wiz[ix]->timestop--;
	    if (self->wiz[ix]->timestop) {
		jx++;
		sprintf(bigbuf, "%s accelerates into a flickering blur.\n", self->wiz[ix]->name);
		PrintMsg2(ix, "Everything around you stops dead. The world is silent and motionless.\n", bigbuf);
	    }
	    else {
		sprintf(bigbuf, "%s reappears, moving at normal speed.\n", self->wiz[ix]->name);
		PrintMsg2(ix, "Everything begins moving again.\n", bigbuf);
	    }
	}
    }
    for (ix=0; ix<self->numcres; ix++) {
	if (self->cre[ix].alive && self->cre[ix].timestop) {
	    self->cre[ix].timestop--;
	    if (self->cre[ix].timestop) {
		jx++;
		sprintf(bigbuf, "%s accelerates into a flickering blur.\n", self->cre[ix].name);
		PrintMsg(bigbuf);
	    }
	    else {
		sprintf(bigbuf, "%s reappears, moving at normal speed.\n", self->cre[ix].name);
		PrintMsg(bigbuf);
	    }
	}
    }
    if (jx) {
	self->turntype = Turn_TIMESTOP;
    }
    else {
	/* no timestop... */
	jx = 0;
	for (ix=0; ix<self->numplayers; ix++) {
	    if (self->wiz[ix]->alive && self->wiz[ix]->haste) {
		self->wiz[ix]->haste--;
		if (self->wiz[ix]->haste==4) {
		    self->wiz[ix]->haste--;
		    jx++;
		    sprintf(bigbuf, "%s begins moving very quickly.\n", self->wiz[ix]->name);
		    PrintMsg2(ix, "Everyone else begins moving very slowly.\n", bigbuf);
		}
		else if (self->wiz[ix]->haste) {
		    jx++;
		    sprintf(bigbuf, "%s is still moving quickly.\n", self->wiz[ix]->name);
		    PrintMsg2(ix, "Everyone else is still moving slowly.\n", bigbuf);
		}
		else {
		    sprintf(bigbuf, "%s slows down to normal speed.\n", self->wiz[ix]->name);
		    PrintMsg2(ix, "Everyone comes back up to normal speed.\n", bigbuf);
		}
	    }
	}
	for (ix=0; ix<self->numcres; ix++) {
	    if (self->cre[ix].alive && self->cre[ix].haste) {
		self->cre[ix].haste--;
		if (self->cre[ix].haste==4) {
		    self->cre[ix].haste--;
		    jx++;
		    sprintf(bigbuf, "%s begins moving very quickly.\n", self->cre[ix].name);
		    PrintMsg(bigbuf);
		}
		else if (self->cre[ix].haste) {
		    jx++;
		    sprintf(bigbuf, "%s is still moving quickly.\n", self->cre[ix].name);
		    PrintMsg(bigbuf);
		}
		else {
		    sprintf(bigbuf, "%s slows down to normal speed.\n", self->cre[ix].name);
		    PrintMsg(bigbuf);
		}
	    }
	}
	if (jx) {
	    self->turntype = Turn_HASTE;
	}
    }

    setup_targetlist(self);
    setup_turnactive(self);
    return (-1);
}

void SeeGesture(pgame, player, asker, buf, size)
game *pgame;
int player, asker;
int *buf;
int size;
{
    struct realgame *self = (struct realgame *)pgame;
    struct wizard *wiz = self->wiz[player];

    int ix, gnum;
    for (ix=0, gnum=wiz->numgests-1; ix<size; ix++, gnum--) {
	if (gnum<0)
	    buf[ix] = (-1);
	else {
	    if (player!=asker &&
		(wiz->gests[gnum].invisible || (wiz->gests[gnum].blind & (1<<asker))))
		buf[ix] = Gesture_UNCLEAR | (Gesture_UNCLEAR << 5);
	    else
		buf[ix] = (wiz->gests[gnum].did[0]) | ((wiz->gests[gnum].did[1]) << 5);
	}
    }
}

void FreeGame(pgame)
game *pgame;
{
    struct realgame *self = (struct realgame *)pgame;
    int ix;

    for (ix=0; ix<self->numplayers; ix++) {
	free((char *)self->wiz[ix]);
    }
    
    for (ix=0; ix<self->numcres; ix++) {
	free(self->cre[ix].name);
    }
    free((char *)self->cre);

    free((char *)self->querylist);

    free((char *)self);
}

void setup_targetlist(self)
struct realgame *self;
{
    int ix, res;

    if (self->numplayers >= self->targetlist_size[0]) {
	self->targetlist_size[0] = self->numplayers*2;
	self->targetlist[0] = (struct target *)realloc(self->targetlist[0], sizeof(struct target) * self->targetlist_size[0]);
    }
    if (self->numcres >= self->targetlist_size[1]) {
	self->targetlist_size[1] = (self->numcres+1)*2;
	self->targetlist[1] = (struct target *)realloc(self->targetlist[1], sizeof(struct target) * self->targetlist_size[1]);
    }
    if ((self->numplayers+self->numcres) >= self->targetlist_size[2]) {
	self->targetlist_size[2] = (self->numplayers+self->numcres)*2;
	self->targetlist[2] = (struct target *)realloc(self->targetlist[2], sizeof(struct target) * self->targetlist_size[2]);
    }

    res = 0;
    for (ix=0; ix<self->numplayers; ix++)
	if (self->wiz[ix]->alive) {
	    self->targetlist[0][res].index = ix | QuVal_Target_Wizard;
	    self->targetlist[0][res].name = self->wiz[ix]->name;
	    self->targetlist[0][res].stuff = StuffAboutBeing((game *)self, QuVal_Target_Wizard, ix);
	    res++;
	}
    self->numtargets[0] = res;

    res = 0;
    for (ix=0; ix<self->numcres; ix++)
	if (self->cre[ix].alive) {
	    self->targetlist[1][res].index = ix | QuVal_Target_Creature;
	    self->targetlist[1][res].name = self->cre[ix].name;
	    self->targetlist[1][res].stuff = StuffAboutBeing((game *)self, QuVal_Target_Creature, ix);
	    res++;
	}
    self->numtargets[1] = res;

    res = 0;
    for (ix=0; ix<self->numplayers; ix++)
	if (!self->wiz[ix]->alive && !self->wiz[ix]->surrendered) {
	    self->targetlist[2][res].index = ix | QuVal_Target_Wizard;
	    self->targetlist[2][res].name = self->wiz[ix]->name;
	    self->targetlist[2][res].stuff = 0;
	    res++;
	}
    for (ix=0; ix<self->numcres; ix++)
	if (!self->cre[ix].alive && !self->cre[ix].nocorpse) {
	    self->targetlist[2][res].index = ix | QuVal_Target_Creature;
	    self->targetlist[2][res].name = self->cre[ix].name;
	    self->targetlist[2][res].stuff = 0;
	    res++;
	}
    self->numtargets[2] = res;

}

int NumberOfTargets(pgame, targettype)
game *pgame;
int targettype;
{
    struct realgame *self = (struct realgame *)pgame;

    if (targettype==QuVal_Target_Wizard) {
	return self->numtargets[0];
    }
    else if (targettype==QuVal_Target_Creature) {
	return self->numtargets[1];
    }
    else if (targettype==QuVal_Target_Corpse) {
	return self->numtargets[2];
    }
    else return 0;
}

int NumberOfBeings(pgame, targettype)
game *pgame;
int targettype;
{
    struct realgame *self = (struct realgame *)pgame;

    if (targettype==QuVal_Target_Wizard) {
	return self->numplayers;
    }
    else if (targettype==QuVal_Target_Creature) {
	return self->numcres;
    }
    else return 0;
}

char *NameOfTarget(pgame, targettype, targetnum)
game *pgame;
int targettype, targetnum;
{
    struct realgame *self = (struct realgame *)pgame;

    if (targettype==QuVal_Target_Wizard) {
	if (targetnum >= 0 && targetnum < self->numtargets[0])
	    return self->targetlist[0][targetnum].name;
	return "ERROR:NOBODY-wiz";
    }
    else if (targettype==QuVal_Target_Creature) {
	if (targetnum >= 0 && targetnum < self->numtargets[1])
	    return self->targetlist[1][targetnum].name;
	return "ERROR:NOBODY-cre";
    }
    else if (targettype==QuVal_Target_Corpse) {
	if (targetnum >= 0 && targetnum < self->numtargets[2])
	    return self->targetlist[2][targetnum].name;
	return "ERROR:NOBODY-corpse";
    }
    else return "ERROR:NOBODY-atall";
}


int IndexOfTarget(pgame, targettype, targetnum)
game *pgame;
int targettype, targetnum;
{
    struct realgame *self = (struct realgame *)pgame;

    if (targettype==QuVal_Target_Wizard) {
	if (targetnum >= 0 && targetnum < self->numtargets[0])
	    return self->targetlist[0][targetnum].index;
	return (-1);
    }
    else if (targettype==QuVal_Target_Creature) {
	if (targetnum >= 0 && targetnum < self->numtargets[1])
	    return self->targetlist[1][targetnum].index;
	return (-1);
    }
    else if (targettype==QuVal_Target_Corpse) {
	if (targetnum >= 0 && targetnum < self->numtargets[2])
	    return self->targetlist[2][targetnum].index;
	return (-1);
    }
    else return (-1);
}

int StuffAboutTarget(pgame, targettype, targetnum)
game *pgame;
int targettype, targetnum;
{
    struct realgame *self = (struct realgame *)pgame;

    if (targettype==QuVal_Target_Wizard) {
	if (targetnum >= 0 && targetnum < self->numtargets[0])
	    return self->targetlist[0][targetnum].stuff;
	return 0;
    }
    else if (targettype==QuVal_Target_Creature) {
	if (targetnum >= 0 && targetnum < self->numtargets[1])
	    return self->targetlist[1][targetnum].stuff;
	return 0;
    }
    else if (targettype==QuVal_Target_Corpse) {
	if (targetnum >= 0 && targetnum < self->numtargets[2])
	    return self->targetlist[2][targetnum].stuff;
	return 0;
    }
    else return 0;
}

/* -1 if dead, -2 if no corpse, surrendered, or out of bounds */
int HitPointsOfBeing(pgame, targettype, indexnum)
game *pgame;
int targettype, indexnum;
{
    struct realgame *self = (struct realgame *)pgame;
    int ix;

    if (targettype==QuVal_Target_Wizard) {
	if (indexnum < self->numplayers) {
	    ix = self->wiz[indexnum]->hitpoints;
	    if (ix<0) ix=0;
	    if (self->wiz[indexnum]->alive)
		return ix;
	    if (self->wiz[indexnum]->surrendered)
		return (-2);
	    else
		return (-1);
	}
	else
	    return -2;
    }
    else if (targettype==QuVal_Target_Creature) {
	if (indexnum < self->numcres) {
	    ix = self->cre[indexnum].hitpoints;
	    if (ix<0) ix=0;
	    if (self->cre[indexnum].alive)
		return ix;
	    if (self->cre[indexnum].nocorpse)
		return (-2);
	    else
		return (-1);
	}
	else
	    return -2;
    }
    else return -2;
}

int StuffAboutBeing(pgame, targettype, indexnum)
game *pgame;
int targettype, indexnum;
{
    struct realgame *self = (struct realgame *)pgame;
    union being *fred;
    int res;

    if (targettype==QuVal_Target_Wizard) {
	if (indexnum < self->numplayers) {
	    fred = (union being *)self->wiz[indexnum];
	}
	else
	    return 0;
    }
    else if (targettype==QuVal_Target_Creature) {
	if (indexnum < self->numcres) {
	    fred = (union being *)&(self->cre[indexnum]);
	}
	else
	    return 0;
    }
    else return 0;

    res=0;
    if (fred->both.resistant_heat)
	res |= Stuff_RESIST_HEAT;
    if (fred->both.resistant_cold)
	res |= Stuff_RESIST_COLD;
    if (fred->both.prot_from_evil)
	res |= Stuff_PROTECT_EVIL;
    if (fred->both.invisibility)
	res |= Stuff_INVISIBLE;
    if (fred->both.blindness)
	res |= Stuff_BLIND;
    if (fred->both.poison_time != (-1))
	res |= Stuff_POISON;
    if (fred->both.disease_time != (-1))
	res |= Stuff_DISEASE;
    
    return res;
}

int OwnerOfCreature(pgame, indexnum)
game *pgame;
int indexnum;
{
    struct realgame *self = (struct realgame *)pgame;
    int res = self->cre[indexnum].owner;

    if (res<0 || !self->wiz[res]->alive)
	return (-1);
    else
	return res;
}

char *NameOfBeing(pgame, targettype, indexnum)
game *pgame;
int targettype, indexnum;
{
    struct realgame *self = (struct realgame *)pgame;
    int ix;

    if (targettype==QuVal_Target_Wizard) {
	if (indexnum < self->numplayers) {
	    return self->wiz[indexnum]->name;
	}
	else
	    return NULL;
    }
    else if (targettype==QuVal_Target_Creature) {
	if (indexnum < self->numcres) {
	    return self->cre[indexnum].name;
	}
	else
	    return NULL;
    }
    else return NULL;
}

int TurnType(pgame)
game *pgame;
{
    struct realgame *self = (struct realgame *)pgame;

    return self->turntype;
}

/* this is called after surrenders are killed */
static void setup_turnactive(self)
struct realgame *self;
{
    int ix;

    for (ix=0; ix<MAXPLAYERS; ix++)
	self->turnactive[ix] = 0;

    switch (self->turntype) {
	case Turn_NORMAL:
	    for (ix=0; ix<self->numplayers; ix++) {
		self->turnactive[ix] = self->wiz[ix]->alive;
	    }
	    break;
	case Turn_HASTE:
	    for (ix=0; ix<self->numplayers; ix++) {
		self->turnactive[ix] = (self->wiz[ix]->alive && self->wiz[ix]->haste);
	    }
	    break;
	case Turn_TIMESTOP:
	    for (ix=0; ix<self->numplayers; ix++) {
		self->turnactive[ix] = (self->wiz[ix]->alive && self->wiz[ix]->timestop);
	    }
	    break;
	default:
	    PrintMsg("ERROR: Unknown turn type\n");
	    break;
    }
}

int TurnPlayerActive(pgame, player)
game *pgame;
int player;
{
    struct realgame *self = (struct realgame *)pgame;

    return self->turnactive[player];
}

/* go over wiz->foundlist and generate queries as to which spell is desired */
static void restrict_gestures(self, wiznum)
struct realgame *self;
int wiznum;
{
    int jx;
    int lcount=0, rcount=0;
    int *llist = self->wiz[wiznum]->llist;
    int *rlist = self->wiz[wiznum]->rlist;
    int *foundlist = self->wiz[wiznum]->foundlist;

    for (jx=0; jx<NUMSPELLS; jx++) {
	if (foundlist[jx]) {
	    switch (foundlist[jx]) {
		case MASK_LEFT:
		    lcount++;
		    llist[lcount] = jx;
		    break;
		case MASK_RIGHT:
		    rcount++;
		    rlist[rcount] = jx;
		    break;
		case MASK_LEFT|MASK_RIGHT:
		    lcount++;
		    llist[lcount] = jx;
		    rcount++;
		    rlist[rcount] = jx;
		    break;
		case MASK_TWOHAND:
		    lcount++;
		    llist[lcount] = jx | QuVal_Hand_Both;
		    rcount++;
		    rlist[rcount] = jx | QuVal_Hand_Both;
		    break;
		default:
		    PrintMsg("ERROR in restrict_gestures\n");
		    break;
	    }
	}
    }
    llist[0] = lcount;
    rlist[0] = rcount;

    if (lcount>1) {
	add_query(self, wiznum, Qu_LeftHand, (char *)llist);
    }
    if (rcount>1) {
	add_query(self, wiznum, Qu_RightHand, (char *)rlist);
    }

    if (self->wiz[wiznum]->delay_bank != (-1))
	add_query(self, wiznum, Qu_SetOffDelay, (char *)self->wiz[wiznum]->delay_bank);
}

static int spelltargetlist[NUMSPELLS] = {
    Qu_TargetWizard, Qu_TargetWizard, Qu_TargetBeingNone, Qu_TargetBeingNone,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetBeingNone,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetWizardNone, Qu_TargetRaiseDead,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetWizardNone, Qu_TargetBeingNone,
    Qu_TargetBeingNone, Qu_NoQuery, Qu_TargetBeingNone, Qu_TargetBeingNone,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetWizardNone, Qu_TargetBeingNone,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetWizardNone, Qu_TargetWizardNone,
    Qu_TargetWizardNone, Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetBeingNone,
    Qu_NoQuery, Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetWizardNone,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_NoQuery, Qu_TargetBeingNone,
    Qu_TargetBeingNone, Qu_TargetBeingNone, Qu_TargetBeingNone
};

/* use llist and rlist together with self->querylist answers (including time delay) to create a spell list. Then create new querylist about targets, and ask it. */
static void build_cast_list(self)
struct realgame *self;
{
    struct castspell *res, *tmp;
    int wiznum, jx, kx;
    struct query *qtmp;

    /* free last turn's castlist */
    while (self->castlist) {
	tmp = self->castlist->next;
	free((char *)self->castlist);
	self->castlist = tmp;
    }

    for (wiznum=0; wiznum<self->numplayers; wiznum++) 
	if (self->turnactive[wiznum]) {
	    int *llist = self->wiz[wiznum]->llist;
	    int *rlist = self->wiz[wiznum]->rlist;
	    int lspel, rspel;

	    if (llist[0]==0)
		lspel = (-1);
	    else if (llist[0]==1)
		lspel = llist[1];
	    else {
		for (jx=0; jx<self->numqueries; jx++)
		    if (self->querylist[jx].player == wiznum
			&& self->querylist[jx].qtype == Qu_LeftHand)
			break;
		lspel = llist[1 + self->querylist[jx].answer];
	    }
	    if (rlist[0]==0)
		rspel = (-1);
	    else if (rlist[0]==1)
		rspel = rlist[1];
	    else {
		for (jx=0; jx<self->numqueries; jx++)
		    if (self->querylist[jx].player == wiznum
			&& self->querylist[jx].qtype == Qu_RightHand)
			break;
		rspel = rlist[1 + self->querylist[jx].answer];
	    }

	    if (lspel>=0 && (lspel & QuVal_Hand_Both)) {
		if (lspel==rspel) {
		    res = (struct castspell *)malloc(sizeof(struct castspell));
		    res->caster = wiznum;
		    res->handage = MASK_TWOHAND;
		    res->spellnum = lspel & (~QuVal_Hand_Both);
		    res->permanent = 0;
		    res->next = self->castlist;
		    self->castlist = res;
		    lspel = (-1);
		    rspel = (-1);
		}
		else
		    lspel = (-1);
	    }
	    if (rspel>=0 && (rspel & QuVal_Hand_Both)) {
		rspel = (-1);
	    }
	    /* only one-handed spells remain */
	    if (lspel>=0) {
		res = (struct castspell *)malloc(sizeof(struct castspell));
		res->caster = wiznum;
		res->handage = MASK_LEFT;
		res->spellnum = lspel;
		res->permanent = 0;
		res->next = self->castlist;
		self->castlist = res;
	    }
	    if (rspel>=0) {
		res = (struct castspell *)malloc(sizeof(struct castspell));
		res->caster = wiznum;
		res->handage = MASK_RIGHT;
		res->spellnum = rspel;
		res->permanent = 0;
		res->next = self->castlist;
		self->castlist = res;
	    }

	    if (self->wiz[wiznum]->delay_bank != (-1)) {
		for (jx=0; jx<self->numqueries; jx++)
		    if (self->querylist[jx].player == wiznum
			&& self->querylist[jx].qtype == Qu_SetOffDelay)
			break;
		if (self->querylist[jx].answer) {
		    res = (struct castspell *)malloc(sizeof(struct castspell));
		    res->caster = wiznum;
		    res->handage = 0;
		    res->spellnum = self->wiz[wiznum]->delay_bank;
		    res->permanent = 0;
		    res->next = self->castlist;
		    self->castlist = res;
		    self->wiz[wiznum]->delay_bank = (-1);
		}
	    }
	}  /* end if self->turnactive[ix] */

    erase_queries(self);

    for (wiznum=0; wiznum<self->numplayers; wiznum++) 
	if (self->turnactive[wiznum]) {
	    struct wizard *wiz = self->wiz[wiznum];

	    /* DELAYED EFFECT */
	    if (wiz->delay_time) {
		struct castspell *dellist[10]; /* that had better be enough */
		int delnum[11];

		int numdel = 0;
		for (tmp=self->castlist; tmp; tmp=tmp->next) {
		    if (tmp->caster == wiznum
			&& tmp->handage != 0 /* can't bank a banked spell */
			&& tmp->spellnum != SP__STAB) {
			dellist[numdel] = tmp;
			delnum[numdel] = tmp->spellnum;
			numdel++;
		    }
		}
		delnum[numdel] = (-1);
		if (numdel==0)
		    res = NULL;
		else if (numdel==1)
		    res = dellist[0];
		else {
		    /* gack -- have to pick from the list. */
		    erase_queries(self);
		    add_query(self, wiznum, Qu_WhichToDelay, delnum);
		    Queries(self->numqueries, self->querylist);
		    res = dellist[self->querylist[0].answer];
		}

		if (res) {
		    struct castspell **ppt;
		    for (ppt = (&(self->castlist));
			 (*ppt) && (*ppt)!=res;
			 ppt = (&((*ppt)->next)));
		    if (!(*ppt)) {
			PrintMsg("ERROR: Unable to find Delayed spell.\n");
		    }
		    else {
			*ppt = res->next;
			res->next = NULL;
			sprintf(bigbuf, "You cast %s; the spell is caught in the web of your Delayed Effect. It rises to hover by your head.\n", spelllist[res->spellnum].name);
			sprintf(bigbuf2, "%s casts %s; the spell is caught in the web of %s Delayed Effect.\n", wiz->name, spelllist[res->spellnum].name, pro_his(wiz->gender));
			PrintMsg2(wiznum, bigbuf, bigbuf2);
			if (wiz->delay_bank != (-1)) {
			    sprintf(bigbuf, "The %s spell that you had caught previously is lost.\n", spelllist[wiz->delay_bank].name);
			    sprintf(bigbuf2, "The %s spell that %s had caught previously is lost.\n", spelllist[wiz->delay_bank].name, wiz->name);
			    PrintMsg2(wiznum, bigbuf, bigbuf2);
			    wiz->delay_bank = (-1);
			}
			wiz->delay_time = 0;
			wiz->delay_bank = res->spellnum;
			free((char *)res);
		    }
		}
	    }

	    /* PERMANENCY */
	    if (wiz->perm_time) {
		struct castspell *dellist[10]; /* that had better be enough */
		int delnum[11];

		int numdel = 0;
		for (tmp=self->castlist; tmp; tmp=tmp->next) {
		    if (tmp->caster == wiznum
			&& tmp->handage != 0 /* can't perm a banked spell */
			&& spelllist[tmp->spellnum].extendable) {
			dellist[numdel] = tmp;
			delnum[numdel] = tmp->spellnum;
			numdel++;
		    }
		}
		delnum[numdel] = (-1);
		if (numdel==0)
		    res = NULL;
		else if (numdel==1)
		    res = dellist[0];
		else {
		    /* gack -- have to pick from the list. */
		    erase_queries(self);
		    add_query(self, wiznum, Qu_WhichToPerm, delnum);
		    Queries(self->numqueries, self->querylist);
		    res = dellist[self->querylist[0].answer];
		}

		if (res) {
		    res->permanent = 1;
		    sprintf(bigbuf, "Your Permanency spell wraps itself around your %s spell.\n", spelllist[res->spellnum].name);
		    PrintMsg2(wiznum, bigbuf, NULL);
		    wiz->perm_time = 0;
		}
	    }
	}

    erase_queries(self);

    /* add target queries for spells, in order */
    for (tmp=self->castlist; tmp; tmp=tmp->next) {
	int orval;
	if (tmp->handage==MASK_LEFT)
	    orval = QuVal_Hand_Left;
	else if (tmp->handage==MASK_RIGHT)
	    orval = QuVal_Hand_Right;
	else if (tmp->handage==0)
	    orval = 0;
	else
	    orval = QuVal_Hand_Both;

	add_query(self, tmp->caster, spelltargetlist[tmp->spellnum], tmp->spellnum | orval);
    }
    Queries(self->numqueries, self->querylist); /* "who do you want to cast X on?" queries */
    for (tmp=self->castlist, jx=0; tmp; tmp=tmp->next, jx++) {
	if (spelltargetlist[tmp->spellnum]==Qu_NoQuery) {
	    tmp->targettype = (-1);
	}
	else {
	    int ival;
	    int tart = self->querylist[jx].answer & QuVal_Target_MASK;
	    if (tart==0) {
		tmp->targettype = 0;
	    }
	    else {
		kx = self->querylist[jx].answer & (~QuVal_Target_MASK);
		ival = IndexOfTarget((game *)self, tart, kx);
		tmp->target = ival & (~QuVal_Target_MASK);
		tmp->targettype = ival & QuVal_Target_MASK;
	    }
	}
    }
}

static void exec_unhealth(self, fred, cnum, wizflag) /* no deaders */
struct realgame *self;
union being *fred;
int cnum;
int wizflag;
{
    if (self->turntype==Turn_TIMESTOP && fred->both.timestop!=1)
	return; /* the illnesses are timestopped and have no effect. */

    if (fred->both.disease_time != (-1)) {
	fred->both.disease_time--;
	switch (fred->both.disease_time) {
	    case 5:
		/* message already printed */ 
		break;
	    case 4:
		sprintf(bigbuf, "%s starts to look hot and flushed.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "You begin to feel somewhat feverish.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 3:
		sprintf(bigbuf, "%s looks even more flushed.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "Your fever is growing worse.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 2:
		sprintf(bigbuf, "%s is flushed and sweating, and seems to be somewhat unsteady on %s feet.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "You are very hot and somewhat dizzy, and your bones are starting to ache.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 1:
		sprintf(bigbuf, "%s is both flushed and shivering violently.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "You are feverish and shivering at the same time, and you ache all over.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 0:
		sprintf(bigbuf, "%s crumples to the ground, agony on %s sweating features.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "Your knees give way and you fall to the ground. Your head pounds unmercifully as consciousness slips away....\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		fred->both.alive = 0;
		break;
	}
    }

    if (fred->both.poison_time != (-1)) {
	fred->both.poison_time--;
	switch (fred->both.poison_time) {
	    case 5:
		/* message already printed */ 
		break;
	    case 4:
		sprintf(bigbuf, "%s is looking pale.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "Your toes suddenly feel slightly numb.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 3:
		sprintf(bigbuf, "%s is looking very pale and weak.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "Your hands and feet are getting numb.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 2:
		sprintf(bigbuf, "%s is white as a sheet, and seems a little unsteady.\n", fred->both.name);
		if (wizflag)
		    PrintMsg2(cnum, "Your limbs are heavy, and your fingers are getting clumsy.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 1:
		sprintf(bigbuf, "%s is having difficulty staying on %s feet.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "You cannot feel your hands and feet, and your vision is becoming clouded.\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		break;
	    case 0:
		sprintf(bigbuf, "%s collapses in a heap, twitching slightly.\n", fred->both.name, pro_his(fred->both.gender));
		if (wizflag)
		    PrintMsg2(cnum, "You stumble and fall to your knees, vision dimming. Numbness creeps through your bones, and you cannot tell if you are still breathing....\n", bigbuf);
		else
		    PrintMsg(bigbuf);
		fred->both.alive = 0;
		break;
	}
    }
}

/* let critters die, let poison/disease advance, etc */
static void pass_time(self)
struct realgame *self;
{
    int ix;

    for (ix=0; ix<self->numplayers; ix++) {
	if (self->wiz[ix]->alive && self->wiz[ix]->hitpoints <= 0) {
	    sprintf(bigbuf, "%s has died!\n", self->wiz[ix]->name);
	    PrintMsg2(ix, "You have died!\n", bigbuf);
	    self->wiz[ix]->alive = 0;
	    clear_out_spells(self->wiz[ix], 1);
	    write_gestures(self, self->wiz[ix], Gesture_NOTHING, Gesture_NOTHING);
	    /* ### other stuff? */
	}
	if (self->wiz[ix]->alive) {
	    exec_unhealth(self, self->wiz[ix], ix, 1);
	}
	if (self->wiz[ix]->alive && self->turnactive[ix]) {
	    if (self->wiz[ix]->delay_time) {
		self->wiz[ix]->delay_time--;
		if (self->wiz[ix]->delay_time==0) {
		    sprintf(bigbuf, "The Delayed Effect spell hovering around %s dies away.\n", self->wiz[ix]->name);
		    PrintMsg2(ix, "The Delayed Effect spell hovering around you dies away.\n", bigbuf);   
		}
	    }
	}
	if (self->wiz[ix]->alive && self->turnactive[ix]) {
	    if (self->wiz[ix]->perm_time) {
		self->wiz[ix]->perm_time--;
		if (self->wiz[ix]->perm_time==0) {
		    sprintf(bigbuf, "The Permanency spell hovering around %s dies away.\n", self->wiz[ix]->name);
		    PrintMsg2(ix, "The Permanency spell hovering around you dies away.\n", bigbuf);   
		}
	    }
	}
    }
    for (ix=0; ix<self->numcres; ix++) {
	if (self->cre[ix].alive && self->cre[ix].hitpoints <= 0) {
	    sprintf(bigbuf, "%s has died.\n", self->cre[ix].name);
	    PrintMsg(bigbuf);
	    self->cre[ix].alive = 0;
	    clear_out_spells(&(self->cre[ix]), 0);
	    /* ### other stuff? */
	}
	if (self->cre[ix].alive)
	    exec_unhealth(self, &self->cre[ix], ix, 0);
    }
}

static int analyze_endgame(self, numlive, numdead, numsurr)
struct realgame *self;
int numlive, numdead, numsurr; /* these add up to self->numplayers, and numlive <= 1 */
{
    int ix, jx;

    if (numlive==1) {

	for (ix=0; ix<self->numplayers; ix++)
	    if (self->wiz[ix]->alive && !self->wiz[ix]->surrendered)
		break;

	if (numsurr==0) {
	    sprintf(bigbuf, "%s is the sole survivor!\n", self->wiz[ix]->name);
	    PrintMsg(bigbuf);
	}
	else if (numsurr==1) {
	    for (jx=0; jx<self->numplayers; jx++)
		if (self->wiz[jx]->alive && self->wiz[jx]->surrendered)
		    break;
	    sprintf(bigbuf, "%s has surrendered to %s!\n", self->wiz[jx]->name, self->wiz[ix]->name);
	    PrintMsg(bigbuf);
	}
	else {
	    sprintf(bigbuf, "All of %s's opponents have surrendered!\n", self->wiz[ix]->name);
	    PrintMsg(bigbuf);
	}

	return ix;
    }
    else {
	/* nobody left alive (and unsurrendered) */
	if (numsurr==0) {
	    PrintMsg("Everyone is dead!\n");
	    return MAXPLAYERS;
	}

	if (numsurr==1) {
	    for (jx=0; jx<self->numplayers; jx++)
		if (self->wiz[jx]->alive && self->wiz[jx]->surrendered)
		    break;
	    sprintf(bigbuf, "%s finds that there is nobody left to surrender to!\n", self->wiz[jx]->name);
	    PrintMsg(bigbuf);
	    return jx;
	}

	if (numdead)
	    PrintMsg("The survivors have all surrendered to each other!\n");
	else
	    PrintMsg("Everyone has surrendered to each other!\n");
	return MAXPLAYERS;
    }
}
