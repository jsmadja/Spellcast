#include <stdio.h>
#include "handwave.h"
#include "internal.h"

#define MARGIN_SIDE 72
#define MARGIN_TOP 72
#define LINEHEIGHT 16
#define GEST_SPACE 12
#define TEXT_MARGIN 108

char *psroutine(val, size)
int val;
int *size;
{
    int tmp;
    if (!size)
	size = (&tmp);

    switch (val) {
	case Gesture_PALM:
	    *size = GEST_SPACE;
	    return "showpalm";
	case Gesture_DIGIT:
	    *size = GEST_SPACE;
	    return "showdigit";
	case Gesture_FINGERS:
	    *size = GEST_SPACE;
	    return "showfinger";
	case Gesture_WAVE:
	    *size = GEST_SPACE;
	    return "showwave";
	case Gesture_SNAP:
	    *size = GEST_SPACE;
	    return "showsnap";
	case Gesture_CLAPHALF:
	    *size = GEST_SPACE;
	    return "showclap";
	case Gesture_PALM | Gesture_DOUBLE:
	    *size = 17;
	    return "showpalm2";
	case Gesture_DIGIT | Gesture_DOUBLE:
	    *size = 17;
	    return "showdigit2";
	case Gesture_FINGERS | Gesture_DOUBLE:
	    *size = GEST_SPACE;
	    return "showfinger2";
	case Gesture_WAVE | Gesture_DOUBLE:
	    *size = 17;
	    return "showwave2";
	case Gesture_SNAP | Gesture_DOUBLE:
	    *size = 17;
	    return "showsnap2";
	case Gesture_CLAPHALF | Gesture_DOUBLE:
	    *size = GEST_SPACE;
	    return "showclap2";
	default:
	    *size = GEST_SPACE;
	    return "nullact";
    }
}

void dump_list(topedge, leftedge, filter)
int topedge, leftedge;
int (*filter)();
{
    int ix, gx;
    int xpos, ypos, xval;
    int val, hop;
    char *cx;
    struct spelldef *spel;

    for (ix=0; ix<NUMSPELLS-1; ix++) {
	xpos = leftedge + MARGIN_SIDE;
	ypos = topedge - (ix*LINEHEIGHT);

	xval = xpos;
	if (filter)
	    spel = &(spelllist[(*filter)(ix)]);
	else
	    spel = &(spelllist[ix]);
	for (gx=0; gx<spel->length; gx++) {
	    val = spel->gests[gx];
	    cx = psroutine(val, &hop);
	    printf("%d %d %s\n", xval, ypos-1, cx);
	    xval += hop;
	}
	printf("%d %d moveto  %d %d lineto stroke\n", xval+3, ypos+4,
			xpos+TEXT_MARGIN-4, ypos+4);
	printf("%d %d moveto (%s) show\n", xpos+TEXT_MARGIN, ypos, spel->name);
    }
}

int alphasort(val)
int val;
{
    return alphabet_sort_list[val];
}

main()
{
    printf("/Times-Roman findfont 12 scalefont setfont\n");
    printf("0.5 setlinewidth\n");

    dump_list(772 - MARGIN_TOP, 0, NULL);
    dump_list(772 - MARGIN_TOP, 264, alphasort);

    printf("/Times-Bold findfont 14 scalefont setfont\n");
    printf("306 752 moveto\n");
    printf("(SpellCast spell lists -- sorted by gesture and name) centershow\n");
    printf("\nshowpage\n");
}
