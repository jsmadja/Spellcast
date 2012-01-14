#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "handwave.h"
#include "spelllist.h"
#include "xspell.h"

#define BBOX_X (32)
#define BBOX_Y (576)
#define BBOX_W (2)
#define BBOX_H (1)

#define BUTTON_W (150)
#define BUTTON_H (32)
#define BUTTON_CORNER (3)
#define BUTTON_SPACE (4)

void draw_button();

void DrawStringCentered(py, str, xpos, ypos)
struct player *py;
char *str;
int xpos, ypos;
{
    static XCharStruct overall;
    int direction, ascent, descent;
    int len = strlen(str);

    XTextExtents(py->font, str, len, &direction, &ascent, &descent, &overall);
    XDrawImageString(py->dpy, py->win, py->blackgc, xpos-(overall.width)/2, ypos, str, len);

}

/* list NUMSPELLS-1 spells (skip SP__STAB) */
void draw_spelllist(py)
struct player *py;
{
    int ix, jx, snum, gx, val, pos;
    int tablehgt;
    struct spelldef *spel;
    char *cx;

#define SL_COLUMNS (2)
    int posx = py->spelllist_rect.x+16;
    int posy = py->spelllist_rect.y+32;

    draw_frame(py, &py->spelllist_rect);

    if (py->spelllist_way==1)
	cx = "Spell List (left button: sorted by gesture)";
    else if (py->spelllist_way==2)
	cx = "Spell List (middle button: alphabetical sort)";
    else
	cx = "Spell List (right button: reversed sort by gesture)";

    DrawStringCentered(py, cx, py->spelllist_rect.x+py->spelllist_rect.w/2,
		py->spelllist_rect.y+4+py->lineheight);

    tablehgt = (NUMSPELLS-1+SL_COLUMNS-1) / SL_COLUMNS;
    for (ix=0; ix<SL_COLUMNS; ix++)
	for (jx=0; jx<tablehgt; jx++) {
	    if (py->spelllist_way==1)
		snum = ix*tablehgt+jx;
	    else if (py->spelllist_way==2)
		snum = alphabet_sort_list[ix*tablehgt+jx];
	    else
		snum = reverse_sort_list[ix*tablehgt+jx];
	    if (snum >= NUMSPELLS-1)
		break;
	    spel = &(spelllist[snum]);
	    for (gx=0; gx<spel->length; gx++) {
		val = spel->gests[gx];
		pos = ((py->spelllist_way==3) ? (gx+(8-spel->length))*(GEST_SMALL+1)-8 :
				gx*(GEST_SMALL+1));
				/*spel->length-gx-1 for reversed gestures*/
		XCopyPlane(py->dpy, py->spelllistbm [(val&Gesture_DOUBLE)?1:0]
				[(val&(~Gesture_DOUBLE))-1], py->win, py->blackgc,
				0, 0, GEST_SMALL, GEST_SMALL, posx+ix*300+pos,
				posy+jx*18, 1);
	    }
	    if (py->spelllist_way!=3) 
		for (gx = spel->length; gx<8; gx++) {
		    pos = gx*(GEST_SMALL+1);
		    XCopyPlane(py->dpy, py->spelllistbm[0][6], py->win, py->blackgc,
				0, 0, GEST_SMALL, GEST_SMALL, posx+ix*300+pos,
				posy+jx*18, 1);
		}
	    XDrawImageString(py->dpy, py->win, py->blackgc, posx+ix*300+8*(GEST_SMALL+1),
			posy+jx*18+((GEST_SMALL+py->lineheight)/2)-2, spel->name,
			strlen(spel->name));
	}
}

static void light_button(py, bnum)
struct player *py;
int bnum;
{
    int hitx, hity;

    if (py->button_lit != (-1)) {
	hitx = py->button_lit % BBOX_W;
	hity = py->button_lit / BBOX_W;
	XDrawRectangle(py->dpy, py->win, py->whitegc, BBOX_X+hitx*BUTTON_W-1,
			BBOX_Y+hity*BUTTON_H-1, BUTTON_W-BUTTON_SPACE+2,
			BUTTON_H-BUTTON_SPACE+2);
    }

    py->button_lit = bnum;
    if (py->button_lit != (-1)) {
	hitx = py->button_lit % BBOX_W;
	hity = py->button_lit / BBOX_W;
	XDrawRectangle(py->dpy, py->win, py->blackgc, BBOX_X+hitx*BUTTON_W-1,
			BBOX_Y+hity*BUTTON_H-1, BUTTON_W-BUTTON_SPACE+2,
			BUTTON_H-BUTTON_SPACE+2);
    }
}

void redraw_buttons(py)
struct player *py;
{
    int bnum;

    for (bnum=0; bnum<BBOX_W*BBOX_H; bnum++) {
	draw_button(py, bnum, 0);
    }
    light_button(py, py->button_lit);
}

