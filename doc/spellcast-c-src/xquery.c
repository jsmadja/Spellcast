#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "handwave.h"
#include "spelllist.h"
#include "xspell.h"

#define QUERY_X  (32)
#define QUERY_Y  (620)
#define QUERY_W  (736)
#define QUERY_H  (100)
#define QUERY_AW  (450)

#define QUERY_LEADING (8)
#define QMENU_LEADING (4)
#define QMENU_BORDER (4)

#define GEST_SPACE (50)

typedef struct _tempstring {
    struct _tempstring *next;
    char str[1];
} tempstring;

static char *TLElementalType_Names[] = {"Fire", "Ice"};
static char *TLHand_Names[] = {"Left", "Right"};
static char *TLNoYes_Names[] = {"No", "Yes"};
static int TLZeroOne_Num[] = {0, 1};
static struct target_list TLBeing = {0, NULL, NULL, 0};
static struct target_list TLBeingNone = {0, NULL, NULL, 0};
static struct target_list TLWizard = {0, NULL, NULL, 0};
static struct target_list TLWizardNone = {0, NULL, NULL, 0};
static struct target_list TLRaiseDead = {0, NULL, NULL, 0};
static struct target_list TLElementalType = {2, TLElementalType_Names, TLZeroOne_Num, 0};
static struct target_list TLPickHand = {2, TLHand_Names, TLZeroOne_Num, 0};
static struct target_list TLNoYes = {2, TLNoYes_Names, TLZeroOne_Num, 0};

static tempstring *tempstringlist = NULL;

void redraw_queries_only();

void init_query(py)
struct player *py;
{
    py->answers_size = 8;
    py->answers = (struct answer *)malloc(sizeof(struct answer) * py->answers_size);
    py->siquery.nlines = 0;
    py->query_hgt = py->lineheight + QUERY_LEADING;
    py->siquery.visible = QUERY_H / py->query_hgt;

    py->TLLeftHand.size = 0;
    py->TLLeftHand.num = 0;
    py->TLRightHand.size = 0;
    py->TLRightHand.num = 0;

    py->siquery.py = py;
    py->siquery.top = 0;
    py->siquery.lineheight = py->query_hgt;
    py->siquery.x = QUERY_X;
    py->siquery.y = QUERY_Y;
    py->siquery.h = QUERY_H;
    py->siquery.redraw = redraw_queries_only;
}

void flush_tempstrings()
{
    tempstring *pt;

    while (tempstringlist) {
	pt = tempstringlist->next;
	free(tempstringlist);
	tempstringlist = pt;
    }
}

char *tempstring_malloc(len)
int len;
{
    tempstring *pt;

    pt = (tempstring *)malloc(sizeof(tempstring) + len + 1);
    pt->next = tempstringlist;
    tempstringlist = pt;
    return (pt->str);
}

void clear_answers(py)
struct player *py;
{
    py->siquery.nlines = 0;
    py->siquery.top = 0;
}

void add_answer(py, qu_str, ans_str, done_init, answer_init, qrock, introck)
struct player *py;
char *qu_str;
char *ans_str;
int done_init, answer_init;
struct query *qrock;
long introck;
{
    int num;

    if (py->siquery.nlines >= py->answers_size) {
	py->answers_size *= 2;
	py->answers = (struct answer *)realloc(py->answers,
			sizeof(struct answer) * py->answers_size);
    }
    num = py->siquery.nlines;
    py->siquery.nlines++;
    py->answers[num].done = done_init;
    py->answers[num].answer = answer_init;
    py->answers[num].query = qrock;
    py->answers[num].rock = introck;
    strcpy(py->answers[num].qu_str, qu_str);
    strcpy(py->answers[num].ans_str, ans_str);
}

int DrawStringField(py, str, xpos, ypos, width)
struct player *py;
char *str;
int xpos, ypos;
int width;
{
    static XCharStruct overall;
    int direction, ascent, descent;
    int len = strlen(str);

    XTextExtents(py->font, str, len, &direction, &ascent, &descent, &overall);
    XDrawImageString(py->dpy, py->win, py->blackgc, xpos, ypos, str, len);
    if (overall.width<width) {
	XClearArea(py->dpy, py->win, xpos+overall.width, ypos-py->ascent,
			width-overall.width, py->totalheight, 0);
    }

    return overall.width;
}

