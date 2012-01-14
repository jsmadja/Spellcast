#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "handwave.h"
#include "spelllist.h"
#include "xspell.h"

extern void redraw(), redraw_stats(), redraw_queries(), redraw_buttons(),
	redraw_text(), redraw_column();
extern int gesture_hit();
void redraw_stats_only(), update_statlist();
void redraw_done_markers();

int blinklevel; /* in tenths of a second */

void mainloop()
{
    int pnum;
    XEvent event;
    int gotit, gotround;
    struct player *py;
    fd_set readbits;
    int numsocks, sock;
    int doneloop = 0;
    char c;
    struct timeval startround, now, pausetime;

    blinklevel = 0;
    for (pnum=0; pnum<numplayers; pnum++) {
	players[pnum].mousestate = ms_None;
	players[pnum].turn_blinklimit = (random() % 50) + 5;
	players[pnum].turn_blinked = 0;
    }
    for (pnum=0; pnum<numplayers; pnum++)
	update_statlist(&players[pnum]);
    gettimeofday(&startround, NULL);

    while (!doneloop) {
	gotround = 0;
	for (pnum=0; pnum<numplayers; pnum++) {
	    gotit = XCheckWindowEvent(players[pnum].dpy, players[pnum].win,
			EVENTMASK, &event);
	    if (gotit) {
		gotround = 1;
		py = &(players[pnum]);
		switch (event.type) {
		    case Expose:
			do {
			    gotit = XCheckWindowEvent(players[pnum].dpy,
					players[pnum].win, ExposureMask, &event);
			} while (gotit);
			py->gotexpose = 1;
			redraw(py);
			break;
		    case KeyPress:
			if (1 == XLookupString(&event, &c, 1, NULL, NULL)) {
			    switch (c) {
				case '\014': /* Ctrl-L */
				    XClearArea(py->dpy, py->win, 0, 0, 0, 0, 1);
				    break;
				default:
				    talk_handle_char(py, c);
			    }
			}
			break;
		    case ButtonPress:
			if (py->mousestate != ms_None) {
			    break;
			}
			if (in_gesture_box(py, event.xbutton.x, event.xbutton.y)) {
			    py->mousestate = gesture_hit(py, event.xbutton.x,
					event.xbutton.y, event.xbutton.button);
			    py->mousebutton = event.xbutton.button;
			}
			else if (in_bbox_box(py, event.xbutton.x, event.xbutton.y)) {
			    py->mousestate = button_hit(py, event.xbutton.x,
					event.xbutton.y, event.xbutton.button);
			    py->mousebutton = event.xbutton.button;
			}
			else if (in_scrollbar_box(&(py->sitext), event.xbutton.x,
					event.xbutton.y)) {
			    py->mousestate = scrollbar_hit(&(py->sitext),
					event.xbutton.x, event.xbutton.y,
					event.xbutton.button);
			    py->mousebutton = event.xbutton.button;
			}
			else if (in_scrollbar_box(&(py->siquery), event.xbutton.x,
					event.xbutton.y)) {
			    py->mousestate = scrollbar_hit(&(py->siquery),
					event.xbutton.x, event.xbutton.y,
					event.xbutton.button);
			    py->mousebutton = event.xbutton.button;
			}
			else if (in_scrollbar_box(&(py->sistats), event.xbutton.x,
					event.xbutton.y)) {
			    py->mousestate = scrollbar_hit(&(py->sistats),
					event.xbutton.x, event.xbutton.y,
					event.xbutton.button);
			    py->mousebutton = event.xbutton.button;
			}
			else if (in_query_box(py, event.xbutton.x,
					event.xbutton.y)) {
			    py->mousestate = query_hit(py, event.xbutton.x,
					event.xbutton.y, event.xbutton.button);
			    py->mousebutton = event.xbutton.button;
			}
			else {
			    py->mousestate = ms_None;
			}
			break;
		    case MotionNotify:
			switch (py->mousestate) {
			    case ms_Gesture:
				gesture_motion(py, event.xbutton.x, event.xbutton.y,
						event.xbutton.button);
				break;
			    case ms_Scroll:
				scrollbar_motion(py->scroll_active, event.xbutton.x,
						event.xbutton.y, event.xbutton.button);
				break;
			    case ms_Query:
				query_motion(py, event.xbutton.x, event.xbutton.y,
						event.xbutton.button);
				break;
			    case ms_DoneBtn:
			    case ms_SpellBtn:
				button_motion(py, event.xbutton.x, event.xbutton.y,
						event.xbutton.button);
				break;
			    default:
				break;
			}
			break;
		    case ButtonRelease:
			if (py->mousebutton == event.xbutton.button) {
			    switch (py->mousestate) {
				case ms_Gesture:
				    gesture_release(py, event.xbutton.x,
						event.xbutton.y, event.xbutton.button);
				    break;
				case ms_Scroll:
				    scrollbar_release(py->scroll_active,
						event.xbutton.x, event.xbutton.y,
						event.xbutton.button);
				    break;
				case ms_Query:
				    query_release(py, event.xbutton.x,
						event.xbutton.y, event.xbutton.button);
				    break;
				case ms_DoneBtn:
				case ms_SpellBtn:
				    button_release(py, event.xbutton.x,
						event.xbutton.y, event.xbutton.button);
				    break;
				default:
				    break;
			    }
			    py->mousestate = ms_None;
			}
			break;
		    default:
			break;
		}
	    }
	}
	doneloop = 1;
	for (pnum=0; pnum<numplayers; pnum++) {
	    if ((players[pnum].turn_active && !players[pnum].turn_done) ||
			players[pnum].mousestate!=ms_None) {
		doneloop = 0;
		break;
	    }
	}
	{
	    /* check time */
	    long sofar;
	    gettimeofday(&now, NULL);
	    sofar = (now.tv_sec - startround.tv_sec) * 10 +
			(now.tv_usec - startround.tv_usec) / 100000;
	    if (sofar > blinklevel) {
		blinklevel = sofar;
		for (pnum=0; pnum<numplayers; pnum++) 
		    if (!players[pnum].turn_blinked
			&& blinklevel > players[pnum].turn_blinklimit) {
			players[pnum].turn_blinked = 1;
			redraw_done_markers(&(players[pnum]));
		    }
	    }
	}
	if (!gotround && !doneloop) {
	    /* no events -- wait a while */
	    FD_ZERO(&readbits);
	    numsocks = 0;
	    for (pnum=0; pnum<numplayers; pnum++) {
		sock = ConnectionNumber(players[pnum].dpy);
		if (sock+1 > numsocks)
		    numsocks = sock+1;
		FD_SET(sock, &readbits);
		XFlush(players[pnum].dpy);
	    }
	    pausetime.tv_sec = 0;
	    pausetime.tv_usec = 200000; /* 1/5th of a second */
	    (void)select(numsocks, &readbits, 0, 0, &pausetime);
	}
    }
}

