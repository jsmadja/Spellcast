package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;


public class FingerOfDeathSpell extends DamagingSpell {

    public FingerOfDeathSpell() {
        spellGestures.add(palm);
        spellGestures.add(wave);
        spellGestures.add(palm);
        spellGestures.add(fingers);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(digit_pointing);
    }
    
    protected int getDamagePoints() {
        return Integer.MAX_VALUE;
    }

    @Override
    public String description() {
        return "Kills the subject stone dead. This spell is so powerful that it is unaffected by a 'counter-spell' although a 'dispel magic' spell cast upon the final gesture will stop it. The usual way to prevent being harmed by this spell is to disrupt it during casting using, for example, an 'anti-spell'.";
    }

}