void redraw_queries_only(py, baronly)
struct player *py;
int baronly;
{
    int ix, qline;
    int ypos;

    if (!baronly) {
	for (ix=0; ix<py->siquery.visible; ix++) {
	    qline = py->siquery.top + ix;
	    if (qline<0)
		continue;
	    ypos = QUERY_Y+ix*py->query_hgt+py->lineheight;
	    if (qline >= py->siquery.nlines) {
		DrawStringField(py, "", QUERY_X+16, ypos, QUERY_W-16);
	    }
	    else {
		DrawStringField(py, py->answers[qline].qu_str, QUERY_X+16,
				ypos, QUERY_AW-16);
		DrawStringField(py, py->answers[qline].ans_str, QUERY_X+QUERY_AW,
				ypos, QUERY_W-QUERY_AW);
	    }
	}
    }

    redraw_scrollbar(&(py->siquery));
}

void redraw_queries(py)
struct player *py;
{
    int ix;

    XDrawRectangle(py->dpy, py->win, py->blackgc, QUERY_X-3, QUERY_Y-2,
		QUERY_W+4, QUERY_H+3);
    XDrawRectangle(py->dpy, py->win, py->blackgc, QUERY_X-SCROLL_W, QUERY_Y-2,
		SCROLL_W-3, QUERY_H+3);

    for (ix=0; ix<py->siquery.visible; ix++) {
	if (ix!=0)
	    XDrawLine(py->dpy, py->win, py->blackgc, QUERY_X, QUERY_Y+ix*py->query_hgt,
			QUERY_X+QUERY_W, QUERY_Y+ix*py->query_hgt);
    }
    redraw_queries_only(py, 0);
}

static void add_to_targetlist(tl, name, targnum)
struct target_list *tl;
char *name;
int targnum;
{
    int size;

    if (tl->size==0) {
	tl->size = 8;
	tl->tnums = (int *)malloc(tl->size*sizeof(int));
	tl->tnames = (char **)malloc(tl->size*sizeof(char *));
    }

    size = tl->num;
    tl->num++;

    if (tl->size <= size) {
	while (tl->size <= size)
	    tl->size *= 2;
	tl->tnums = (int *)realloc(tl->tnums, tl->size*sizeof(int));
	tl->tnames = (char **)realloc(tl->tnames, tl->size*sizeof(char *));
    }

    tl->tnums[size] = targnum;
    tl->tnames[size] = name;
}

static void invent_target_lists()
{
    int jx, numtargs;
    char *nm, *realnm;
    int tl;
    long stuff;

    TLBeing.num = 0;
    TLBeingNone.num = 0;
    TLWizard.num = 0;
    TLWizardNone.num = 0;
    TLRaiseDead.num = 0;

    add_to_targetlist(&TLBeingNone, "nobody", 0);
    add_to_targetlist(&TLWizardNone, "nobody", 0);

    numtargs = NumberOfTargets(gameval, QuVal_Target_Wizard);
    for (jx=0; jx<numtargs; jx++) {
	tl = jx | QuVal_Target_Wizard;
	realnm = NameOfTarget(gameval, QuVal_Target_Wizard, jx);
	stuff = StuffAboutTarget(gameval, QuVal_Target_Wizard, jx);
	if (stuff & Stuff_INVISIBLE) {
	    nm = tempstring_malloc(sizeof(char) * (strlen(realnm) + 15));
	    strcpy(nm, realnm);
	    strcat(nm, " [invisible]");
	}
	else
	    nm = realnm;
	add_to_targetlist(&TLBeing, nm, tl);
	add_to_targetlist(&TLBeingNone, nm, tl);
	add_to_targetlist(&TLWizard, nm, tl);
	add_to_targetlist(&TLWizardNone, nm, tl);
	add_to_targetlist(&TLRaiseDead, nm, tl);
    }

    numtargs = NumberOfTargets(gameval, QuVal_Target_Creature);
    for (jx=0; jx<numtargs; jx++) {
	tl = jx | QuVal_Target_Creature;
	realnm = NameOfTarget(gameval, QuVal_Target_Creature, jx);
	stuff = StuffAboutTarget(gameval, QuVal_Target_Creature, jx);
	if (stuff & Stuff_INVISIBLE) {
	    nm = tempstring_malloc(sizeof(char) * (strlen(realnm) + 15)); 
	    strcpy(nm, realnm);
	    strcat(nm, " [invisible]");
	}
	else
	    nm = realnm;
	add_to_targetlist(&TLBeing, nm, tl);
	add_to_targetlist(&TLBeingNone, nm, tl);
	add_to_targetlist(&TLRaiseDead, nm, tl);
    }

    numtargs = NumberOfTargets(gameval, QuVal_Target_Corpse);
    for (jx=0; jx<numtargs; jx++) {
	tl = jx | QuVal_Target_Corpse;
	realnm = NameOfTarget(gameval, QuVal_Target_Corpse, jx);
	nm = tempstring_malloc(sizeof(char) * (strlen(realnm) + 10));
	strcpy(nm, realnm);
	strcat(nm, " [dead]");
	add_to_targetlist(&TLRaiseDead, nm, tl);
    }
}

