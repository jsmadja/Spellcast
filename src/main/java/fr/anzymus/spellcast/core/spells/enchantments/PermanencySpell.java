package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class PermanencySpell extends AbstractSpell {

    public PermanencySpell() {
        spellGestures.add(snap);
        spellGestures.add(palm);
        spellGestures.add(fingers);
        spellGestures.add(palm);
        spellGestures.add(snap);
        spellGestures.add(digit_pointing);
        spellGestures.add(wave);
    }
    
    @Override
    public String description() {
        return "This spell only works if cast upon a wizard. The next spell he completes, provided it is on this turn or one of the next 3, and which falls into the category of \"Enchantments\" (except 'anti-spell', 'disease', 'poison', or 'time-stop') will have its effect made permanent. This means that the effect of the extended spell on the first turn of its duration is repeated eternally. For example, a 'confusion' spell will be the same gesture rather than re-rolling the dice, a 'charm person' will mean repetition of the chosen gesture, etc. If the subject of the 'permanency' casts more than one spell at the same time eligible for permanency, he chooses which has its duration extended. Note that the person who has his spell made permanent does not necessarily have to make himself the subject of the spell. A 'permanency' spell cannot increase its own duration, nor the duration of spells saved by a 'delayed effect' (so if both a 'permanency' and 'delayed effect' are eligible for the same spell to be banked or extended, a choice must be made, the losing spell being neutralized and working on the next spell instead).";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
