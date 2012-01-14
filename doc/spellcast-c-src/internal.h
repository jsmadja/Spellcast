#include "spelllist.h"

#define PrintMsg(msg)   (log_text(self, msg),\
			self->callbacks.proc_PrintMsg)((msg), self,\
			(self->rock))
#define PrintMsg2(person, msg1, msgelse)   (log_text(self, msgelse),\
			self->callbacks.proc_PrintMsg2)((person), (msg1),\
			(msgelse), self, (self->rock))
#define PrintMsg3(person1, person2, msg1, msg2, msgelse)   (log_text(self, msgelse),\
			self->callbacks.proc_PrintMsg3)((person1), (person2),\
			(msg1), (msg2), (msgelse), self, (self->rock))
#define Queries(numqueries, querylist)   (setup_targetlist(self),\
			(self->callbacks.proc_Queries)((numqueries),\
			(querylist), self, (self->rock)))

#define MAXSPELLCHOICES (16)	/* max number of spells that could possibly be
				invoked with one gesture */

struct target {
    char *name;
    int index;
    long stuff;
};

struct realgame {
    struct interface callbacks;
    char *rock;
    int numplayers;
    int turn;
    int turntype;
    int turnactive[MAXPLAYERS];

    char *gamelog;
    int gamelog_pos;
    int gamelog_size;

    struct wizard *wiz[MAXPLAYERS];
    struct creature *cre;
    int numcres, cre_size;

    struct query *querylist;
    int querylist_size;
    int numqueries;

    int numtargets[3];
    struct target *targetlist[3];
    int targetlist_size[3];

    struct castspell *castlist, *hastelist;

    /* per-round data */
    int fl_icestorm, fl_firestorm, fl_dispelmagic;
    int blind_array;
};

struct wizgesture {
    int did[2];
    int invisible; /* was invisible for this move? */
    int blind; /* bit-array -- who was blind this move? */
    int turnnum, log_hp;
};

struct permstats {
    int mind_spell, mind_detail;
    int fl_haste;
    int fl_prot_evil;
    int fl_resist_heat;
    int fl_resist_cold;
    int fl_blindness;
    int fl_invisibility;
};

struct wizard {
    int alive;
    int hitpoints, max_hitpoints;
    int resistant_heat, resistant_cold, prot_from_evil, haste, timestop;
    int invisibility, blindness;
    int poison_time, disease_time;
    int mind_spell, mind_caster;
    struct permstats perm;
    char *name;
    int gender;
    /* per-round */
    int zaplist[NUMSPELLS];
    int enchant_caster, raisedead_caster, enchant_ppend;
    int fl_resist_heat, fl_resist_cold;
    int fl_resist_icestorm; /* due to fireball hits */

    int numgests;
    int gests_size;
    struct wizgesture *gests;
    int surrendered;
    int fl_cast_lightning;
    int hand_paralyzed;
    int perm_time, delay_time;
    int delay_bank;

    int foundlist[NUMSPELLS];
    int llist[MAXSPELLCHOICES], rlist[MAXSPELLCHOICES];
};

struct creature {
    int alive;
    int hitpoints, max_hitpoints;
    int resistant_heat, resistant_cold, prot_from_evil, haste, timestop;
    int invisibility, blindness;
    int poison_time, disease_time;
    int mind_spell, mind_caster;
    struct permstats perm;
    char *name;
    int gender;
    /* per-round */
    int zaplist[NUMSPELLS];
    int enchant_caster, raisedead_caster, enchant_ppend;
    int fl_resist_heat, fl_resist_cold;
    int fl_resist_icestorm; /* due to fireball hits */

    int type;
    int nocorpse;
    int owner;
    int last_target, last_targettype;
    int nowm_spell, nowm_caster;
};

union being {
    struct {
	int alive;
	int hitpoints, max_hitpoints;
	int resistant_heat, resistant_cold, prot_from_evil, haste, timestop;
	int invisibility, blindness;
	int poison_time, disease_time;
	int mind_spell, mind_caster;
	struct permstats perm;
	char *name;
	int gender;
	/* per-round */
	int zaplist[NUMSPELLS];
	int enchant_caster, raisedead_caster, enchant_ppend;
	int fl_resist_heat, fl_resist_cold;
	int fl_resist_icestorm; /* due to fireball hits */
    } both;

    struct wizard wiz;
    struct creature cre;
};


#define MASK_LEFT (1)
#define MASK_RIGHT (2)
#define MASK_TWOHAND (4)

struct castspell {
    int caster;
    int handage;
    int spellnum;
    int target;
    int targettype; /* QuVal_Target_*, or 0 for nobody, or (-1) for no target required */
    int permanent;
    int rock;

    struct castspell *next;
};

#define Creature_GOBLIN (1)
#define Creature_OGRE   (2)
#define Creature_TROLL  (3)
#define Creature_GIANT  (4)
#define Creature_ICEL   (5)
#define Creature_FIREL  (6)

extern void setup_targetlist(); /* needed if Queries() is to be used */
