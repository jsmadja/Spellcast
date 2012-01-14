#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "handwave.h"
#include "spelllist.h"
#include "xspell.h"

#define TEXT_WID (400)
#define TEXT_HGT (512)
#define TEXT_X (32)
#define TEXT_Y (32)

#define MAXLINESIZE (512) /* maximum number of chars that could possibly fit
			     in a line. If the player uses bits1, this will
			     probably crash. Tough. */

static void redraw_text_only();
void redraw_scrollbar();

void init_xtext(pnum)
int pnum;
{
    struct player *py = &(players[pnum]);
    static XCharStruct overall;
    int direction, ascent, descent;

    py->cursx = 0;
    py->sitext.nlines = 0;

    XTextExtents(py->font, "pl", 2, &direction, &ascent, &descent, &overall);
    py->lineheight = ascent+descent;
    if (py->lineheight <= 12)
	py->lineheight += 2;
    else
	py->lineheight = (int)((double)(py->lineheight)*1.17);

    py->texty = TEXT_Y + ascent + 1;
    py->textx = TEXT_X;
    py->sitext.visible = (TEXT_HGT-2) / py->lineheight;

    py->ascent = ascent;
    py->totalheight = ascent+descent;
    
    py->lineindex_size = 10;
    py->lineindex = (struct line *)malloc(sizeof(struct line)*py->lineindex_size);
    py->linebuf_size = 2*MAXLINESIZE;
    py->linebuf = (char *)malloc(py->linebuf_size);
    strcpy(py->linebuf, "");
    py->addpoint = 0;
    py->lineindex[0].start = 0;

    py->sitext.py = py;
    py->sitext.top = 0;
    py->sitext.lineheight = py->lineheight;
    py->sitext.x = TEXT_X;
    py->sitext.y = TEXT_Y;
    py->sitext.h = TEXT_HGT;
    py->sitext.redraw = redraw_text_only;
}

static void redraw_text_only(py, baronly)
struct player *py;
int baronly;
{
    int ix, sline;
    int len, width;

    if (!baronly) {
	/* draw text */
	for (ix=py->sitext.top; ix<=py->sitext.nlines; ix++) {
	    sline = ix - py->sitext.top;
	    if (sline>=py->sitext.visible)
		break;
	    if (ix==py->sitext.nlines) {
		len = strlen(py->linebuf+py->lineindex[ix].start);
	    }
	    else {
		len = py->lineindex[ix].len;
	    }
	    width = py->lineindex[ix].width;
	    XDrawImageString(py->dpy, py->win, py->blackgc, py->textx,
			py->texty + py->lineheight*sline,
			py->linebuf+py->lineindex[ix].start, len);
	    XClearArea(py->dpy, py->win, py->textx+width, py->texty +
			py->lineheight*sline - py->ascent, TEXT_WID-width,
			py->totalheight, 0);
	}
    }

    redraw_scrollbar(&(py->sitext));
}

void complete_line(py)
struct player *py;
{
    int ln = py->sitext.nlines;
    int jump;
    static XCharStruct overall;
    int direction, ascent, descent;
    char *start;

    if (ln+2 >= py->lineindex_size) {
	py->lineindex_size *= 2;
	py->lineindex = (struct line *)realloc(py->lineindex,
			sizeof(struct line)*py->lineindex_size);
    }
    start = py->linebuf+py->lineindex[ln].start;
    py->lineindex[ln].len = strlen(start);
    XTextExtents(py->font, start, py->lineindex[ln].len, &direction, &ascent,
		&descent, &overall);
    py->lineindex[ln].width = overall.width;

    /* set up new incomplete line */
    py->lineindex[ln+1].start = py->lineindex[ln].start+py->lineindex[ln].len + 1;
    /* len will not be computed until line is finished */
    py->lineindex[ln+1].width = 0;
    start = NULL;
    if (py->lineindex[ln+1].start+MAXLINESIZE >= py->linebuf_size) {
	py->linebuf_size *= 2;
	py->linebuf = (char *)realloc(py->linebuf, py->linebuf_size);
    }
    strcpy(py->linebuf+py->lineindex[ln+1].start, "");
    py->addpoint = py->lineindex[ln+1].start;

    py->cursx = 0;
    py->sitext.nlines++;

    if (py->sitext.nlines-py->sitext.top == py->sitext.visible) {
	jump = ((py->sitext.nlines-py->sitext.top) - py->sitext.visible) + 1;
	py->sitext.top += jump;
	if (!py->backstore)
	    redraw_text_only(py, 0);
    }
    else {
	if (!py->backstore)
	    redraw_text_only(py, 1);
    }
}

void redraw_text(py)
struct player *py;
{
    XDrawRectangle(py->dpy, py->win, py->blackgc, TEXT_X-3, TEXT_Y-2,
		TEXT_WID+4, TEXT_HGT+3);
    XDrawRectangle(py->dpy, py->win, py->blackgc, TEXT_X-SCROLL_W, TEXT_Y-2,
		SCROLL_W-3, TEXT_HGT+3);
    redraw_text_only(py, 0);
}

