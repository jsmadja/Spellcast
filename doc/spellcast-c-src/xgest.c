#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "handwave.h"
#include "spelllist.h"
#include "xspell.h"

#include "ba.bm"
#include "bc.bm"
#include "bd.bm"
#include "bf.bm"
#include "bk.bm"
#include "bn.bm"
#include "bp.bm"
#include "bs.bm"
#include "bu.bm"
#include "bw.bm"
#include "bva.bm"
#include "bvc.bm"
#include "bvd.bm"
#include "bvf.bm"
#include "bvk.bm"
#include "bvp.bm"
#include "bvs.bm"
#include "bvw.bm"

#include "sc.bm"
#include "sd.bm"
#include "sd2.bm"
#include "sf.bm"
#include "sf2.bm"
#include "sp.bm"
#include "sp2.bm"
#include "ss.bm"
#include "ss2.bm"
#include "sw.bm"
#include "sw2.bm"
#include "sdot.bm"

#define COLUMN_SIZE (8)
#define COLUMN_X (476)
#define COLUMN_Y (33)
#define GEST_SPACE (50)

void init_bitmaps(py)
struct player *py;
{
    py->gesturebm[0][0] = XCreatePixmapFromBitmapData(py->dpy, py->win, bn_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][1] = XCreatePixmapFromBitmapData(py->dpy, py->win, bp_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][2] = XCreatePixmapFromBitmapData(py->dpy, py->win, bd_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][3] = XCreatePixmapFromBitmapData(py->dpy, py->win, bf_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][4] = XCreatePixmapFromBitmapData(py->dpy, py->win, bw_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][5] = XCreatePixmapFromBitmapData(py->dpy, py->win, bc_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][6] = XCreatePixmapFromBitmapData(py->dpy, py->win, bs_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][7] = XCreatePixmapFromBitmapData(py->dpy, py->win, bk_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][8] = XCreatePixmapFromBitmapData(py->dpy, py->win, ba_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[0][9] = XCreatePixmapFromBitmapData(py->dpy, py->win, bu_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);

    py->gesturebm[1][0] = XCreatePixmapFromBitmapData(py->dpy, py->win, bn_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][1] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvp_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][2] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvd_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][3] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvf_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][4] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvw_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][5] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvc_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][6] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvs_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][7] = XCreatePixmapFromBitmapData(py->dpy, py->win, bvk_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][8] = XCreatePixmapFromBitmapData(py->dpy, py->win, bva_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);
    py->gesturebm[1][9] = XCreatePixmapFromBitmapData(py->dpy, py->win, bu_bits,
		GEST_SIZE, GEST_SIZE, 1, 0, 1);

    py->spelllistbm[0][0] = XCreatePixmapFromBitmapData(py->dpy, py->win, sp_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[0][1] = XCreatePixmapFromBitmapData(py->dpy, py->win, sd_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[0][2] = XCreatePixmapFromBitmapData(py->dpy, py->win, sf_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[0][3] = XCreatePixmapFromBitmapData(py->dpy, py->win, sw_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[0][4] = 0; /* there is no single clap */
    py->spelllistbm[0][5] = XCreatePixmapFromBitmapData(py->dpy, py->win, ss_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[0][6] = XCreatePixmapFromBitmapData(py->dpy, py->win, sdot_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);

    py->spelllistbm[1][0] = XCreatePixmapFromBitmapData(py->dpy, py->win, sp2_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[1][1] = XCreatePixmapFromBitmapData(py->dpy, py->win, sd2_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[1][2] = XCreatePixmapFromBitmapData(py->dpy, py->win, sf2_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[1][3] = XCreatePixmapFromBitmapData(py->dpy, py->win, sw2_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[1][4] = XCreatePixmapFromBitmapData(py->dpy, py->win, sc_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[1][5] = XCreatePixmapFromBitmapData(py->dpy, py->win, ss2_bits,
		GEST_SMALL, GEST_SMALL, 1, 0, 1);
    py->spelllistbm[1][6] = 0; /* no double dots */

}

void redraw_column(py)
struct player *py;
{
    int buf[COLUMN_SIZE+1];
    int ix, gx;
    int vall, valr;
    int posx, posy;
    int pnum = py-players;

    if (turnstate==State_Init) {
	return;
    }

    for (ix=0; ix<numplayers; ix++) {
	posx = COLUMN_X + ix*(2*GEST_SIZE + 20);
	posy = COLUMN_Y;

	DrawStringCentered(py, NameOfBeing(gameval, QuVal_Target_Wizard, ix),
			posx+GEST_SIZE, posy-py->lineheight+py->ascent);

	if (turnstate!=State_Top) {
	    SeeGesture(gameval, ix, pnum, buf, COLUMN_SIZE);
	    gx=0;
	    if (turnstate==State_EQueries && ix!=pnum) {
		buf[0] = (Gesture_UNCLEAR<<5) + Gesture_UNCLEAR;
	    }
	}
	else {
	    SeeGesture(gameval, ix, pnum, buf+1, COLUMN_SIZE);
	    gx=1;
	    if (ix==pnum) {
		vall = py->gesture_chosen[0];
		valr = py->gesture_chosen[1];
	    }
	    else {
		vall = Gesture_UNCLEAR;
		valr = Gesture_UNCLEAR;
	    }
	    XCopyPlane(py->dpy, py->gesturebm[0][vall], py->win, py->blackgc,
			0, 0, GEST_SIZE, GEST_SIZE, posx,
			posy+(GEST_SPACE)*(COLUMN_SIZE-1), 1);
	    XCopyPlane(py->dpy, py->gesturebm[1][valr], py->win, py->blackgc,
			0, 0, GEST_SIZE, GEST_SIZE, posx+GEST_SPACE,
			posy+(GEST_SPACE)*(COLUMN_SIZE-1), 1);
	}
	for (/* gx is set */; gx<COLUMN_SIZE; gx++) {
	    if (buf[gx]==(-1)) {
		valr = Gesture_NOTHING;
		vall = Gesture_NOTHING;
	    }
	    else {
		valr = buf[gx] >> 5;
		vall = buf[gx] & 31; /* 31==01111 */
	    }
	    XCopyPlane(py->dpy, py->gesturebm[0][vall], py->win, py->blackgc,
			0, 0, GEST_SIZE, GEST_SIZE, posx,
			posy+(GEST_SPACE)*(COLUMN_SIZE-(gx+1)), 1);
	    XCopyPlane(py->dpy, py->gesturebm[1][valr], py->win, py->blackgc,
			0, 0, GEST_SIZE, GEST_SIZE, posx+GEST_SPACE,
			posy+(GEST_SPACE)*(COLUMN_SIZE-(gx+1)), 1);
	}
    }
}

void draw_frame(py, mrec)
struct player *py;
struct rect *mrec;
{
    XClearArea(py->dpy, py->win, mrec->x, mrec->y, mrec->w, mrec->h, 0);

    XDrawRectangle(py->dpy, py->win, py->blackgc, mrec->x, mrec->y,
		mrec->w-FRAME_SHADOW, mrec->h-FRAME_SHADOW);
    XDrawRectangle(py->dpy, py->win, py->blackgc, mrec->x+1, mrec->y+1,
		mrec->w-FRAME_SHADOW-2, mrec->h-FRAME_SHADOW-2);
    XFillRectangle(py->dpy, py->win, py->blackgc, mrec->x+mrec->w-FRAME_SHADOW,
		mrec->y+FRAME_SHADOW, FRAME_SHADOW, mrec->h-FRAME_SHADOW);
    XFillRectangle(py->dpy, py->win, py->blackgc, mrec->x+FRAME_SHADOW,
		mrec->y+mrec->h-FRAME_SHADOW, mrec->w-FRAME_SHADOW, FRAME_SHADOW);
}

void draw_gesture_menu(py)
struct player *py;
{
    static char gesture_prompt[2][128];
    int ix, jx;
    int posx, posy;

    draw_frame(py, &py->gesture_rect);
    sprintf(gesture_prompt[0], "What gesture do you want to");
    sprintf(gesture_prompt[1], "make with your %s hand?",
		py->gesture_hand?"right":"left");
    XDrawImageString(py->dpy, py->win, py->blackgc, py->gesture_rect.x+20,
		py->gesture_rect.y+10+py->lineheight, gesture_prompt[0],
		strlen(gesture_prompt[0]));
    XDrawImageString(py->dpy, py->win, py->blackgc, py->gesture_rect.x+20,
		py->gesture_rect.y+10+2*py->lineheight, gesture_prompt[1],
		strlen(gesture_prompt[1]));

    posx = py->gesture_rect.x+20;
    posy = py->gesture_rect.y+60;
    for (jx=0; jx<2; jx++)
	for (ix=0; ix<4; ix++) {
	    XCopyPlane(py->dpy, py->gesturebm[py->gesture_hand][jx*4+ix],
			py->win, py->blackgc, 0, 0, GEST_SIZE, GEST_SIZE,
			posx+ix*GEST_SPACE, posy+jx*GEST_SPACE, 1);
	}
}

int in_gesture_box(py, xpos, ypos)
struct player *py;
int xpos, ypos;
{
    return (xpos >= COLUMN_X
	    && xpos < COLUMN_X + numplayers*(2*GEST_SIZE + 20)
	    && ypos >= COLUMN_Y
	    && ypos < COLUMN_Y + GEST_SPACE*COLUMN_SIZE);
}

int gesture_hit(py, xpos, ypos, button)
struct player *py;
int xpos, ypos; /* relative to column pos */
int button;
{
    int pnum = py-players;
    int hitcol, hitrow, hithand;

    xpos -= COLUMN_X;
    ypos -= COLUMN_Y;

    if (!py->turn_active || py->turn_done)
	return ms_None;

    hitcol = xpos / (2*GEST_SIZE + 20);
    hitrow = ypos / GEST_SPACE;
    hithand = (xpos % (2*GEST_SIZE + 20)) / GEST_SPACE;
    if (hithand < 0 || hithand > 1)
	return ms_None;

    if (hitrow==COLUMN_SIZE-1 && hitcol==pnum && button==1 && turnstate==State_Top) {
	py->gesture_rect.w = 240;
	py->gesture_rect.h = 180;
	py->gesture_rect.x = xpos + COLUMN_X - py->gesture_rect.w/2;
	py->gesture_rect.y = ypos + COLUMN_Y - py->gesture_rect.h/2;
	py->gesture_hand = hithand;
	py->gesture_sel = (-1);
	adjust_rect(&py->gesture_rect);
	backing_store(py, &py->gesture_rect);
	draw_gesture_menu(py);
	gesture_motion(py, xpos+COLUMN_X, ypos+COLUMN_Y, button);
	return ms_Gesture;
    }

    return ms_None;
}

int gesture_motion(py, xpos, ypos, button)
struct player *py;
int xpos, ypos;
int button;
{
    int hitx, hity, hit;
    int posx, posy;

    posx = py->gesture_rect.x+20;
    posy = py->gesture_rect.y+60;

    hitx = (xpos - posx + GEST_SPACE) / GEST_SPACE - 1;
    hity = (ypos - posy + GEST_SPACE) / GEST_SPACE - 1;

    if (hitx>=0 && hitx<4 && hity>=0 && hity<2)
	hit = hity*4 + hitx;
    else
	hit = (-1);

    if (hit==py->gesture_sel)
	return;

    if (py->gesture_sel != (-1)) {
	/* erase old */
	hitx = py->gesture_sel % 4;
	hity = py->gesture_sel / 4;
	XDrawRectangle(py->dpy, py->win, py->whitegc, posx+hitx*GEST_SPACE-1,
			posy+hity*GEST_SPACE-1, GEST_SIZE+1, GEST_SIZE+1);
	XDrawRectangle(py->dpy, py->win, py->whitegc, posx+hitx*GEST_SPACE-2,
			posy+hity*GEST_SPACE-2, GEST_SIZE+3, GEST_SIZE+3);
    }

    py->gesture_sel = hit;

    if (py->gesture_sel != (-1)) {
	/* draw new */
	hitx = py->gesture_sel % 4;
	hity = py->gesture_sel / 4;
	XDrawRectangle(py->dpy, py->win, py->blackgc, posx+hitx*GEST_SPACE-1,
			posy+hity*GEST_SPACE-1, GEST_SIZE+1, GEST_SIZE+1);
	XDrawRectangle(py->dpy, py->win, py->blackgc, posx+hitx*GEST_SPACE-2,
			posy+hity*GEST_SPACE-2, GEST_SIZE+3, GEST_SIZE+3);
    }
}

int gesture_release(py, xpos, ypos, button)
struct player *py;
int xpos, ypos; /* relative to column pos */
int button;
{
    int posx, posy;
    int pnum = py-players;

    xpos -= COLUMN_X;
    ypos -= COLUMN_Y;

    backing_restore(py);
    if (py->gesture_sel != (-1))
	py->gesture_chosen[py->gesture_hand] = py->gesture_sel;

    posx = COLUMN_X + pnum*(2*GEST_SIZE + 20);
    posy = COLUMN_Y;
    XCopyPlane(py->dpy, py->gesturebm[0][py->gesture_chosen[0]], py->win,
		py->blackgc, 0, 0, GEST_SIZE, GEST_SIZE, posx,
		posy+(GEST_SPACE)*(COLUMN_SIZE-1), 1);
    XCopyPlane(py->dpy, py->gesturebm[1][py->gesture_chosen[1]], py->win,
		py->blackgc, 0, 0, GEST_SIZE, GEST_SIZE, posx+GEST_SPACE,
		posy+(GEST_SPACE)*(COLUMN_SIZE-1), 1);
}
