/* header file for client */

typedef void game;

#define Turn_NORMAL	(0)
#define Turn_HASTE	(1)
#define Turn_TIMESTOP	(2)

#define Gesture_NOTHING		(0)
#define Gesture_PALM		(1)
#define Gesture_DIGIT		(2)
#define Gesture_FINGERS		(3)
#define Gesture_WAVE		(4)
#define Gesture_CLAPHALF	(5)
#define Gesture_SNAP		(6)
#define Gesture_KNIFE		(7)

#define Gesture_ANTISPELL	(8) /* not less than NUMGESTURES, because. */
#define Gesture_UNCLEAR		(9) /* not less than NUMGESTURES, because. */

#define NUMGESTURES		(8)
#define Gesture_DOUBLE		(16)

#define MAXPLAYERS		(8)

#define Gender_NONE		(0)
#define Gender_MALE		(1)
#define Gender_FEMALE		(2)
#define Gender_NEUTER		(3)

#define Stuff_RESIST_HEAT	(1<<0)
#define Stuff_RESIST_COLD	(1<<1)
#define Stuff_PROTECT_EVIL	(1<<2)
#define Stuff_POISON		(1<<3)
#define Stuff_DISEASE		(1<<4)
#define Stuff_INVISIBLE		(1<<5)
#define Stuff_BLIND		(1<<6)

#define Qu_NoQuery	(0)
/* placeholder; to be ignored by client */
#define Qu_LeftHand	(1)
#define Qu_RightHand	(2)
/* rock is an array of int. rock[0] is the list length; rock[1..len] is a spell number,
   ORed with QuVal_Hand_Both if the spell requires both hands. Answer is an index 
   into rock (0..len-1) */
#define Qu_SetOffDelay	(3)
/* rock is the spell number */

#define Qu_ElementalType (4)
/* no rock */

#define Qu_ParalysisHand (5)
#define Qu_CharmHand (6)
/* rock is index number of target wizard */

#define Qu_CharmGesture (7)
/* rock is index number of target wizard. Answer is Gesture_*. */

#define Qu_MonsterTarget (8)
/* rock is (0 if monster gets one attack, 1 or 2 if it gets two) * 256 + monster index.
   Answer is a wizard/creature number, ORed with QuVal_Target_*. (Or 0 for none). */

#define Qu_WhichToDelay (9)
#define Qu_WhichToPerm (10)
/* rock is a pointer to an int array of spell numbers, terminated by -1. */

/* for qtypes higher than this, it's a target picker. rock is the spell number,
   ORed with QuVal_Hand_*. Answer is a wizard/creature/corpse number, ORed with
   QuVal_Target_*. (Or 0 for none) */

#define Qu_TargetBeing		(20)
#define Qu_TargetBeingNone	(21)
#define Qu_TargetWizard		(22)
#define Qu_TargetWizardNone	(23)
#define Qu_TargetRaiseDead	(24)


#define QuVal_Hand_Left		(256)
#define QuVal_Hand_Right	(512)
#define QuVal_Hand_Both		(1024)
#define QuVal_Hand_MASK		(QuVal_Hand_Left | QuVal_Hand_Right | QuVal_Hand_Both)

#define QuVal_Target_Wizard	(256)
#define QuVal_Target_Creature	(512)
#define QuVal_Target_Corpse	(1024)
#define QuVal_Target_MASK	(QuVal_Target_Wizard | QuVal_Target_Creature | QuVal_Target_Corpse)


struct query {
    int player;
    int qtype;
    char *rock;
    int answer; /* to be filled in */
};

struct interface {
    /* in the printing calls, any char * may be NULL */
    void (*proc_PrintMsg)( /* char *msg, game *game, rock */ );
    void (*proc_PrintMsg2)( /* int person1, char *msg1, char *msgelse, game *game, rock */ );  
    void (*proc_PrintMsg3)( /* int person1, int person2, char *msg1, char *msg2,
		char *msgelse, game *game, rock */ );
    void (*proc_Queries)( /* int numqueries, struct query *qlist, game *game, rock */ ); 
};

extern game *BeginGame( /* int numplayers, char **names, int *genders, struct interface *callbacks, rock */ );
/* names is a pointer to an array of name strings. */

extern int RunTurn( /* game *game, int *moves */ );
/* moves contains two ints for each player (left, right). The encoding is with Gesture_*. */

extern void SeeGesture( /* game *game, int player, int asker, int *buf, int size */ );

extern void FreeGame( /* game *game */ );

extern int TurnType( /* game *game */ );
extern int TurnPlayerActive( /* game *game, int player */ );

extern int NumberOfTargets( /* game *game, int targettype */);
extern char *NameOfTarget( /* game *game, int targettype, int targetnum */);
extern int StuffAboutTarget( /* game *game, int targettype, int targetnum */);
/* targettype is QuVal_Target_* */

extern int NumberOfBeings( /* game *game, int targettype */);
extern int HitPointsOfBeing( /* game *game, int targettype, int indexnum */);
extern int StuffAboutBeing( /* game *game, int targettype, int indexnum */);
extern int OwnerOfCreature( /* game *game, int indexnum */);
extern char *NameOfBeing( /* game *game, int targettype, int indexnum */);
/* targettype is QuVal_Target_* */

extern void LogInTranscript( /* game *game, char *str */);
extern void WriteTranscript( /* game *game, FILE *f */);
