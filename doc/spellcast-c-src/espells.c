#include <strings.h>

#include "handwave.h"
#include "internal.h"

/* the following are purely for readability */
#define P Gesture_PALM
#define D Gesture_DIGIT
#define F Gesture_FINGERS
#define W Gesture_WAVE
#define C Gesture_CLAPHALF
#define S Gesture_SNAP
#define P2 (Gesture_PALM | Gesture_DOUBLE)
#define D2 (Gesture_DIGIT | Gesture_DOUBLE)
#define F2 (Gesture_FINGERS | Gesture_DOUBLE)
#define W2 (Gesture_WAVE | Gesture_DOUBLE)
#define C2 (Gesture_CLAPHALF | Gesture_DOUBLE)
#define S2 (Gesture_SNAP | Gesture_DOUBLE)

struct spelldef spelllist[NUMSPELLS] = {
    {4, {C2, D,  P,  W},	0, 1, "Dispel Magic"},
    {5, {C2, S,  W,  W,  S},	0, 1, "Summon Elemental"},
    {2, {C2, W2},		0, 1, "Magic Mirror"},
    {5, {D,  F,  F,  D,  D},	0, 0, "Lightning Bolt"},
    {4, {D,  F,  P,  W},	0, 1, "Cure Heavy Wounds"},
    {3, {D,  F,  W},		0, 1, "Cure Light Wounds"},
    {3, {D,  P,  P},		1, 0, "Amnesia"},
    {3, {D,  S,  F},		1, 0, "Confusion"},
    {6, {D,  S,  F,  F,  F,  C2},0, 0,"Disease"},
    {5, {D,  W,  F,  F,  D2},	1, 0, "Blindness"},
    {6, {D,  W,  S,  S,  S,  P},0, 1, "Delayed Effect"},
    {6, {D,  W,  W,  F,  W,  C2},0, 1,"Raise Dead"},
    {6, {D,  W,  W,  F,  W,  D},0, 0, "Poison"},
    {3, {F,  F,  F},		1, 0, "Paralysis"},
    {5, {F,  P,  S,  F,  W},	0, 1, "Summon Troll"},
    {5, {F,  S,  S,  D,  D},	0, 0, "Fireball"},
    {1, {P},			0, 1, "Shield"},
    {1, {P2},			0, 0, "SURRENDER"},
    {4, {P,  D,  W,  P},	0, 1, "Remove Enchantment"},
    {4, {P,  P,  W2, S2},	1, 1, "Invisibility"},
    {4, {P,  S,  D,  D},	1, 0, "Charm Monster"},
    {4, {P,  S,  D,  F},	1, 0, "Charm Person"},
    {4, {P,  S,  F,  W},	0, 1, "Summon Ogre"},
    {8, {P,  W,  P,  F,  S,  S,  S,  D},0,0,"Finger of Death"},
    {6, {P,  W,  P,  W,  W,  C2},1, 1,"Haste"},
    {2, {S,  D},		0, 0, "Missile"},
    {3, {S,  F,  W},		0, 1, "Summon Goblin"},
    {3, {S,  P,  F},		0, 0, "Anti-Spell"},
    {7, {S,  P,  F,  P,  S,  D,  W},0,1,"Permanency"},
    {4, {S,  P,  P,  C2},	0, 1, "Time Stop"},
    {4, {S,  S,  F,  P},	1, 1, "Resist Cold"},
    {3, {S,  W,  D},		1, 0, "Fear"},
    {4, {S,  W,  W,  C2},	0, 0, "Fire Storm"},
    {4, {W,  D,  D,  C2},	0, 0, "Lightning Bolt"},
    {3, {W,  F,  P},		0, 0, "Cause Light Wounds"},
    {6, {W,  F,  P,  S,  F,  W},0, 1, "Summon Giant"},
    {4, {W,  P,  F,  D},	0, 0, "Cause Heavy Wounds"},
    {3, {W,  P,  P},		0, 0, "Counter-Spell"},
    {4, {W,  S,  S,  C2},	0, 0, "Ice Storm"},
    {4, {W,  W,  F,  P},	1, 1, "Resist Heat"},
    {3, {W,  W,  P},		1, 1, "Protection From Evil"},
    {3, {W,  W,  S},		0, 0, "Counter-Spell"},
    {1,	{Gesture_KNIFE},	0, 0, "stab"}
};

#undef P
#undef D
#undef F
#undef W
#undef C
#undef S
#undef P2
#undef D2
#undef F2
#undef W2
#undef C2
#undef S2

