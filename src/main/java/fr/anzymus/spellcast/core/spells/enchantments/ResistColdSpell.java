package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class ResistColdSpell extends AbstractSpell {

    public ResistColdSpell() {
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(fingers);
        spellGestures.add(palm);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "The effects of this spell are identical to 'resist heat' but resistance is to cold ('ice storm' and ice elementals) and it destroys ice elementals if they are the subject of the spell but doesn't affect fire elementals.";
    }

}