void dump_text(pnum, str)
int pnum;
char *str;
{
    struct player *py = &(players[pnum]);
    char *cx, *cy;
    int tmp;
    char *chtmp;
    int len, width, sline;
    static XCharStruct overall;
    int direction, ascent, descent;

    if (!str) return;

    cx = str;

    while (*cx) {
	for (cy=cx;
	     !(*cy=='\0' || *cy=='\n' || (*cy==' ' && cy>cx && *(cy-1)!=' '));
	     cy++);
	len = (cy-cx);
	XTextExtents(py->font, cx, len, &direction, &ascent, &descent, &overall);
	width = overall.width;
	if (width >= TEXT_WID - py->cursx) {
	    complete_line(py);
	}
	sline = py->sitext.nlines - py->sitext.top;
	if (sline>=0 && sline<py->sitext.visible) {
	    py->gotexpose = 1;
	    if (!py->backstore)
		XDrawImageString(py->dpy, py->win, py->blackgc, py->textx+py->cursx,
				py->texty + py->lineheight*sline, cx, len);
	}
	chtmp = py->linebuf+py->addpoint;
	for (tmp=0; tmp<len; tmp++)
	    chtmp[tmp] = cx[tmp];
	chtmp[tmp] = '\0';
	py->addpoint += len;
	py->cursx += width;
	py->lineindex[py->sitext.nlines].width = py->cursx;

	if (*cy=='\n') {
	    complete_line(py);
	    cy++;
	}

	cx = cy;
    }
}

int in_scrollbar_box(si, xpos, ypos)
struct scroll_interface *si;
int xpos, ypos;
{
    return (xpos >= si->x-SCROLL_W
	    && xpos < si->x
	    && ypos >= si->y
	    && ypos < si->y+si->h);
}

void redraw_scrollbar(si)
struct scroll_interface *si;
{
    struct player *py = si->py;
    int sbeg, send;

    /* draw scrollbar */
    if (si->nlines) {
	sbeg = (si->top)*si->h / si->nlines;
	send = (si->top+si->visible)*si->h / si->nlines;
	if (send>si->h)
	    send = si->h;
    }
    else {
	sbeg = 0;
	send = si->h;
    }

    if (sbeg>0) {
	XFillRectangle(py->dpy, py->win, py->whitegc, si->x-SCROLL_W+2, si->y,
			SCROLL_W-6, sbeg);
    }
    XFillRectangle(py->dpy, py->win, py->blackgc, si->x-SCROLL_W+2, si->y+sbeg,
		SCROLL_W-6, send-sbeg);
    if (send<si->h) {
	XFillRectangle(py->dpy, py->win, py->whitegc, si->x-SCROLL_W+2,
			si->y+send, SCROLL_W-6, si->h-send);
    }
}

int scrollbar_hit(si, xpos, ypos, button)
struct scroll_interface *si;
int xpos, ypos;
int button;
{
    int sbeg, send;

    if (si->nlines) {
	sbeg = (si->top)*si->h / si->nlines;
	send = (si->top+si->visible)*si->h / si->nlines;
	if (send>si->h)
	    send = si->h;
    }
    else {
	sbeg = 0;
	send = si->h;
    }

    if (ypos>=si->y+sbeg && ypos<si->y+send) {
	si->grip = ypos - (si->y+sbeg);
    }
    else {
	si->grip = (-1);
    }
    si->moved = 0;

    si->py->scroll_active = si;
    return ms_Scroll;
}

int scrollbar_motion(si, xpos, ypos, button)
struct scroll_interface *si;
int xpos, ypos;
int button;
{
    int newtop;

    si->moved = 1;

    if (si->grip != (-1)) {
	newtop = (ypos-si->grip-si->y)*si->nlines / si->h;
	if (newtop > si->nlines - si->visible + 1)
	    newtop = si->nlines - si->visible + 1;
	if (newtop<0)
	    newtop = 0;

	if (si->top != newtop) {
	    si->top = newtop;
	    (*(si->redraw))(si->py, 0);
	}
    }
}

int scrollbar_release(si, xpos, ypos, button)
struct scroll_interface *si;
int xpos, ypos;
int button;
{
    int ln;
    int newtop;

    if (!si->moved) {
	ln = (ypos-si->y) / si->lineheight;
	if (ln<=0)
	    ln = 1;
	newtop = si->top;
	if (button==1)
	    newtop += (ln);
	else if (button==3)
	    newtop -= (ln);

	if (newtop > si->nlines - si->visible + 1)
	    newtop = si->nlines - si->visible + 1;
	if (newtop<0)
	    newtop = 0;

	if (si->top != newtop) {
	    si->top = newtop;
	    (*(si->redraw))(si->py, 0);
	}
    }
}
