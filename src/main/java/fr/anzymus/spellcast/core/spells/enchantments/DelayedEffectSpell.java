package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class DelayedEffectSpell extends AbstractSpell {

    public DelayedEffectSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(wave);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(palm);
    }
    
    @Override
    public String description() {
        return "This spell only works if cast upon a wizard. The next spell he completes, provided it is on this turn or one of the next 3 is \"banked\" until needed, i.e. it fails to work until its caster desires. This next spell which is to be banked does not include the actual spell doing the banking. The spell must be written down to be used by its caster at the same time that he writes his gestures. Note that spells banked are those cast by the subject not those cast at him. If he casts more than one spell at the same time he chooses which is to be banked. Remember that P is a 'shield' spell, and surrender is not a spell. A wizard may only have one spell banked at any one time.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
