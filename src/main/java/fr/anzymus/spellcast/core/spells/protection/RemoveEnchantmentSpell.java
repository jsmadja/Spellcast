package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class RemoveEnchantmentSpell extends AbstractSpell {

    public RemoveEnchantmentSpell() {
        spellGestures.add(palm);
        spellGestures.add(digit_pointing);
        spellGestures.add(wave);
        spellGestures.add(palm);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        target.setAmnesic(false);
        target.setParalyzing(false);
        target.setResistHeat(false);
    }

    @Override
    public String description() {
        return "If the subject of this spell is currently being effected by any of the spells in the spells in the \"enchantments\" section, and/or if spells from that section are cast at him at the same time as the remove enchantment, then any such spells terminate immediately although their effect for that turn might already have passed. For example, the victim of a 'blindness' spell would not be able to see what his opponent's gestures were on the turn his sight is restored by a 'remove enchantment'. Note that the 'remove enchantment' affects all spells from the \"Enchantment\" section whether the caster wants them to all go or not. A second effect of the spell is to destroy any monster upon which it is cast, although the monster can attack in that turn. It does not affect wizards unless cast on a wizard as he creates a monster when the monster is destroyed, and the effects described above apply.";
    }

}
