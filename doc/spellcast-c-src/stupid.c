#include <stdio.h>
#include <string.h>

#include "handwave.h"
#include "spelllist.h"

struct maingame {
    int ignorestuff;
};

static void PrintMsg(msg, gameval, rock)
char *msg;
game *gameval;
struct maingame *rock;
{
    if (msg) {
	printf("*: ");
	fputs(msg, stdout);
    }
}

static void PrintMsg2(person1, msg1, msgelse, gameval, rock)
int person1;
char *msg1, *msgelse;
game *gameval;
struct maingame *rock;
{
    if (msg1) {
	printf("%d: ", person1);
	fputs(msg1, stdout);
    }
    if (msgelse) {
	printf("~%d: ", person1);
	fputs(msgelse, stdout);
    }
}

static void PrintMsg3(person1, person2, msg1, msg2, msgelse, gameval, rock)
int person1, person2;
char *msg1, *msg2, *msgelse;
game *gameval;
struct maingame *rock;
{
    if (msg1) {
	printf("%d: ", person1);
	fputs(msg1, stdout);
    }
    if (msg2) {
	printf("%d: ", person2);
	fputs(msg2, stdout);
    }
    if (msgelse) {
	printf("~%d/~%d: ", person1, person2);
	fputs(msgelse, stdout);
    }
}

static void Queries(numqueries, qlist, gameval, rock)
int numqueries;
struct query qlist[];
game *gameval;
struct maingame *rock;
{
    int ix, jx, attnum, spellnum;
    int res;
    char inbuf[256];
    char outbuf[512];
    static int targetlist[256];
    static char *targetnamelist[256];
    int numtargs, targnum;

    int *hlist;
    char *ques, *handage;

