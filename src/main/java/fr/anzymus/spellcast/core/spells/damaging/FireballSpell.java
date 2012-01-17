package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;


public class FireballSpell extends DamagingSpell {

    public FireballSpell() {
        spellGestures.add(fingers);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(digit_pointing);
        spellGestures.add(digit_pointing);
    }
    
    protected int getDamagePoints() {
        return 5;
    }

    @Override
    public String description() {
        return "The subject of this spell is hit by a ball of fire and sustains 5 points of damage unless he is resistant to fire. If at the same time an 'ice storm' prevails, the subject of the 'fireball' is instead not harmed by either spell, although the storm will affect others as normal. If directed at an ice elemental, the fireball will destroy it before it can attack, but has no other effect on the creatures.";
    }

}
