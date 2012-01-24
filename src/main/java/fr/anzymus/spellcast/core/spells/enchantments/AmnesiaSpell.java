package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class AmnesiaSpell extends AbstractSpell {

    public AmnesiaSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(palm);
        spellGestures.add(palm);
    }
    
    @Override
    public String description() {
        return "If the subject of this spell is a wizard, next turn he must repeat identically the gestures he made in the current turn, including stabs. If the subject is a monster it will attack whoever it attacked this turn. If the subject is simultaneously the subject of any of 'confusion', 'charm person', 'charm monster', 'paralysis' or 'fear' then none of the spells work.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        target.setAmnesic(true);
    }

}