void XQueries(numqueries, qlist, gameval, rock)
int numqueries;
struct query qlist[];
game *gameval;
struct maingame *rock;
{
    int ix, jx, kx;
    char *handage;
    int spellnum, attnum;
    struct player *py;
    int *hlist;
    int gotany = 0;
    int pnum;
    int answer_init;
    struct target_list *tl;
    static qu_buf[MAXQUESTIONLENGTH], ans_buf[MAXANSWERLENGTH];

    flush_tempstrings();

    turnstate = State_Queries;
    for (ix=0; ix<numplayers; ix++) {
	py = &(players[ix]);
	py->turn_active = 0;
	clear_answers(py);
    }

    for (ix=0; ix<numqueries; ix++) {
	if (qlist[ix].qtype==Qu_NoQuery)
	    continue;
	py = &(players[qlist[ix].player]);
	py->turn_active = 1;
	pnum = py-players;
	gotany = 1;
	switch (qlist[ix].qtype) {
	    case Qu_LeftHand:
	    case Qu_RightHand:
		if (qlist[ix].qtype==Qu_LeftHand) {
		    strcpy(qu_buf, "What spell do you want to cast with your left hand?");
		    tl = &(py->TLLeftHand);
		}
		else {
		    strcpy(qu_buf, "What spell do you want to cast with your right hand?");
		    tl = &(py->TLRightHand);
		}
		hlist = (int *)qlist[ix].rock;
		tl->num = 0;
		for (jx=0; jx<hlist[0]; jx++) {
		    spellnum = hlist[jx+1];
		    if (spellnum & QuVal_Hand_Both) {
			handage = tempstring_malloc(sizeof(char) *
					(strlen(spelllist[spellnum &
					(~QuVal_Hand_Both)].name) + 16));
			sprintf(handage, "%s [both hands]",
					spelllist[spellnum &
					(~QuVal_Hand_Both)].name);
		    }
		    else
			handage = spelllist[spellnum & (~QuVal_Hand_Both)].name;
		    add_to_targetlist(tl, handage, jx);
		}
		strcpy(ans_buf, "");
		add_answer(py, qu_buf, ans_buf, 0, 0, &(qlist[ix]), (long)hlist);
		turnstate = State_EQueries;
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
		sprintf(qu_buf, "What gesture do you want %s's %s hand to make?",
				NameOfBeing(gameval, QuVal_Target_Wizard,
				spellnum), (jx?"right":"left"));
		strcpy(ans_buf, "");
		add_answer(py, qu_buf, ans_buf, 0, 0, &(qlist[ix]), 0);
		turnstate = State_EQueries;
		break;
	    case Qu_WhichToDelay:
	    case Qu_WhichToPerm:
		/* use TLRightHand and TLLeftHand, since these queries always
		   show up by themselves */
		if (qlist[ix].qtype == Qu_WhichToDelay) {
		    tl = &(py->TLRightHand);
		    sprintf(qu_buf, "Which spell do you want to delay?");
		}
		else {
		    tl = &(py->TLLeftHand);
		    sprintf(qu_buf, "Which spell do you want to make permanent?");
		}
		hlist = (int *)qlist[ix].rock;
		tl->num = 0;
		for (jx=0; hlist[jx]!=(-1); jx++) {
		    add_to_targetlist(tl, spelllist[hlist[jx]].name, jx);
		}
		strcpy(ans_buf, "");
		add_answer(py, qu_buf, ans_buf, 0, 0, &(qlist[ix]), 0);
		turnstate = State_EQueries;
		break;
	    case Qu_SetOffDelay:
		sprintf(qu_buf, "Do you want to release the %s from the Delayed Effect?",
				spelllist[(int)qlist[ix].rock].name);
		strcpy(ans_buf, "No");
		add_answer(py, qu_buf, ans_buf, 1, 0, &(qlist[ix]), 0);
		turnstate = State_EQueries;
		break;
	    case Qu_ParalysisHand:
	    case Qu_CharmHand:
		if (qlist[ix].qtype == Qu_ParalysisHand) 
		    sprintf(qu_buf, "Which of %s's hands do you want to paralyze?",
				NameOfBeing(gameval, QuVal_Target_Wizard,
				qlist[ix].rock));
		else
		    sprintf(qu_buf, "Which of %s's hands do you want to control?",
				NameOfBeing(gameval, QuVal_Target_Wizard,
				qlist[ix].rock));
		strcpy(ans_buf, "");
		add_answer(py, qu_buf, ans_buf, 0, 0, &(qlist[ix]), 0);
		break;
	    case Qu_ElementalType:
		sprintf(qu_buf, "Which type of elemental do you want to summon?");
		strcpy(ans_buf, "");
		add_answer(py, qu_buf, ans_buf, 0, 0, &(qlist[ix]), 0);
		break;
	    case Qu_MonsterTarget:
		jx = (int)(qlist[ix].rock);
		attnum = jx / 256;
		spellnum = jx % 256;
		switch (attnum) {
		    case 1:
			sprintf(qu_buf, "Whom do you want %s to attack?",
					NameOfBeing(gameval, QuVal_Target_Creature,
					spellnum));
			break;
		    case 2:
			sprintf(qu_buf, "Whom do you want %s's first attack to be at?",
					NameOfBeing(gameval, QuVal_Target_Creature,
					spellnum));
			break;
		    case 3:
			sprintf(qu_buf, "Whom do you want %s's second attack to be at?",
					NameOfBeing(gameval, QuVal_Target_Creature,
					spellnum));
			break;
		    default:
			sprintf(qu_buf, "ERROR: Query about %d (%d)", spellnum, attnum);
			break;
		}
		{
		    int numtargs = NumberOfTargets(gameval, QuVal_Target_Wizard);
		    strcpy(ans_buf, "nobody");
		    answer_init = 0;
		    for (jx=0; jx<numtargs; jx++) {
			kx = (IndexOfTarget(gameval, QuVal_Target_Wizard, jx) &
					~QuVal_Target_MASK);
			if (kx != pnum) {
			    answer_init = jx | QuVal_Target_Wizard;
			    strcpy(ans_buf, NameOfTarget(gameval,
					QuVal_Target_Wizard, jx));
			    break;
			}
		    }
		}
		add_answer(py, qu_buf, ans_buf, 1, answer_init, &(qlist[ix]), 0);
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
		    sprintf(qu_buf, "Who do you want to stab at (%s)?", handage);
		else
		    sprintf(qu_buf, "Who do you want to cast %s at (%s)?",
				spelllist[spellnum].name, handage);
		if (spelllist[spellnum].selfcast) {
		    int numtargs = NumberOfTargets(gameval, QuVal_Target_Wizard);
		    strcpy(ans_buf, "nobody");
		    answer_init = 0;
		    for (jx=0; jx<numtargs; jx++) {
			kx = (IndexOfTarget(gameval, QuVal_Target_Wizard, jx) &
					~QuVal_Target_MASK);
			if (kx == pnum) {
			    answer_init = jx | QuVal_Target_Wizard;
			    strcpy(ans_buf, NameOfTarget(gameval,
					QuVal_Target_Wizard, jx));
			    if (StuffAboutTarget(gameval, QuVal_Target_Wizard,
					jx) & Stuff_INVISIBLE)
				strcat(ans_buf, " [invisible]");
			    break;
			}
		    }
		}
		else {
		    int numtargs = NumberOfTargets(gameval, QuVal_Target_Wizard);
		    strcpy(ans_buf, "nobody");
		    answer_init = 0;
		    for (jx=0; jx<numtargs; jx++) {
			kx = (IndexOfTarget(gameval, QuVal_Target_Wizard, jx) &
					~QuVal_Target_MASK);
			if (kx != pnum) {
			    answer_init = jx | QuVal_Target_Wizard;
			    strcpy(ans_buf, NameOfTarget(gameval,
					QuVal_Target_Wizard, jx));
			    if (StuffAboutTarget(gameval, QuVal_Target_Wizard,
					jx) & Stuff_INVISIBLE)
				strcat(ans_buf, " [invisible]");
			    break;
			}
		    }
		}
		add_answer(py, qu_buf, ans_buf, 1, answer_init, &(qlist[ix]), 0);
		turnstate = State_EQueries;
		break;
	    default:
		break;
	}
    }

