#include <stdio.h>
#include <strings.h>

#include "handwave.h"
#include "internal.h"

static char bigbuf[512];

void log_text();

void init_transcript(self)
struct realgame *self;
{
    self->gamelog_size = 4096;
    self->gamelog = (char *)malloc(sizeof(char) * self->gamelog_size);
    strcpy(self->gamelog, "");
    self->gamelog_pos = 0;

    log_text(self, "Spellcast Game Transcript\n\n");
}

void log_text(self, str)
struct realgame *self;
char *str;
{
    int len;

    if (!str)
	return;
    len = strlen(str);
    if (len+self->gamelog_pos >= self->gamelog_size) {
	while (len+self->gamelog_pos >= self->gamelog_size) {
	    self->gamelog_size *= 2;
	}
	self->gamelog = (char *)realloc(self->gamelog, sizeof(char) * self->gamelog_size);
    }

    strcpy(self->gamelog+self->gamelog_pos, str);
    self->gamelog_pos += len;
}

void LogInTranscript(pgame, str)
game *pgame;
char *str;
{
    log_text((struct realgame *)pgame, str);
}

static char cheap_untranslate(val)
int val;
{
    switch (val) {
	case Gesture_PALM:
	    return 'P';
	case Gesture_DIGIT:
	    return 'D';
	case Gesture_FINGERS:
	    return 'F';
	case Gesture_WAVE:
	    return 'W';
	case Gesture_SNAP:
	    return 'S';
	case Gesture_CLAPHALF:
	    return 'C';
	case Gesture_KNIFE:
	    return 'K';
	case Gesture_NOTHING:
	    return '.';
	case Gesture_ANTISPELL:
	    return '=';
	default:
	    return '?';
    }
}

void log_round_header(self)
struct realgame *self;
{
    char *cx;
    int ix, gnum;

    switch (self->turntype) {
	case Turn_TIMESTOP:
	    cx = " (Time Stop)";
	    break;
	case Turn_HASTE:
	    cx = " (Haste)";
	    break;
	default:
	    cx = "";
	    break;
    }

    sprintf(bigbuf, "\n\tTurn %d%s:\n", self->turn, cx);
    log_text(self, bigbuf);

    for (ix=0; ix<self->numplayers; ix++) {
	struct wizard *wiz = self->wiz[ix];
	if (wiz->alive) {
	    if (self->turnactive[ix]) {
		gnum = wiz->numgests-1;
		sprintf(bigbuf, "%s (%d): %c %c\n", wiz->name, wiz->hitpoints,
				cheap_untranslate(wiz->gests[gnum].did[0]),
				cheap_untranslate(wiz->gests[gnum].did[1]));
	    }
	    else {
		sprintf(bigbuf, "%s (%d): [no gestures]\n", wiz->name,
				wiz->hitpoints);
	    }
	    log_text(self, bigbuf);
	}
    }
    for (ix=0; ix<self->numcres; ix++) {
	struct creature *thud = &(self->cre[ix]);
	if (thud->alive) {
	    sprintf(bigbuf, "%s (%d)\n", thud->name, thud->hitpoints);
	    log_text(self, bigbuf);
	}
    }
    log_text(self, "\n");
}

#define ABBREVLEN (8)

void WriteTranscript(pgame, f)
game *pgame;
FILE *f;
{
    struct realgame *self = (struct realgame *)pgame;

    fputs(self->gamelog, f);
}
