package fr.anzymus.spellcast.core;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang3.RandomStringUtils;

import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;
import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Spells;

public class Wizard extends LivingEntity {

    private GestureHistory gestureHistory = new GestureHistory();

    private List<Spell> spells = Spells.createList();
    private Player owner;
    
    private boolean invisible;
    
    private CreatureNameCreator creatureNameCreator = new CreatureNameCreator();

    private List<Creature> creatures = new ArrayList<Creature>();
    
    public Wizard(Player owner) {
        super();
        setName("Wizard "+RandomStringUtils.randomAlphabetic(5).toLowerCase());
        this.owner = owner;
    }

    public GestureHistory getGestureHistory() {
        return gestureHistory ;
    }

    public void makeGesture(Gesture leftHandGesture, Gesture rightHandGesture) {
        Gestures gestures = new Gestures(leftHandGesture, rightHandGesture);
        gestureHistory.add(gestures);
    }

    public List<CastableSpell> castSpells() {
        List<CastableSpell> spellsToCast = new ArrayList<CastableSpell>();
        for (Spell spell : spells) {
            Hand hand = spell.apply(gestureHistory);
            if(hand != Hand.none) {
                CastableSpell castableSpell = new CastableSpell(spell, hand);
                spellsToCast.add(castableSpell);
            }
        }
        return spellsToCast;
    }

    public Player getOwner() {
        return owner;
    }

    public void summonCreature(Creature creature) {
        if(creature instanceof Goblin) {
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
    
}