void draw_button(py, bnum, mode)
struct player *py;
int bnum;
int mode; /* 0 for all, 1 for clear interior and relabel */
{
    int ix, jx;
    char *cx;

    static XPoint plist[9] = {
	{0+BUTTON_CORNER, 0},
	{BUTTON_W-BUTTON_SPACE-2*BUTTON_CORNER, 0},
	{BUTTON_CORNER, BUTTON_CORNER},
	{0, BUTTON_H-BUTTON_SPACE-2*BUTTON_CORNER},
	{-BUTTON_CORNER, BUTTON_CORNER},
	{-(BUTTON_W-BUTTON_SPACE-2*BUTTON_CORNER), 0},
	{-BUTTON_CORNER, -BUTTON_CORNER},
	{0, -(BUTTON_H-BUTTON_SPACE-2*BUTTON_CORNER)},
	{BUTTON_CORNER, -BUTTON_CORNER},
    };

    ix = bnum % BBOX_W;
    jx = bnum / BBOX_W;

    if (mode==0) {
	plist[0].x = BBOX_X + ix*BUTTON_W + BUTTON_CORNER;
	plist[0].y = BBOX_Y + jx*BUTTON_H;
	XDrawLines(py->dpy, py->win, py->blackgc, plist, 9, CoordModePrevious);
    }

    if (mode==1) {
	/* clear interior */
	XClearArea(py->dpy, py->win, BBOX_X + ix*BUTTON_W + BUTTON_CORNER,
			BBOX_Y + jx*BUTTON_H + BUTTON_CORNER,
			BUTTON_W-BUTTON_SPACE-2*BUTTON_CORNER,
			BUTTON_H-BUTTON_SPACE-2*BUTTON_CORNER, 0);
    }

    switch (bnum) {
	case 0:
	    if (!py->turn_active)
		cx = "please wait....";
	    else if (turnstate == State_Top) {
		if (!py->turn_done)
		    cx = "End Move";
		else
		    cx = "Move ENDED.";
	    }
	    else if (turnstate == State_End) {
		if (!py->turn_done)
		    cx = "Quit";
		else
		    cx = "Wait....";
	    }
	    else {
		if (!py->turn_done)
		    cx = "End Answers";
		else
		    cx = "Answers ENDED.";
	    }
	    DrawStringCentered(py, cx, BBOX_X+ix*BUTTON_W+BUTTON_W/2,
			BBOX_Y+jx*BUTTON_H+py->lineheight+1);
	    break;
	case 1:
	    DrawStringCentered(py, "Spell List", BBOX_X+ix*BUTTON_W+BUTTON_W/2,
			BBOX_Y+jx*BUTTON_H+py->lineheight+1);
	    break;
	default:
	    break;
    }

}

int in_bbox_box(py, xpos, ypos)
struct player *py;
int xpos, ypos;
{
    return (xpos >= BBOX_X
	    && xpos < BBOX_X + BBOX_W*BUTTON_W
	    && ypos >= BBOX_Y
	    && ypos < BBOX_Y + BBOX_H*BUTTON_H);
}

int button_hit(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int hitx, hity, bnum;
    int ix;

    hitx = (xpos-BBOX_X) / BUTTON_W;
    hity = (ypos-BBOX_Y) / BUTTON_H;

    if (hitx<0 || hity<0 || hitx>=BBOX_W || hity>=BBOX_H)
	return ms_None;

    bnum = hitx + hity*BBOX_W;

    switch (bnum) {
	case 0:
	    if (!py->turn_active)
		return ms_None;
	    light_button(py, bnum);
	    return ms_DoneBtn;
	    break;
	case 1:
	    py->spelllist_rect.w = 620;
	    py->spelllist_rect.h = 420;
	    py->spelllist_rect.x = 10;
	    py->spelllist_rect.y = 10;
	    py->spelllist_way = button;
	    backing_store(py, &py->spelllist_rect);
	    draw_spelllist(py);
	    return ms_SpellBtn;
	    break;
	default:
	    printf("ERROR: unknown button thing!\n");
	    break;
    }
}

int button_motion(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int hitx, hity, bnum;

    switch (py->mousestate) {
	case ms_DoneBtn:
	    hitx = (xpos-BBOX_X+BUTTON_W) / BUTTON_W - 1;
	    hity = (ypos-BBOX_Y+BUTTON_H) / BUTTON_H - 1;
	    if (hitx==0 && hity==0) {
		if (py->button_lit!=0)
		    light_button(py, 0);
	    }
	    else {
		if (py->button_lit!=(-1))
		    light_button(py, (-1));
	    }
	    break;
	case ms_SpellBtn:
	default:
	    break;
    }
}

int button_release(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int ix, isok;

    switch (py->mousestate) {
	case ms_DoneBtn:
	    isok = 1;
	    if (turnstate==State_EQueries || turnstate==State_Queries) {
		for (ix=0; ix<py->siquery.nlines; ix++)
		    if (!py->answers[ix].done) {
			isok = 0;
			strcpy(py->answers[ix].ans_str, "<answer?>");
			/* ### set value to scroll to? */
		    }
	    }
	    if (py->button_lit==0) {
		if (isok) {
		    py->turn_done = !(py->turn_done);
		    redraw_done_markers(py);
		}
		else
		    redraw_queries_only(py, 0);
		light_button(py, (-1));
	    }
	    draw_button(py, 0, 1);
	    break;
	case ms_SpellBtn:
	    backing_restore(py);
	    break;
	default:
	    break;
    }
}
