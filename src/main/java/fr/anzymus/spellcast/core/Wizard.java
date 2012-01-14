package fr.anzymus.spellcast.core;

import java.util.List;

import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Spells;

public class Wizard implements Target {

    private int health = 14;
    private GestureHistory gestureHistory = new GestureHistory();

    private List<Spell> spells = Spells.createList();
    private Player owner;
    
    public Wizard(Player owner) {
        this.owner = owner;
    }
    
    public int getHealth() {
        return health;
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

    public void removeHealth(int healtPoint) {
        health -= healtPoint;
        if(health < 0) {
            health = 0;
        }
    }

    public Player getOwner() {
        return owner;
    }

}