    for (ix=0; ix<numqueries; ix++) {
	switch (qlist[ix].qtype) {
	    case Qu_NoQuery:
		/*printf("ERROR: got NoQuery\n");*/
		break;
	    case Qu_SetOffDelay:
		printf(
"%d: Do you want to release the %s from the Delayed Effect?\n (0:no, 1:yes): ",
				qlist[ix].player,
				spelllist[(int)qlist[ix].rock].name);
		while (1) {
		    fgets(inbuf, 256, stdin);
		    res = atol(inbuf);
		    if (res!=0 && res!=1) {
			printf(" [Please enter 0 or 1]: ");
		    }
		    else
			break;
		}
		qlist[ix].answer = res;
		break;
	    case Qu_LeftHand:
	    case Qu_RightHand:
		if (qlist[ix].qtype==Qu_LeftHand)
		    ques = "What spell do you want to cast with your left hand?";
		else
		    ques = "What spell do you want to cast with your right hand?";
		hlist = (int *)qlist[ix].rock;
		printf("%d: %s\n (", qlist[ix].player, ques);
		for (jx=0; jx<hlist[0]; jx++) {
		    spellnum = hlist[jx+1];
		    printf("%d: %s%s%s", jx, spelllist[spellnum & (~QuVal_Hand_Both)].name,
			   ((spellnum & QuVal_Hand_Both) ? " [both hands]" : ""),
			   (jx+1==hlist[0]) ? "): " : ", ");
		}
		while (1) {
		    fgets(inbuf, 256, stdin);
		    res = atol(inbuf);
		    if (res<0 || res>=hlist[0]) {
			printf(" [Please enter a number from 0 to %d]: ", hlist[0]-1);
		    }
		    else
			break;
		}
		qlist[ix].answer = res;
		break;
	    case Qu_ElementalType:
		printf(
"%d: Which type of elemental do you want to summon?\n (0:fire, 1:ice): ",
				qlist[ix].player);
		while (1) {
		    fgets(inbuf, 256, stdin);
		    res = atol(inbuf);
		    if (res!=0 && res!=1) {
			printf(" [Please enter 0 or 1]: ");
		    }
		    else
			break;
		}
		qlist[ix].answer = res;
		break;
	    case Qu_ParalysisHand:
		printf(
"%d: Which of %s's hands do you want to paralyze?\n (0:left, 1:right): ",
				qlist[ix].player,
				NameOfBeing(gameval, QuVal_Target_Wizard,
				qlist[ix].rock));
		while (1) {
		    fgets(inbuf, 256, stdin);
		    res = atol(inbuf);
		    if (res!=0 && res!=1) {
			printf(" [Please enter 0 or 1]: ");
		    }
		    else
			break;
		}
		qlist[ix].answer = res;
		break;
	    case Qu_CharmHand:
		printf(
"%d: Which of %s's hands do you want to charm?\n (0:left, 1:right): ",
				qlist[ix].player,
				NameOfBeing(gameval, QuVal_Target_Wizard,
				qlist[ix].rock));
		while (1) {
		    fgets(inbuf, 256, stdin);
		    res = atol(inbuf);
		    if (res!=0 && res!=1) {
			printf(" [Please enter 0 or 1]: ");
		    }
		    else
			break;
		}
		qlist[ix].answer = res;
		break;
	    case Qu_CharmGesture:
		jx = (int)(qlist[ix].rock);
		if (jx >= 128) {
		    spellnum = jx - 128;
		    jx = 1;
		}
		else {
		    spellnum = jx;
		    jx = 0;
		}
		printf(
"%d: What gesture do you want %s's %s hand to make?\n (enter a letter): ",
				qlist[ix].player,
				NameOfBeing(gameval, QuVal_Target_Wizard,
				spellnum), (jx?"right":"left"));
		while (1) {
		    fgets(inbuf, 256, stdin);
		    if (inbuf[0]=='\0' || inbuf[0]=='\n') {
			printf(" [Please enter a letter]: ");
		    }
		    else {
			res = cheap_translate(inbuf[0]);
			break;
		    }
		}
		qlist[ix].answer = res;
		break;
	    case Qu_MonsterTarget:
		jx = (int)(qlist[ix].rock);
		attnum = jx / 256;
		spellnum = jx % 256;
		switch (attnum) {
		    case 1:
			sprintf(outbuf, "Whom do you want %s to attack?",
					NameOfBeing(gameval,
					QuVal_Target_Creature, spellnum));
			break;
		    case 2:
			sprintf(outbuf, "Whom do you want %s's first attack to be at?",
					NameOfBeing(gameval,
					QuVal_Target_Creature, spellnum));
			break;
		    case 3:
			sprintf(outbuf, "Whom do you want %s's second attack to be at?",
					NameOfBeing(gameval,
					QuVal_Target_Creature, spellnum));
			break;
		    default:
			sprintf(outbuf, "ERROR: Query about %d (%d)", spellnum, attnum);
			break;
		}
		printf("%d: %s\n", qlist[ix].player, outbuf);
		targnum = 0;
		targetlist[targnum] = 0;
		targetnamelist[targnum] = "nobody";
		targnum++;
		numtargs = NumberOfTargets(gameval, QuVal_Target_Wizard);
		for (jx=0; jx<numtargs; jx++) {
		    targetlist[targnum] = jx | QuVal_Target_Wizard;
		    targetnamelist[targnum] = NameOfTarget(gameval,
				QuVal_Target_Wizard, jx);
		    targnum++;
		}
		numtargs = NumberOfTargets(gameval, QuVal_Target_Creature);
		for (jx=0; jx<numtargs; jx++) {
		    targetlist[targnum] = jx | QuVal_Target_Creature;
		    targetnamelist[targnum] = NameOfTarget(gameval,
				QuVal_Target_Creature, jx);
		    targnum++;
		}
		numtargs = targnum;
		if (numtargs==0)
		    printf("ERROR: No targets available!\n");
		else {
		    printf(" [");
		    for (jx=0; jx<numtargs; jx++) {
			printf("%d: %s%s", jx, targetnamelist[jx],
					((jx<numtargs-1)?", ":""));
		    }
		    printf("]: ");
		    while (1) {
			fgets(inbuf, 256, stdin);
			res = atol(inbuf);
			if (res<0 || res>=numtargs) {
			    printf(" [Please enter a number from 0 to %d]: ",
					numtargs-1);
			}
			else
			    break;
		    }
		    qlist[ix].answer = targetlist[res];
		}
		break;
	    case Qu_WhichToDelay:
	    case Qu_WhichToPerm:
		if (qlist[ix].qtype == Qu_WhichToDelay)
		    printf("%d: Which spell do you want to delay?\n",
				qlist[ix].player);
		else
		    printf("%d: Which spell do you want to make permanent?\n",
				qlist[ix].player);
		
		hlist = (int *)qlist[ix].rock;
		printf(" [");
		for (jx=0; hlist[jx]!=(-1); jx++) {
		    printf("%s%d: %s", ((jx)?", ":""), jx, spelllist[hlist[jx]].name);
		}
		numtargs = jx;
		printf("]: ");
		while (1) {
		    fgets(inbuf, 256, stdin);
		    res = atol(inbuf);
		    if (res<0 || res>=numtargs) {
			printf(" [Please enter a number from 0 to %d]: ", numtargs-1);
		    }
		    else
			break;
		}
		qlist[ix].answer = res;
		break;
	    case Qu_TargetBeing:
	    case Qu_TargetBeingNone:
	    case Qu_TargetWizard:
	    case Qu_TargetWizardNone:
	    case Qu_TargetRaiseDead:
		jx = (int)(qlist[ix].rock);
		spellnum = jx & (~QuVal_Hand_MASK);
		if (jx & QuVal_Hand_Left)
		    handage = "with your left hand";
		else if (jx & QuVal_Hand_Right)
		    handage = "with your right hand";
		else if (jx & QuVal_Hand_Both)
		    handage = "with both hands";
		else
		    handage = "from the Delayed Effect";
		if (spellnum==SP__STAB)
		    sprintf(outbuf, "Who do you want to stab at (%s)?", handage);
		else
		    sprintf(outbuf, "Who do you want to cast %s at (%s)?",
				spelllist[spellnum].name, handage);
		printf("%d: %s\n", qlist[ix].player, outbuf);
		targnum = 0;
		if (qlist[ix].qtype==Qu_TargetBeingNone || 
				qlist[ix].qtype==Qu_TargetWizardNone) {
		    targetlist[targnum] = 0;
		    targetnamelist[targnum] = "nobody";
		    targnum++;
		}
		numtargs = NumberOfTargets(gameval, QuVal_Target_Wizard);
		for (jx=0; jx<numtargs; jx++) {
		    targetlist[targnum] = jx | QuVal_Target_Wizard;
		    targetnamelist[targnum] = NameOfTarget(gameval,
				QuVal_Target_Wizard, jx);
		    targnum++;
		}
		if (qlist[ix].qtype==Qu_TargetBeing ||
				qlist[ix].qtype==Qu_TargetBeingNone || 
				qlist[ix].qtype==Qu_TargetRaiseDead) {
		    numtargs = NumberOfTargets(gameval, QuVal_Target_Creature);
		    for (jx=0; jx<numtargs; jx++) {
			targetlist[targnum] = jx | QuVal_Target_Creature;
			targetnamelist[targnum] = NameOfTarget(gameval,
					QuVal_Target_Creature, jx);
			targnum++;
		    }
		}
		if (qlist[ix].qtype==Qu_TargetRaiseDead) {
		    numtargs = NumberOfTargets(gameval, QuVal_Target_Corpse);
		    for (jx=0; jx<numtargs; jx++) {
			targetlist[targnum] = jx | QuVal_Target_Corpse;
			targetnamelist[targnum] = NameOfTarget(gameval,
					QuVal_Target_Corpse, jx);
			targnum++;
		    }
		}
		numtargs = targnum;
		if (numtargs==0)
		    printf("ERROR: No targets available!\n");
		else {
		    printf(" [");
		    for (jx=0; jx<numtargs; jx++) {
			printf("%d: %s%s", jx, targetnamelist[jx], ((jx<numtargs-1)?",
					":""));
		    }
		    printf("]: ");
		    while (1) {
			fgets(inbuf, 256, stdin);
			res = atol(inbuf);
			if (res<0 || res>=numtargs) {
			    printf(" [Please enter a number from 0 to %d]: ",
					numtargs-1);
			}
			else
			    break;
		    }
		    qlist[ix].answer = targetlist[res];
		}
		break;

	    default:
		printf("ERROR: Unknown query type %d!\n", qlist[ix].qtype);
		break;
	}
    }
}

int cheap_translate(ch)
char ch;
{
    switch (ch) {
	case 'p':
	case 'P':
	    return Gesture_PALM;
	case 'f':
	case 'F':
	    return Gesture_FINGERS;
	case 'd':
	case 'D':
	    return Gesture_DIGIT;
	case 's':
	case 'S':
	    return Gesture_SNAP;
	case 'c':
	case 'C':
	    return Gesture_CLAPHALF;
	case 'w':
	case 'W':
	    return Gesture_WAVE;
	case 'k':
	case 'K':
	    return Gesture_KNIFE;
	case ' ':
	case 'n':
	case 'N':
	default:
	    return Gesture_NOTHING;
    }
}

void show_beings(gameval)
game *gameval;
{
    int ix, max, hp;

    printf("\n");
    max = NumberOfBeings(gameval, QuVal_Target_Wizard);
    for (ix=0; ix<max; ix++) {
	hp = HitPointsOfBeing(gameval, QuVal_Target_Wizard, ix);
	if (hp>=0)
	    printf("%s (%d); ", NameOfBeing(gameval, QuVal_Target_Wizard, ix), hp);
    }
    max = NumberOfBeings(gameval, QuVal_Target_Creature);
    for (ix=0; ix<max; ix++) {
	hp = HitPointsOfBeing(gameval, QuVal_Target_Creature, ix);
	if (hp>=0)
	    printf("%s (%d); ", NameOfBeing(gameval, QuVal_Target_Creature, ix), hp);
    }
    printf("\n");
}

main(argc, argv)
int argc;
char *argv[];
{
    struct interface procs;
    struct maingame ignorerock;
    game *gameval;
    static char *namelist[MAXPLAYERS] = {"Arnold", "Barbara", "Crash"};
    static int genderlist[MAXPLAYERS] = {Gender_MALE, Gender_FEMALE, Gender_NEUTER};
    static int movelist[MAXPLAYERS*2];
    static char inbuf[256];
    int numplayers, whowon;
    int ix, val;

    procs.proc_PrintMsg = PrintMsg;
    procs.proc_PrintMsg2 = PrintMsg2;
    procs.proc_PrintMsg3 = PrintMsg3;
    procs.proc_Queries = Queries;

    numplayers = 2;
    if (argc==2 && !strcmp(argv[1], "-3")) {
	numplayers = 3;
    }
    else if (argc>=2) {
	numplayers = 0;
	for (ix=1; ix<argc; ix++) {
	    char *tmp;
	    namelist[numplayers] = argv[ix];
	    tmp = index(namelist[numplayers], ':');
	    if (!tmp)
		genderlist[numplayers] = Gender_MALE;
	    else {
		switch (*(tmp+1)) {
		    case 'f':
		    case 'F':
			genderlist[numplayers] = Gender_FEMALE;
			break;
		    case 'n':
		    case 'N':
			genderlist[numplayers] = Gender_NEUTER;
			break;
		    case 'x':
		    case 'X':
			genderlist[numplayers] = Gender_NONE;
			break;
		    case 'm':
		    case 'M':
		    default:
			genderlist[numplayers] = Gender_MALE;
			break;
		}
		*tmp = '\0';
	    }
	    numplayers++;
	}
    }

    if (numplayers < 2) {
	printf("You have listed only %d players!\n", numplayers);
	exit(12);
    }
    if (numplayers > MAXPLAYERS) {
	printf("There is a maximum of %d players!\n", MAXPLAYERS);
	exit(12);
    }

    gameval = BeginGame(numplayers, namelist, genderlist, &procs, &ignorerock);

    do {
	show_beings(gameval);
	val = 0;
	switch (TurnType(gameval)) {
	    case Turn_HASTE:
		printf("(hasted) ");
		break;
	    case Turn_TIMESTOP:
		printf("(timestopped) ");
		break;
	    case Turn_NORMAL:
	    default:
		break;
	}
	for (ix=0; ix<MAXPLAYERS; ix++)
	    if (TurnPlayerActive(gameval, ix)) {
		val++;
		printf("%d", ix);
	    }
	printf(": Enter moves: ");
	bzero(inbuf, numplayers*2);
	fgets(inbuf, 256, stdin);
	for (ix=0; ix<numplayers*2; ix++) {
	    movelist[ix] = cheap_translate(inbuf[ix]);
	}
	whowon = RunTurn(gameval, movelist);
    } while (whowon < 0);

    if (whowon==MAXPLAYERS)
	printf("+++ the game is a draw +++\n");
    else
	printf("+++ %s has won +++\n", namelist[whowon]);

    FreeGame(gameval);
}

