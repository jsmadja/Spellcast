package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class CharmPersonSpell extends AbstractSpell {

    public CharmPersonSpell() {
        spellGestures.add(palm);
        spellGestures.add(snap);
        spellGestures.add(digit_pointing);
        spellGestures.add(fingers);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {

    }

    @Override
    public String description() {
        return "Except for cancellation with other enchantments, this spell only affects humans. The subject is told which of his hands will be controlled at the time the spell hits, and in the following turn, the caster of the spell writes down the gesture he wants the subject's named hand to perform. This could be a stab or nothing. (Some people, myself included do not allow the gesture to be nothing. Makes the game more exciting, as there is no reason to choose any non-nothing gesture when you have the choice. -- B) If the subject is only so because of a reflection from a 'magic mirror' the subject of the mirror assumes the role of caster and writes down his opponent's gesture. If the subject is also the subject of any of 'amnesia', 'confusion', 'charm monster', 'paralysis' or 'fear', none of the spells work.";
    }

}
