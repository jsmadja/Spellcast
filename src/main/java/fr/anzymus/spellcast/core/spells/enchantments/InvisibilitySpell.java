package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class InvisibilitySpell extends AbstractSpell {

    public InvisibilitySpell() {
        spellGestures.add(palm);
        spellGestures.add(palm);
        spellGestures.add(wave, wave);
        spellGestures.add(snap, snap);
    }

    @Override
    public void castTo(LivingEntity target) {
        if(target instanceof Wizard) {
            Wizard wizard = (Wizard) target;
            wizard.setInvisible(true);
        }
    }

    @Override
    public String description() {
        return "This spell is similar to 'blindness' only the subject of the spell becomes invisible to his opponent and his monsters. All spells he creates, though not gestures, can be seen by his opponent and identified. The subject cannot be attacked by any monsters although they can be directed at him in case he becomes visible prematurely. Wizards can still stab and direct spells at him, with the same hope. Any monster made invisible is destroyed due to the unstable nature of such magically created creatures.";
    }

}
