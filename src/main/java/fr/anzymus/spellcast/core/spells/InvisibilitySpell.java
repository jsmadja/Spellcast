package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.Target;

public class InvisibilitySpell extends AbstractSpell {

    public InvisibilitySpell() {
        spellGestures.add(palm);
        spellGestures.add(palm);
        spellGestures.add(wave, wave);
        spellGestures.add(snap, snap);
    }

    @Override
    public void castTo(Target target) {
    }
    
}
