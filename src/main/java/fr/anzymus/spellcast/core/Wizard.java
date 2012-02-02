package fr.anzymus.spellcast.core;

import static fr.anzymus.spellcast.core.Hand.both;
import static fr.anzymus.spellcast.core.Hand.each;
import static fr.anzymus.spellcast.core.Hand.left;
import static fr.anzymus.spellcast.core.Hand.none;
import static fr.anzymus.spellcast.core.Hand.right;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;
import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Spells;

public class Wizard extends LivingEntity {

    private Logger log = LoggerFactory.getLogger(Wizard.class);

    private GestureHistory gestureHistory = new GestureHistory();

    private List<Spell> spells = Spells.createList();
    private boolean surrender;

    private boolean invisible;

    private CreatureNameCreator creatureNameCreator = new CreatureNameCreator();

    private List<Creature> creatures = new ArrayList<Creature>();

    public Wizard(String name) {
        super();
        setName(name);
    }

    public GestureHistory getGestureHistory() {
        return gestureHistory;
    }

    public void makeGesture(Gesture leftHandGesture, Gesture rightHandGesture) {
        Gestures gestures = new Gestures(leftHandGesture, rightHandGesture);
        gestureHistory.add(gestures);
    }

    public List<CastableSpell> castSpells() {
        boolean isLeftHandUsed = false;
        boolean isRightHandUsed = false;
        List<CastableSpell> spellsToCast = new ArrayList<CastableSpell>();
        for (Spell spell : spells) {
            Hand hand = spell.apply(gestureHistory);
            boolean isCastable = hand == each || hand == both
                    || (hand != none && (hand == left && !isLeftHandUsed) || (hand == right && !isRightHandUsed));
            if (isCastable) {
                if (hand == both) {
                    CastableSpell castableSpell = new CastableSpell(spell, hand);
                    spellsToCast.clear();
                    spellsToCast.add(castableSpell);
                    return spellsToCast;
                }
                if (hand == each) {
                    CastableSpell castableSpellLeft = new CastableSpell(spell, left);
                    CastableSpell castableSpellRight = new CastableSpell(spell, right);
                    spellsToCast.add(castableSpellLeft);
                    spellsToCast.add(castableSpellRight);
                } else {
                    CastableSpell castableSpell = new CastableSpell(spell, hand);
                    spellsToCast.add(castableSpell);
                }
                isLeftHandUsed = hand == left || hand == both || hand == each;
                isRightHandUsed = hand == right || hand == both || hand == each;
            }
        }
        return spellsToCast;
    }

    public void summonCreature(Creature creature) {
        if (creature instanceof Goblin) {
            String goblinName = creatureNameCreator.createGoblinName();
            creature.setName(goblinName);
        }
        creatures.add(creature);
    }

    @Override
    public int getInitialHealth() {
        return 14;
    }

    public boolean isInvisible() {
        return invisible;
    }

    public void setInvisible(boolean invisible) {
        this.invisible = invisible;
    }

    public List<Creature> getCreatures() {
        return creatures;
    }

    public void replaceGesturesByLastGestures() {
        gestureHistory.removeLastGestures();
        Gestures lastGestures = gestureHistory.getLastGestures();
        gestureHistory.add(lastGestures);
    }

    public void replaceGestureBy(Hand hand, Gesture gesture) {
        Gestures gestures = gestureHistory.removeLastGestures();
        if (hand == Hand.left) {
            gestures.setLeftHandGesture(gesture);
        } else {
            gestures.setRightHandGesture(gesture);
        }
        gestureHistory.add(gestures);
    }

    public void surrender() {
        this.surrender = true;
    }

    public boolean hasSurrendered() {
        return surrender;
    }

    public void cast(Spell spell, LivingEntity target) {
        if (spell != null) {
            log.info(getName() + " cast " + spell.getClass().getSimpleName() + " to " + target.getName());
            spell.castTo(this, target);
        }
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Wizard) {
            return getName().equals(((Wizard) obj).getName());
        }
        return false;
    }

    @Override
    public String toString() {
        String string = getName() + " " + getHealth() + " HP";
        if (isAmnesic()) {
            string += " amnesic";
        }
        if (isInvisible()) {
            string += " invisible";
        }
        if (isParalyzing()) {
            string += " paralyzing";
        }
        if (isResistHeat()) {
            string += " resistHeat";
        }
        if (hasCounterSpell()) {
            string += " counterSpell";
        }
        if (hasMagicMirror()) {
            string += " magic mirror";
        }
        if (hasShield()) {
            string += " shield";
        }
        if (isConfused()) {
            string += " confused";
        }
        return string;
    }

}
