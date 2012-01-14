package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.Target;
import fr.anzymus.spellcast.core.Wizard;

public class SurrenderSpell extends AbstractSpell {

    public SurrenderSpell() {
        spellGestures.add(palm, palm);
    }

    @Override
    public void castTo(Target target) {
        if(target instanceof Wizard) {
            Wizard wizard = (Wizard) target;
            Player player = wizard.getOwner();
            player.surrender();
        }
    }
    
}