void redraw(py)
struct player *py;
{
    redraw_text(py);
    redraw_column(py);
    redraw_buttons(py);
    redraw_queries(py);
    redraw_stats(py);
    redraw_talk(py);
    if (py->backstore)
	switch (py->mousestate) {
	    case ms_Gesture:
		draw_gesture_menu(py);
		break;
	    case ms_SpellBtn:
		draw_spelllist(py);
		break;
	    case ms_Query:
		draw_query_menu(py);
		break;
	    default:
		break;
	}
}

#define STATS_X (476)
#define STATS_Y (449)
#define STATS_W (324)
#define STATS_H (104)
#define STATS_HP (244)

void init_stats(py)
struct player *py;
{
    py->statlist_size = 4;
    py->statlist = (struct statthing *)malloc(sizeof(struct statthing) * py->statlist_size);

    py->sistats.nlines = 0;
    py->sistats.visible = (STATS_H-2) / py->lineheight;
    py->sistats.py = py;
    py->sistats.top = 0;
    py->sistats.lineheight = py->lineheight;
    py->sistats.x = STATS_X;
    py->sistats.y = STATS_Y;
    py->sistats.h = STATS_H;
    py->sistats.redraw = redraw_stats_only;
}

void clear_stats(py)
struct player *py;
{
    py->sistats.nlines = 0;
    py->sistats.top = 0;
}

void add_statthing(py, index, type, hp, stuff)
struct player *py;
int index, type;
int hp;
long stuff;
{
    int num;

    if (py->sistats.nlines >= py->statlist_size) {
	while (py->sistats.nlines >= py->statlist_size)
	    py->statlist_size *= 2;
	py->statlist = (struct statthing *)realloc(py->statlist,
			sizeof(struct statthing) * py->statlist_size);
    }
    num = py->sistats.nlines;
    py->sistats.nlines++;
    py->statlist[num].index = index;
    py->statlist[num].type = type;
    py->statlist[num].hp = hp;
    py->statlist[num].stuff = stuff;
}

#define DRAWSTAR(py)  ((py)->turn_active ? (py)->turn_done : (py)->turn_blinked)

void redraw_done_markers(pdone)
struct player *pdone;
{
    int pnum, pdonenum;
    int ix, qline;
    int ypos;

    struct player *py;

    pdonenum = (pdone-players);
    for (pnum=0; pnum<numplayers; pnum++) {
	py = &(players[pnum]);
	for (ix=0; ix<py->sistats.visible; ix++) {
	    qline = py->sistats.top + ix;
	    if (qline >= py->sistats.nlines) break;
	    if (py->statlist[qline].type == QuVal_Target_Wizard
		&& py->statlist[qline].index == pdonenum)
		break;
	}
	if (ix<py->sistats.visible
	    && !(qline >= py->sistats.nlines)) {
	    ypos = STATS_Y+ix*py->lineheight+py->lineheight;
	    py->gotexpose = 1;
	    if (!py->backstore) {
		if (DRAWSTAR(pdone)) {
		    DrawStringField(py, "*", STATS_X+4, ypos, 12);
		}
		else {
		    DrawStringField(py, " ", STATS_X+4, ypos, 12);
		}
	    }
	}
    }
}

