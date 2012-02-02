package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class PoisonSpell extends AbstractSpell {

    public PoisonSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(fingers);
        spellGestures.add(wave);
        spellGestures.add(digit_pointing);
    }
    
    @Override
    public String description() {
        return "This is the same as the disease spell except that 'cure heavy wounds' does not stop its effects.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