    if (!gotany)
	return;

    invent_target_lists();

    for (ix=0; ix<numplayers; ix++) {
	py = &(players[ix]);
	py->turn_done = !py->turn_active;
    }
    for (ix=0; ix<numplayers; ix++) {
	py = &(players[ix]);
	redraw_column(py);
	draw_button(py, 0, 1); /* redraw turn-end button */
	redraw_queries_only(py, 0);
	/*update_statlist(py);*/
    }
    mainloop();
    for (ix=0; ix<numplayers; ix++) {
	py = &(players[ix]);
	for (jx=0; jx<py->siquery.nlines; jx++) {
	    py->answers[jx].query->answer = py->answers[jx].answer;
	}
    }
}

void draw_query_menu(py)
struct player *py;
{
    int ix, jx;
    int qtype = py->answers[py->query_hit].query->qtype;

    draw_frame(py, &py->query_rect);
    if (qtype==Qu_CharmGesture) {
	int posx, posy, hand;
	posx = py->query_rect.x+20;
	posy = py->query_rect.y+20;
	hand = (strstr(py->answers[py->query_hit].qu_str, "right hand") ? 1 : 0);
	for (jx=0; jx<2; jx++)
	    for (ix=0; ix<4; ix++) {
		XCopyPlane(py->dpy, py->gesturebm[hand][jx*4+ix], py->win,
				py->blackgc, 0, 0, GEST_SIZE, GEST_SIZE,
				posx+ix*GEST_SPACE, posy+jx*GEST_SPACE, 1);
	    }
    }
    else {
	for (ix=0; ix<py->query_tl->num; ix++) {
	    XDrawImageString(py->dpy, py->win, py->blackgc, py->query_rect.x+16,
			py->query_rect.y + ix*(py->lineheight+QMENU_LEADING) +
			py->lineheight + QMENU_BORDER, py->query_tl->tnames[ix],
			strlen(py->query_tl->tnames[ix]));
	}
    }
}

