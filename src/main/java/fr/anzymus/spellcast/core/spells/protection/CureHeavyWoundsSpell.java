package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class CureHeavyWoundsSpell extends AbstractSpell {

    public CureHeavyWoundsSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(fingers);
        spellGestures.add(palm);
        spellGestures.add(wave);
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "This spell is the same as 'cure light wounds' for its effect, but 2 points of damage are cured instead of 1, or only 1 if only 1 had been sustained. A side effect is that the spell will also cure a disease (note 'raise dead' on a live individual won't).";
    }

}
