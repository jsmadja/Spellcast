package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class FearSpell extends AbstractSpell {

    public FearSpell() {
        spellGestures.add(snap);
        spellGestures.add(wave);
        spellGestures.add(digit_pointing);
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "In the turn following the casting of this spell, the subject cannot perform a C, D, F or S gesture. This obviously has no effect on monsters. If the subject is also the subject of 'amnesia', 'confusion', 'charm person', 'charm monster' or 'paralysis', then none of the spells work.";
    }
    
}