int in_query_box(py, xpos, ypos)
struct player *py;
int xpos, ypos;
{
    return (xpos>=QUERY_X && ypos>=QUERY_Y && xpos<QUERY_X+QUERY_W
	    && ypos<QUERY_Y+QUERY_H);
}

int query_hit(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int hitq = (ypos-QUERY_Y) / py->query_hgt + py->siquery.top;
    int ix;
    static XCharStruct overall;
    int direction, ascent, descent;

    if (!py->turn_active || py->turn_done)
	return ms_None;

    if (hitq<0 || hitq>=py->siquery.nlines)
	return ms_None;

    if (!py->answers[hitq].query)
	return ms_None;

    switch (py->answers[hitq].query->qtype) {
	case Qu_WhichToPerm:
	    py->query_tl = (&(py->TLLeftHand));
	    break;
	case Qu_WhichToDelay:
	    py->query_tl = (&(py->TLRightHand));
	    break;
	case Qu_LeftHand:
	    py->query_tl = (&(py->TLLeftHand));
	    break;
	case Qu_RightHand:
	    py->query_tl = (&(py->TLRightHand));
	    break;
	case Qu_ElementalType:
	    py->query_tl = (&TLElementalType);
	    break;
	case Qu_MonsterTarget:
	    py->query_tl = (&TLBeingNone);
	    break;
	case Qu_TargetBeing:
	    py->query_tl = (&TLBeing);
	    break;
	case Qu_TargetBeingNone:
	    py->query_tl = (&TLBeingNone);
	    break;
	case Qu_TargetWizard:
	    py->query_tl = (&TLWizard);
	    break;
	case Qu_TargetWizardNone:
	    py->query_tl = (&TLWizardNone);
	    break;
	case Qu_TargetRaiseDead:
	    py->query_tl = (&TLRaiseDead);
	    break;
	case Qu_ParalysisHand:
	case Qu_CharmHand:
	    py->query_tl = (&TLPickHand);
	    break;
	case Qu_SetOffDelay:
	    py->query_tl = (&TLNoYes);
	    break;
	case Qu_CharmGesture:
	    py->query_tl = NULL;
	    break;
	case Qu_SaveTranscript: /* special value defined in xspell.h */
	    py->query_tl = (&TLNoYes);
	    break;
	default:
	    return ms_None;
	    break;
    }

    py->query_hit = hitq;

    if (py->answers[hitq].query->qtype==Qu_CharmGesture) {
	py->query_rect.w = 240;
	py->query_rect.h = 140;
	py->query_rect.x = xpos - py->query_rect.w/2;
	py->query_rect.y = ypos - py->query_rect.h/2;
    }
    else {
	int maxwid = 0;
	for (ix=0; ix<py->query_tl->num; ix++) {
	    XTextExtents(py->font, py->query_tl->tnames[ix],
			strlen(py->query_tl->tnames[ix]), &direction, &ascent,
			&descent, &overall);
	    if (overall.width > maxwid)
		maxwid = overall.width;
	}
	py->query_rect.w = maxwid + 32 + FRAME_SHADOW;
	py->query_rect.h = py->query_tl->num * (py->lineheight+QMENU_LEADING) +
			2*QMENU_BORDER+FRAME_SHADOW;
	py->query_rect.x = xpos - py->query_rect.w/2;
	py->query_rect.y = ypos - py->query_rect.h/2;
    }
    py->query_sel = (-1);
    adjust_rect(&py->query_rect);
    backing_store(py, &py->query_rect);
    draw_query_menu(py);
    query_motion(py, xpos, ypos, button);

    return ms_Query;
}

