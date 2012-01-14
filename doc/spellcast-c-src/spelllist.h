#define NUMSPELLS (43)		/* number of spells in the book */
#define MAXSPELLLENGTH 8

struct spelldef {
    int length;
    int gests[MAXSPELLLENGTH];
    int extendable; /* with permanency */
    int selfcast; /* generally cast it on yourself? */
    char *name;
};

extern struct spelldef spelllist[NUMSPELLS];
extern int reverse_sort_list[NUMSPELLS];
extern int alphabet_sort_list[NUMSPELLS];

#define SP__DISPEL_MAGIC	(0)
#define SP__SUMMON_ELEMENTAL	(1)
#define SP__MAGIC_MIRROR	(2)
#define SP__LIGHTNING_BOLT	(3)
#define SP__CURE_HEAVY_WOUNDS	(4)
#define SP__CURE_LIGHT_WOUNDS	(5)
#define SP__AMNESIA		(6)
#define SP__CONFUSION		(7)
#define SP__DISEASE		(8)
#define SP__BLINDNESS		(9)
#define SP__DELAYED_EFFECT	(10)
#define SP__RAISE_DEAD		(11)
#define SP__POISON		(12)
#define SP__PARALYSIS		(13)
#define SP__SUMMON_TROLL	(14)
#define SP__FIREBALL		(15)
#define SP__SHIELD		(16)
#define SP__SURRENDER		(17)
#define SP__REMOVE_ENCHANTMENT	(18)
#define SP__INVISIBILITY	(19)
#define SP__CHARM_MONSTER	(20)
#define SP__CHARM_PERSON	(21)
#define SP__SUMMON_OGRE		(22)
#define SP__FINGER_OF_DEATH	(23)
#define SP__HASTE		(24)
#define SP__MISSILE		(25)
#define SP__SUMMON_GOBLIN	(26)
#define SP__ANTI_SPELL		(27)
#define SP__PERMANENCY		(28)
#define SP__TIME_STOP		(29)
#define SP__RESIST_COLD		(30)
#define SP__FEAR		(31)
#define SP__FIRE_STORM		(32)
#define SP__LIGHTNING_BOLT2	(33)
#define SP__CAUSE_LIGHT_WOUNDS	(34)
#define SP__SUMMON_GIANT	(35)
#define SP__CAUSE_HEAVY_WOUNDS	(36)
#define SP__COUNTER_SPELL	(37)
#define SP__ICE_STORM		(38)
#define SP__RESIST_HEAT		(39)
#define SP__PROTECTION_FROM_EVIL (40)
#define SP__COUNTER_SPELL2	(41)
#define	SP__STAB		(42) /* ok, it's not really a spell */
