package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import fr.anzymus.spellcast.core.Target;

public class ParalysisSpell extends AbstractSpell {

    public ParalysisSpell() {
        spellGestures.add(fingers);
        spellGestures.add(fingers);
        spellGestures.add(fingers);
    }

    @Override
    public void castTo(Target target) {
    }
    
}
