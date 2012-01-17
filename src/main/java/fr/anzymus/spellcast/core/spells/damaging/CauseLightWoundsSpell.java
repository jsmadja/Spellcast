package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;


public class CauseLightWoundsSpell extends DamagingSpell {

    public CauseLightWoundsSpell() {
        spellGestures.add(wave);
        spellGestures.add(fingers);
        spellGestures.add(palm);
    }
    
    protected int getDamagePoints() {
        return 2;
    }

    @Override
    public String description() {
        return "The subject of this spell is inflicted with 2 points of damage. Resistance to heat or cold offers no defence. A simultaneous 'cure light wounds' will serve only to reduce the damage to 1 point. A 'shield' has no effect.";
    }

}
