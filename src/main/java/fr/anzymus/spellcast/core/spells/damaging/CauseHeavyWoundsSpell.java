package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;


public class CauseHeavyWoundsSpell extends DamagingSpell {

    public CauseHeavyWoundsSpell() {
        spellGestures.add(wave);
        spellGestures.add(palm);
        spellGestures.add(fingers);
        spellGestures.add(digit_pointing);
    }
    
    protected int getDamagePoints() {
        return 3;
    }

    @Override
    public String description() {
        return "This has the same effect as 'cause light wounds' but inflicts 3 points of damage instead of 2.";
    }

}
