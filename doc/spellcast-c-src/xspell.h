struct maingame {
    int ignorestuff;
};

struct line {
    int start;
    int len;
    int width;
};

struct rect {
    int x, y, w, h;
};

struct scroll_interface {
    struct player *py;
    int x, y, h;
    int top;
    int visible;
    int nlines;
    int lineheight;
    int grip;
    int moved;
    void (*redraw)();
};

#define MAXQUESTIONLENGTH (256)
#define MAXANSWERLENGTH (64)

struct target_list {
    int num;
    char **tnames;
    int *tnums;
    int size;
};

struct answer {
    struct query *query; /* NULL for just a message */
    int answer;
    int done;
    char qu_str[MAXQUESTIONLENGTH];
    char ans_str[MAXANSWERLENGTH];
    long rock;
};

struct statthing {
    int index, type;
    int hp;
    long stuff;
};

struct player {
    Display *dpy;
    int scn;
    Window win;
    Pixmap backpm;
    GC blackgc, whitegc;
    XFontStruct *font;
    Pixmap gesturebm[2][10];
    Pixmap spelllistbm[2][7];

    int backstore;
    struct rect backrec;
    int gotexpose; /* matters during backing store periods */
    int cursx; /* pixels, lines */
    int textx, texty;
    int lineheight;
    struct scroll_interface sitext, siquery, sistats;
    struct scroll_interface *scroll_active;

    int ascent, totalheight;

    struct line *lineindex;
    int lineindex_size;
    int linesnum;  /* number of complete lines actually in buffer */
    char *linebuf; /* null-terminated lines. The last one (pointed to by
		      lineindex[linesnum].start) is incomplete (or empty),
		      but still null-term. */
    int linebuf_size;
    int addpoint; /* points to '\0' at end of incomplete line */

    int mousestate, mousebutton;
    int button_lit;
    int turn_active, turn_done;
    int turn_blinklimit, turn_blinked;
    int gesture_chosen[2];

    struct rect gesture_rect;
    int gesture_hand, gesture_sel;
    struct rect spelllist_rect;
    int spelllist_way;
    int query_hgt;
    int query_hit;
    int query_sel;
    struct target_list *query_tl;
    struct rect query_rect;
    struct target_list TLRightHand, TLLeftHand;
    struct answer *answers;
    int answers_size;
    struct statthing *statlist;
    int statlist_size;
    char *talk_buf;
    int talk_size;
    int talk_pt, talk_xlast, talk_x, talk_toobig;
};

extern int win_wid, win_hgt;

#define SCROLL_W (16)
#define GEST_SIZE (48)
#define GEST_SMALL (16)
#define FRAME_SHADOW (3)

/* values for py->mousestate */
#define ms_None (0)
#define ms_Gesture (1)
#define ms_DoneBtn (2)
#define ms_SpellBtn (3)
#define ms_Scroll (4)
#define ms_Query (5)
#define ms_Talk (6)

#define EVENTMASK (ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | \
		KeyPressMask | ExposureMask)

extern int numplayers;
extern struct player *players;
extern game *gameval;

#define State_Init (0)
#define State_Top (1)
#define State_EQueries (2) /* Queries before gestures are revealed */
#define State_Queries (3)
#define State_End (4)

#define Qu_SaveTranscript		(-50)

extern int turnstate;
extern int movelist[];
extern int blinklevel;