int reverse_sort_list[NUMSPELLS] = {
    SP__LIGHTNING_BOLT2, SP__DISEASE, SP__TIME_STOP, SP__ICE_STORM, SP__RAISE_DEAD,
    SP__HASTE, SP__FIRE_STORM, SP__LIGHTNING_BOLT, SP__CHARM_MONSTER, SP__FIREBALL,
    SP__BLINDNESS, SP__CAUSE_HEAVY_WOUNDS, SP__MISSILE, SP__FINGER_OF_DEATH,
    SP__POISON, SP__FEAR, SP__CHARM_PERSON, SP__PARALYSIS, SP__ANTI_SPELL,
    SP__CONFUSION, SP__SHIELD, SP__SURRENDER, SP__RESIST_COLD, SP__CAUSE_LIGHT_WOUNDS,
    SP__RESIST_HEAT, SP__AMNESIA, SP__COUNTER_SPELL, SP__DELAYED_EFFECT,
    SP__REMOVE_ENCHANTMENT, SP__PROTECTION_FROM_EVIL, SP__INVISIBILITY,
    SP__COUNTER_SPELL2, SP__SUMMON_ELEMENTAL, SP__MAGIC_MIRROR, SP__PERMANENCY,
    SP__CURE_LIGHT_WOUNDS, SP__SUMMON_GOBLIN, SP__SUMMON_OGRE, SP__SUMMON_TROLL,
    SP__SUMMON_GIANT, SP__DISPEL_MAGIC, SP__CURE_HEAVY_WOUNDS, SP__STAB
};
int alphabet_sort_list[NUMSPELLS] = {
    SP__AMNESIA, SP__ANTI_SPELL, SP__BLINDNESS, SP__CAUSE_HEAVY_WOUNDS,
    SP__CAUSE_LIGHT_WOUNDS, SP__CHARM_MONSTER, SP__CHARM_PERSON, SP__CONFUSION,
    SP__COUNTER_SPELL, SP__COUNTER_SPELL2, SP__CURE_HEAVY_WOUNDS,
    SP__CURE_LIGHT_WOUNDS, SP__DELAYED_EFFECT, SP__DISEASE, SP__DISPEL_MAGIC,
    SP__FEAR, SP__FINGER_OF_DEATH, SP__FIRE_STORM, SP__FIREBALL, SP__HASTE,
    SP__ICE_STORM, SP__INVISIBILITY, SP__LIGHTNING_BOLT, SP__LIGHTNING_BOLT2,
    SP__MAGIC_MIRROR, SP__MISSILE, SP__PARALYSIS, SP__PERMANENCY, SP__POISON,
    SP__PROTECTION_FROM_EVIL, SP__RAISE_DEAD, SP__REMOVE_ENCHANTMENT,
    SP__RESIST_COLD, SP__RESIST_HEAT, SP__SHIELD, SP__SUMMON_ELEMENTAL,
    SP__SUMMON_GIANT, SP__SUMMON_GOBLIN, SP__SUMMON_OGRE, SP__SUMMON_TROLL,
    SP__SURRENDER, SP__TIME_STOP, SP__STAB
};

static int check_one_spell(numgests, glist, mainhand, spel)
int numgests;
struct wizgesture *glist;
int mainhand;
struct spelldef *spel;
{
    int otherhand = (1-mainhand);
    int jx, result, pos;

    for (jx=spel->length-1; jx>=0; jx--) {
	pos = numgests - (spel->length - jx);
	if (pos<0) {
	    /* fell off beginning of gesture list */
	    return 0; 
	}
	if (spel->gests[jx] & Gesture_DOUBLE) {
	    result = spel->gests[jx] & (~Gesture_DOUBLE);
	    if (!(glist[pos].did[mainhand] == result
		  && glist[pos].did[otherhand] == result)) {
		/* failed to do two-handed gesture */
		return 0; 
	    }
	}
	else {
	    if (!(glist[pos].did[mainhand] == spel->gests[jx])) {
		/* failed to do one-handed gesture */
		 return 0; 
	    }
	}
    }

    return 1;
}

/* writes results into found[] (OR of MASK_LEFT and MASK_RIGHT, or else MASK_BOTH) */
void find_castspells(found, numgests, glist)
int *found; /* pointer to array[NUMSPELLS] */
int numgests;
struct wizgesture *glist;
{
    int ix, result;
    struct spelldef *spel;

    for (ix=0; ix<NUMSPELLS; ix++) {
	spel = &(spelllist[ix]);
	result = 0;
	if (check_one_spell(numgests, glist, 0, spel))
	    result |= MASK_LEFT;
	if (check_one_spell(numgests, glist, 1, spel))
	    result |= MASK_RIGHT;
	if (result && (spel->gests[spel->length-1] & Gesture_DOUBLE)) {
	    /* spell was completed with a double-handed gesture */
	    result = MASK_TWOHAND;
	}
	found[ix] = result;
    }
}
