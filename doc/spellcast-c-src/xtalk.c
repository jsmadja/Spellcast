#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "handwave.h"
#include "spelllist.h"
#include "xspell.h"

#define TALK_X (348)
#define TALK_Y (576)
#define TALK_W (448)
#define TALK_LEADING (8)

static char bigbuf[2048], bigbuf2[2048];

void init_talk(py)
struct player *py;
{
    py->talk_size = 128;
    py->talk_buf = (char *)malloc(sizeof(char) * py->talk_size);
    strcpy(py->talk_buf, "");
    py->talk_pt = 0;
    py->talk_xlast = (-1);
    py->talk_x = 0;
    py->talk_toobig = 0;
}

void redraw_talk_only(py, recomp)
struct player *py;
int recomp;
{
    static XCharStruct overall;
    int direction, ascent, descent;
    int width;

    static XPoint caret[3] = {
	{0, 0}, {2, -3}, {2, 3} 
    };

    if (recomp) {
	XTextExtents(py->font, py->talk_buf, py->talk_pt, &direction, &ascent,
			&descent, &overall);
	py->talk_x = overall.width;
    }

    if (py->talk_x != py->talk_xlast) {
	caret[0].x = TALK_X+py->talk_xlast - 2;
	caret[0].y = TALK_Y+py->lineheight + 4;
	XDrawLines(py->dpy, py->win, py->whitegc, caret, 3, CoordModePrevious);
	py->talk_xlast = py->talk_x;
    }

    width = DrawStringField(py, py->talk_buf, TALK_X, TALK_Y+py->lineheight, TALK_W);

    caret[0].x = TALK_X+py->talk_x - 2;
    caret[0].y = TALK_Y+py->lineheight + 4;
    XDrawLines(py->dpy, py->win, py->blackgc, caret, 3, CoordModePrevious);

    py->talk_toobig = (width >= (TALK_W-py->lineheight));
}

void redraw_talk(py)
struct player *py;
{
    XDrawRectangle(py->dpy, py->win, py->blackgc, TALK_X-3, TALK_Y-2, TALK_W+6,
		py->lineheight+TALK_LEADING+4);

    redraw_talk_only(py, 0);
}

static void ensure_talk_size(py, size)
struct player *py;
int size;
{
    if (size < py->talk_size)
	return;

    while (size < py->talk_size)
	py->talk_size *= 2;

    py->talk_buf = (char *)realloc(py->talk_buf, sizeof(char) * py->talk_size);
}

void talk_handle_char(py, ch)
struct player *py;
char ch;
{
    char *cp;
    int ix;
    int pnum;

    if (isprint(ch)) {
	if (!py->talk_toobig) {
	    ensure_talk_size(py, strlen(py->talk_buf)+1);
	    for (cp = &(py->talk_buf[py->talk_pt]); *cp; cp++);
	    for (; cp >= &(py->talk_buf[py->talk_pt]); cp--)
		*(cp+1) = *cp;
	    py->talk_buf[py->talk_pt] = ch;
	    py->talk_pt++;
	    redraw_talk_only(py, 1);
	}
    }
    else
	switch (ch) {
	    case '\n':
	    case '\r':
	    case '\023':
		if (py->talk_pt) {
		    pnum = py-players;
		    sprintf(bigbuf, "You say \"%s\"\n", py->talk_buf);
		    sprintf(bigbuf2, "%s says \"%s\"\n", NameOfBeing(gameval,
				QuVal_Target_Wizard, pnum), py->talk_buf);
		    for (ix=0; ix<numplayers; ix++)
			if (ix==pnum)
			    dump_text(ix, bigbuf);
			else
			    dump_text(ix, bigbuf2);
		    LogInTranscript(gameval, bigbuf2);
		}
		py->talk_pt = 0;
		py->talk_buf[0] = '\0';
		redraw_talk_only(py, 1);
		break;
	    case '\177':
	    case '\010':
		if (py->talk_pt) {
		    py->talk_pt--;
		    for (cp = &(py->talk_buf[py->talk_pt]); *cp; cp++)
			*cp = *(cp+1);
		    redraw_talk_only(py, 1);
		}
		break;
	    case '\013':
		py->talk_buf[py->talk_pt] = '\0';
		redraw_talk_only(py, 1);
		break;
	    case '\002':
		if (py->talk_pt) {
		    py->talk_pt--;
		    redraw_talk_only(py, 1);
		}
		break;
	    case '\006':
		if (py->talk_buf[py->talk_pt]) {
		    py->talk_pt++;
		    redraw_talk_only(py, 1);
		}
		break;
	    case '\001':
		py->talk_pt = 0;
		redraw_talk_only(py, 1);
		break;
	    case '\005':
		while (py->talk_buf[py->talk_pt])
		    py->talk_pt++;
		redraw_talk_only(py, 1);
		break;
	}
}