int query_motion(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int hitrow;
    int qtype = py->answers[py->query_hit].query->qtype;

    if (qtype==Qu_CharmGesture) {
	int posx, posy, hitx, hity;

	posx = py->query_rect.x+20;
	posy = py->query_rect.y+20;

	hitx = (xpos - posx + GEST_SPACE) / GEST_SPACE - 1;
	hity = (ypos - posy + GEST_SPACE) / GEST_SPACE - 1;

	if (hitx>=0 && hitx<4 && hity>=0 && hity<2)
	    hitrow = hity*4 + hitx;
	else
	    hitrow = (-1);

	if (hitrow==py->query_sel)
	    return;

	if (py->query_sel != (-1)) {
	    /* erase old */
	    hitx = py->query_sel % 4;
	    hity = py->query_sel / 4;
	    XDrawRectangle(py->dpy, py->win, py->whitegc, posx+hitx*GEST_SPACE-1,
			posy+hity*GEST_SPACE-1, GEST_SIZE+1, GEST_SIZE+1);
	    XDrawRectangle(py->dpy, py->win, py->whitegc, posx+hitx*GEST_SPACE-2,
			posy+hity*GEST_SPACE-2, GEST_SIZE+3, GEST_SIZE+3);
	}

	py->query_sel = hitrow;

	if (py->query_sel != (-1)) {
	    /* draw new */
	    hitx = py->query_sel % 4;
	    hity = py->query_sel / 4;
	    XDrawRectangle(py->dpy, py->win, py->blackgc, posx+hitx*GEST_SPACE-1,
			posy+hity*GEST_SPACE-1, GEST_SIZE+1, GEST_SIZE+1);
	    XDrawRectangle(py->dpy, py->win, py->blackgc, posx+hitx*GEST_SPACE-2,
			posy+hity*GEST_SPACE-2, GEST_SIZE+3, GEST_SIZE+3);
	}
    }
    else {
	ypos -= (py->query_rect.y + QMENU_BORDER);
	if (ypos<0 || xpos < py->query_rect.x ||
			xpos >= py->query_rect.x+py->query_rect.w)
	    hitrow = (-1);
	else 
	    hitrow = ypos / (py->lineheight+QMENU_LEADING);

	if (hitrow<0 || hitrow>=py->query_tl->num)
	    hitrow = (-1);

	if (hitrow==py->query_sel)
	    return;

	if (py->query_sel != (-1)) {
	    /* erase old */
	    XDrawRectangle(py->dpy, py->win, py->whitegc,
			py->query_rect.x+QMENU_BORDER, py->query_rect.y +
			py->query_sel*(py->lineheight+QMENU_LEADING) +
			QMENU_BORDER, py->query_rect.w-(FRAME_SHADOW+2*QMENU_BORDER),
			py->lineheight+QMENU_LEADING);
	}

	py->query_sel = hitrow;

	if (py->query_sel != (-1)) {
	    /* draw new */
	    XDrawRectangle(py->dpy, py->win, py->blackgc,
			py->query_rect.x+QMENU_BORDER, py->query_rect.y +
			py->query_sel*(py->lineheight+QMENU_LEADING) +
			QMENU_BORDER, py->query_rect.w-(FRAME_SHADOW+2*QMENU_BORDER),
			py->lineheight+QMENU_LEADING);
	}
    }
}

