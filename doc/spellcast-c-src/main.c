/* This program is copyright 1993 by Andrew Plotkin.
 The source code may be freely copied, distributed,
 and modified, as long as this copyright notice is
 retained.
 */

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "handwave.h"
#include "spelllist.h"
#include "patchlevel.h"

#include "xspell.h"

int win_wid, win_hgt;

struct player *players;
int numplayers;
int turnstate;

game *gameval = NULL;

static int defaultnum = 0;
static char *defaultnamelist[10] = {"White", "Black", "Red", "Green", "Blue",
		"Yellow", "Orange", "Purple", "Grey", "Charteuse"};
static char *namelist[MAXPLAYERS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static char *displist[MAXPLAYERS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static int genderlist[MAXPLAYERS] = {Gender_MALE, Gender_MALE, Gender_MALE,
		Gender_MALE, Gender_MALE, Gender_MALE, Gender_MALE, Gender_MALE};

static struct query savequery = {
    0,
    Qu_SaveTranscript,
    NULL,
    0
};

static void PrintMsg(), PrintMsg2(), PrintMsg3();
extern void redraw();
extern void XQueries();

main(argc, argv)
int argc;
char *argv[];
{
    struct interface procs;
    struct maingame ignorerock;
    static char inbuf[256];
    int whowon;
    int ix, val;
    struct player *py;
    int movelist[MAXPLAYERS*2];

    procs.proc_PrintMsg = PrintMsg;
    procs.proc_PrintMsg2 = PrintMsg2;
    procs.proc_PrintMsg3 = PrintMsg3;
    procs.proc_Queries = XQueries;

    if (argc==1) {
	printf("usage: spellcast RemoteDisplay [ RemoteDisplay2 ... ]\n");
	exit(12);
    }

    numplayers = 1;
    displist[0] = NULL;
    for (ix=1; ix<argc; ix++) {
	if (strlen(argv[ix])!=0) {
	    if (index(argv[ix], ':'))
		displist[numplayers] = argv[ix];
	    else {
		displist[numplayers] = (char *)malloc(strlen(argv[ix])+6);
		strcpy(displist[numplayers], argv[ix]);
		strcat(displist[numplayers], ":0.0");
	    }
	}
	else
	    displist[numplayers] = NULL;
	/* allow an empty argument to signify NULL display */
	numplayers++;
    }

    if (numplayers < 2) {
	printf("You have listed only %d players!\n", numplayers);
	exit(12);
    }
    if (numplayers > MAXPLAYERS) {
	printf("There is a maximum of %d players!\n", MAXPLAYERS);
	exit(12);
    }

    players = (struct player *)malloc(sizeof(struct player) * numplayers);

    win_wid = 832;
    if (numplayers > 3)
	win_wid += (numplayers-3) * (2*GEST_SIZE + 20);
    win_hgt = 750;
    turnstate = State_Init;

    for (ix=0; ix<numplayers; ix++) {
	val = win_init(ix);
	if (!val) {
	    fprintf(stderr, "spellcast: exiting.\n");
	    exit(-1);
	}
    }

    gameval = BeginGame(numplayers, namelist, genderlist, &procs, &ignorerock);

    do {
	val = TurnType(gameval);
	turnstate = State_Top;
	for (ix=0; ix<numplayers; ix++) {
	    py = &(players[ix]);
	    py->turn_active = TurnPlayerActive(gameval, ix); 
	    py->turn_done = !py->turn_active;
	}
	for (ix=0; ix<numplayers; ix++) {
	    py = &(players[ix]);
	    py->gesture_chosen[0] = Gesture_NOTHING;
	    py->gesture_chosen[1] = Gesture_NOTHING;
	    clear_answers(py);
	    if (py->turn_active)
		switch (val) {
		    case Turn_HASTE:
			add_answer(py, "Enter your gestures for your Hastened move.",
					"", 1, 0, NULL, 0);
			break;
		    case Turn_TIMESTOP:
			add_answer(py, "Enter your gestures for your Time-Stopped move.",
					"", 1, 0, NULL, 0);
			break;
		    default:
			add_answer(py,
"Enter your gestures for this move, at the bottom of your gesture list.",
					"", 1, 0, NULL, 0);
			break;
		}
	    else
		switch (val) {
		    case Turn_HASTE:
			add_answer(py, "Other people are Hastened this turn.",
					"", 1, 0, NULL, 0);
			break;
		    case Turn_TIMESTOP:
			add_answer(py, "You are frozen in time.", "", 1, 0, NULL, 0);
			break;
		    default:
			add_answer(py, "You are out of the game.", "", 1, 0, NULL, 0);
			break;
		}
	    redraw_column(py);
	    draw_button(py, 0, 1); /* redraw turn-end button */
	    redraw_queries_only(py, 0);
	    /*update_statlist(py);*/
	}
	mainloop();

	for (ix=0; ix<numplayers; ix++) {
	    struct player *py = &(players[ix]);
	    movelist[ix*2+0] = py->gesture_chosen[0];
	    movelist[ix*2+1] = py->gesture_chosen[1];
	}
	whowon = RunTurn(gameval, movelist);
    } while (whowon < 0);

    if (whowon==MAXPLAYERS)
	sprintf(inbuf, "\n+++ The game is a draw +++\n");
    else
	sprintf(inbuf, "\n+++ %s has won +++\n", NameOfBeing(gameval,
			QuVal_Target_Wizard, whowon));
    PrintMsg(inbuf, gameval, &ignorerock);
    LogInTranscript(gameval, inbuf);
    LogInTranscript(gameval, "\n");

    turnstate = State_End;
    for (ix=0; ix<numplayers; ix++) {
	py = &(players[ix]);
	py->turn_active = 1;
	py->turn_done = 0;
    }
    for (ix=0; ix<numplayers; ix++) {
	py = &(players[ix]);
	clear_answers(py);
	if (ix==0)
	    add_answer(py, "Do you want to save a transcript of this game?",
			"No", 1, 0, &savequery, 0);
	redraw_column(py);
	draw_button(py, 0, 1); /* redraw turn-end button */
	redraw_queries_only(py, 0);
	/*update_statlist(py);*/
    }
    mainloop();

    savequery.answer = players[0].answers[0].answer;
    if (savequery.answer) {
	char fname[512];
	FILE *fl;

	sprintf(fname, "%s/spellcast-%d", TRANSCRIPTDIR, getpid());
	fl = fopen(fname, "w");
	if (!fl) {
	    printf("spellcast: unable to open %s for writing\n", fname);
	    perror("spellcast: error");
	}
	else {
	    WriteTranscript((game *)gameval, fl);
	    fclose(fl);
	    printf("Saved transcript in %s.\n", fname);
	}
    }
    FreeGame(gameval);

}

int win_init(pnum)
int pnum;
{
    XSetWindowAttributes attr;
    XGCValues gcvalues;
    char winname[256];
    char *fontname;
    char *name, *tmp;
    int ix, val;

    struct player *py = &(players[pnum]);

    py->dpy = XOpenDisplay(displist[pnum]);
    if (!py->dpy) {
	fprintf(stderr, "spellcast: could not open display for player %d.\n", pnum);
	return 0;
    }

    py->scn = DefaultScreen(py->dpy);
    py->win = XCreateSimpleWindow(py->dpy, DefaultRootWindow(py->dpy), 100, 100,
		win_wid, win_hgt, 1, BlackPixel(py->dpy, py->scn),
		WhitePixel(py->dpy, py->scn));
    py->backpm = XCreatePixmap(py->dpy, py->win, win_wid, win_hgt,
		DefaultDepth(py->dpy, py->scn));
    py->backstore = 0;

    { /* make some window managers happy */
	XWMHints wmhints;
	wmhints.flags = InputHint;
	wmhints.input = True;
	XSetWMHints(py->dpy, py->win, &wmhints);
    }

    attr.event_mask = EVENTMASK;
    XChangeWindowAttributes(py->dpy, py->win, CWEventMask, &attr);

    tmp = XGetDefault(py->dpy, "spellcast", "name");
    if (tmp) {
	name = (char *)malloc(strlen(tmp)+1);
	strcpy(name, tmp);
	tmp = index(name, ':');
	if (!tmp)
	    genderlist[pnum] = Gender_MALE;
	else {
	    switch (*(tmp+1)) {
		case 'f':
		case 'F':
		    genderlist[pnum] = Gender_FEMALE;
		    break;
		case 'n':
		case 'N':
		    genderlist[pnum] = Gender_NEUTER;
		    break;
		case 'x':
		case 'X':
		    genderlist[pnum] = Gender_NONE;
		    break;
		case 'm':
		case 'M':
		default:
		    genderlist[pnum] = Gender_MALE;
		    break;
	    }
	    *tmp = '\0';
	}
	tmp = name+strlen(name)-1;
	while (tmp>name && (*tmp==' ' || *tmp=='\t'))
	    tmp--;
	*(tmp+1) = '\0';
	namelist[pnum] = name;
    }
    else {
	namelist[pnum] = defaultnamelist[defaultnum];
	defaultnum++;
    }

    do {
	val = 0;
	if (strlen(namelist[pnum])<1)
	    val = 1;
	if (!strcmp(namelist[pnum], "nobody"))
	    val = 1;
	if (!strcmp(namelist[pnum], "Nobody"))
	    val = 1;
	for (ix=0; ix<pnum; ix++)
	    if (!strcmp(namelist[pnum], namelist[ix]))
		val = 1;

	if (val) {
	    tmp = namelist[pnum];
	    namelist[pnum] = defaultnamelist[defaultnum];
	    defaultnum++;

	    fprintf(stderr,
"spellcast: the name '%s' for player %d is taken. Switching to '%s'...\n",
			tmp, pnum, namelist[pnum]);
	}
    } while (val);

    sprintf(winname, "Spellcast: %s", namelist[pnum]);
    XStoreName(py->dpy, py->win, winname);

    XMapWindow(py->dpy, py->win);

    fontname = XGetDefault(py->dpy, "spellcast", "font");
    if (!fontname)
	fontname = BODYFONT;
    py->font = XLoadQueryFont(py->dpy, fontname);

    if (!py->font) {
	fprintf(stderr,
"spellcast: could not find font %s for player %d. Switching to %s...\n",
			fontname, pnum, BODYFONT_ALT);
	py->font = XLoadQueryFont(py->dpy, BODYFONT_ALT);
	if (!py->font) {
	    fprintf(stderr,
"spellcast: could not find font %s for player %d. Switching to %s...\n",
			BODYFONT_ALT, pnum, BODYFONT_ALT2);
	    py->font = XLoadQueryFont(py->dpy, BODYFONT_ALT2);
	    if (!py->font) {
		fprintf(stderr, "spellcast: could not find font %s for player %d.\n",
				BODYFONT_ALT2, pnum);

		return 0;
	    }

	}
    }

    gcvalues.font = py->font->fid;
    gcvalues.foreground = BlackPixel(py->dpy, py->scn);
    gcvalues.background = WhitePixel(py->dpy, py->scn);
    py->blackgc = XCreateGC(py->dpy, py->win, GCForeground|GCBackground|GCFont,
		&gcvalues);
    XSetGraphicsExposures(py->dpy, py->blackgc, 0);

    gcvalues.foreground = WhitePixel(py->dpy, py->scn);
    gcvalues.background = BlackPixel(py->dpy, py->scn);
    py->whitegc = XCreateGC(py->dpy, py->win, GCForeground|GCBackground|GCFont,
		&gcvalues);
    XSetGraphicsExposures(py->dpy, py->whitegc, 0);

    init_bitmaps(py);
    init_xtext(pnum);
    init_query(py);
    init_stats(py);
    init_talk(py);
    py->button_lit = (-1);
    redraw(py);

    return 1;
}

#define POPUP_ELBOW (8)

void adjust_rect(mrec)
struct rect *mrec;
{
    if (mrec->x+mrec->w >= win_wid-POPUP_ELBOW)
	mrec->x = win_wid-POPUP_ELBOW - mrec->w;
    if (mrec->y+mrec->h >= win_hgt-POPUP_ELBOW)
	mrec->y = win_hgt-POPUP_ELBOW - mrec->h;
    if (mrec->x < POPUP_ELBOW)
	mrec->x = POPUP_ELBOW;
    if (mrec->y < POPUP_ELBOW)
	mrec->y = POPUP_ELBOW;
}

void backing_store(py, mrec)
struct player *py;
struct rect *mrec;
{
    py->backrec = (*mrec);
    if (py->backstore) {
	printf("ERROR: backing store already on\n");
    }
    py->backstore = 1;
    py->gotexpose = 0;
    XCopyArea(py->dpy, py->win, py->backpm, py->blackgc, mrec->x, mrec->y,
		mrec->w, mrec->h, 0, 0);
}

void backing_restore(py)
struct player *py;
{
    if (!py->backstore) {
	printf("ERROR: backing store already off\n");
    }
    py->backstore = 0;
    if (py->gotexpose) {
	XClearArea(py->dpy, py->win, py->backrec.x, py->backrec.y,
			py->backrec.w, py->backrec.h, 0);
	redraw(py);
    }
    else {
	XCopyArea(py->dpy, py->backpm, py->win, py->blackgc, 0, 0,
			py->backrec.w, py->backrec.h, py->backrec.x, py->backrec.y);
    }
}

static void PrintMsg(msg, gameval, rock)
char *msg;
game *gameval;
struct maingame *rock;
{
    int ix;
    if (msg) {
	for (ix=0; ix<numplayers; ix++)
	    dump_text(ix, msg);
    }
}

static void PrintMsg2(person1, msg1, msgelse, gameval, rock)
int person1;
char *msg1, *msgelse;
game *gameval;
struct maingame *rock;
{
    int ix;

    for (ix=0; ix<numplayers; ix++) {
	if (ix==person1) {
	    if (msg1)
		dump_text(ix, msg1);
	}
	else {
	    if (msgelse)
		dump_text(ix, msgelse);
	}
    }
}

static void PrintMsg3(person1, person2, msg1, msg2, msgelse, gameval, rock)
int person1, person2;
char *msg1, *msg2, *msgelse;
game *gameval;
struct maingame *rock;
{
    int ix;

    for (ix=0; ix<numplayers; ix++) {
	if (ix==person1) {
	    if (msg1)
		dump_text(ix, msg1);
	}
	else if (ix==person2) {
	    if (msg2)
		dump_text(ix, msg2);
	}
	else {
	    if (msgelse)
		dump_text(ix, msgelse);
	}
    }
}