void redraw_stats_only(py, baronly)
struct player *py;
int baronly;
{
    int ix, qline;
    int ypos;
    struct statthing *st;
    char sbuf[32];
    struct player *other;

    if (!baronly) {
	for (ix=0; ix<py->sistats.visible; ix++) {
	    qline = py->sistats.top + ix;
	    if (qline<0)
		continue;
	    ypos = STATS_Y+ix*py->lineheight+py->lineheight;
	    if (qline >= py->sistats.nlines) {
		DrawStringField(py, "", STATS_X+4, ypos, STATS_W-4);
	    }
	    else {
		st = &(py->statlist[qline]);
		if (st->type==QuVal_Target_Wizard) {
		    other = &(players[st->index]);
		    if (DRAWSTAR(other)) {
			DrawStringField(py, "*", STATS_X+4, ypos, 12);
		    }
		    else {
			DrawStringField(py, " ", STATS_X+4, ypos, 12);
		    }
		    DrawStringField(py, NameOfBeing(gameval, st->type, st->index),
				STATS_X+16, ypos, STATS_W-16);
		}
		else {
		    DrawStringField(py, "", STATS_X+4, ypos, 20);
		    DrawStringField(py, NameOfBeing(gameval, st->type, st->index),
				STATS_X+24, ypos, STATS_W-24);
		}
		sprintf(sbuf, "%d %s%s%s%s%s%s%s", st->hp,
			(st->stuff & Stuff_INVISIBLE) ? "I" : "",
			(st->stuff & Stuff_RESIST_HEAT) ? "H" : "",
			(st->stuff & Stuff_RESIST_COLD) ? "C" : "",
			(st->stuff & Stuff_PROTECT_EVIL) ? "P" : "",
			(st->stuff & Stuff_BLIND) ? "b" : "",
			(st->stuff & Stuff_DISEASE) ? "d" : "",
			(st->stuff & Stuff_POISON) ? "p" : "");
		DrawStringField(py, sbuf, STATS_X+STATS_HP, ypos, STATS_W-STATS_HP);
	    }
	}
    }

    redraw_scrollbar(&(py->sistats));
}

void redraw_stats(py)
struct player *py;
{
    int ix;

    XDrawRectangle(py->dpy, py->win, py->blackgc, STATS_X-3, STATS_Y-2,
		STATS_W+4, STATS_H+3);
    XDrawRectangle(py->dpy, py->win, py->blackgc, STATS_X-SCROLL_W, STATS_Y-2,
		SCROLL_W-3, STATS_H+3);

    redraw_stats_only(py, 0);
}

static int statthing_pnum;

int statthing_compare(st1, st2)
struct statthing *st1, *st2;
{
    int own1, own2;

    if (st1->type==QuVal_Target_Wizard)
	own1 = st1->index;
    else
	own1 = OwnerOfCreature(gameval, st1->index);
    if (st2->type==QuVal_Target_Wizard)
	own2 = st2->index;
    else
	own2 = OwnerOfCreature(gameval, st2->index);

    if (own1 != own2) {
	if (own1==(-1)) own1 = (-300);
	else if (own1 == statthing_pnum)
	    own1 = (-200);
	if (own2==(-1)) own2 = (-300);
	else if (own2 == statthing_pnum)
	    own2 = (-200);
	return (own1 - own2);
    }

    if (st1->type==QuVal_Target_Wizard && st2->type==QuVal_Target_Wizard)
	return (0);
    if (st1->type==QuVal_Target_Wizard)
	return (-100);
    if (st2->type==QuVal_Target_Wizard)
	return (100);
    return (st1->index - st2->index);
}

void update_statlist(py)
struct player *py;
{
    int ix, max, hp;
    long stuff;
    clear_stats(py);

    max = NumberOfBeings(gameval, QuVal_Target_Wizard);
    for (ix=0; ix<max; ix++) {
	hp = HitPointsOfBeing(gameval, QuVal_Target_Wizard, ix);
	stuff = StuffAboutBeing(gameval, QuVal_Target_Wizard, ix);
	if (hp>=0)
	    add_statthing(py, ix, QuVal_Target_Wizard, hp, stuff);
    }
    max = NumberOfBeings(gameval, QuVal_Target_Creature);
    for (ix=0; ix<max; ix++) {
	hp = HitPointsOfBeing(gameval, QuVal_Target_Creature, ix);
	stuff = StuffAboutBeing(gameval, QuVal_Target_Creature, ix);
	if (hp>=0)
	    add_statthing(py, ix, QuVal_Target_Creature, hp, stuff);
    }

    statthing_pnum = py-players;
    qsort(py->statlist, py->sistats.nlines, sizeof(struct statthing), statthing_compare);

    redraw_stats_only(py, 0);
}