int query_release(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int qtype = py->answers[py->query_hit].query->qtype;
    int ix, jx;

    backing_restore(py);
    if (py->query_sel != (-1)) {
	if (qtype==Qu_CharmGesture) {
	    py->answers[py->query_hit].answer = py->query_sel;
	    py->answers[py->query_hit].done = 1;
	    strcpy(py->answers[py->query_hit].ans_str, gesture_name(py->query_sel));
	}
	else {
	    py->answers[py->query_hit].answer = py->query_tl->tnums[py->query_sel];
	    py->answers[py->query_hit].done = 1;
	    strcpy(py->answers[py->query_hit].ans_str, py->query_tl->tnames[py->query_sel]);
	    if (qtype==Qu_LeftHand || qtype==Qu_RightHand) {
		struct answer *ans = (&(py->answers[py->query_hit]));
		struct answer *anso;
		int *hlist = (int *)ans->rock;
		int *hlisto;
		int otherq;
		int forceans = (-1);
		struct target_list *othertl;

		if (qtype==Qu_LeftHand) {
		    otherq = Qu_RightHand;
		    othertl = (&(py->TLRightHand));
		}
		else {
		    otherq = Qu_LeftHand;
		    othertl = (&(py->TLLeftHand));
		}

		for (ix=0; ix<py->siquery.nlines; ix++) {
		    if (py->answers[ix].query->qtype == otherq) {
			anso = (&(py->answers[ix]));
			hlisto = (int *)anso->rock;
			if (hlist[py->query_sel+1] & QuVal_Hand_Both) {
			    /* force other hand to same spell */
			    for (jx=0; jx<hlisto[0]; jx++)
				if (hlist[py->query_sel+1] == hlisto[jx+1]) {
				    forceans = jx;
				    break;
				}
			}
			else {
			    /* should force other hand away from 2-handed
			       answer, but there is no case in the game where
			       this is a concern. */
			}
			if (forceans != (-1)) {
			    /*printf("Forcing q.%d to %d\n", ix, forceans);
			      fflush(stdout);*/
			    py->answers[ix].answer = othertl->tnums[forceans];
			    py->answers[ix].done = 1;
			    strcpy(py->answers[ix].ans_str, othertl->tnames[forceans]);
			}
		    }
		}
	    }
	}
	redraw_queries_only(py, 0);
    }
}

