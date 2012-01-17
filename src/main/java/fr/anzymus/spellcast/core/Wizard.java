package fr.anzymus.spellcast.core;

import java.util.ArrayList;
import java.util.List;

import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;
import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
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
        this.owner = owner;
    }

    public GestureHistory getGestureHistory() {
        return gestureHistory ;
    }

    public void makeGesture(Gesture leftHandGesture, Gesture rightHandGesture) {
        Gestures gestures = new Gestures(leftHandGesture, rightHandGesture);
        gestureHistory.add(gestures);
    }

    public Spell castSpell() {
        for (Spell spell : spells) {
            if(spell.apply(gestureHistory)) {
                return spell;
            }
        }
        return null;
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
        return creatures ;
    }
    
}
