package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import fr.anzymus.spellcast.core.Target;
import fr.anzymus.spellcast.core.Wizard;

public class StabSpell extends AbstractSpell {

    public StabSpell() {
        spellGestures.add(stab);
    }

    public void castTo(Target target) {
        if(target instanceof Wizard) {
            Wizard wizard = (Wizard) target;
            wizard.removeHealth(1);
        }
    }

}
