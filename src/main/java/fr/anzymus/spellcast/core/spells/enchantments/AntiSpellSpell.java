package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class AntiSpellSpell extends AbstractSpell {

    public AntiSpellSpell() {
        spellGestures.add(snap);
        spellGestures.add(palm);
        spellGestures.add(fingers);
    }
    
    @Override
    public String description() {
        return "On the turn following the casting of this spell, the subject cannot include any gestures made on or before this turn in a spell sequence and must restart a new spell from the beginning of that spell sequence. The spell does not affect spells which are cast on the same turn nor does it affect monsters.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
