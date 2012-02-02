package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class CharmMonsterSpell extends AbstractSpell {

    public CharmMonsterSpell() {
        spellGestures.add(palm);
        spellGestures.add(snap);
        spellGestures.add(digit_pointing);
        spellGestures.add(digit_pointing);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {

    }

    @Override
    public String description() {
        return "Except for cancellation with other enchantments, this spell only affects monsters (excluding elementals). Control of the monster is transferred to the caster of the spell (or retained by him) as of this turn, i.e. the monster will attack whosoever its new controller dictates from that turn onwards including that turn. Further charms are, of course, possible, transferring as before. If the subject of the charm is also the subject of any of: 'amnesia', 'confusion', 'charm person', 'fear' or 'paralysis', none of the spells work.";
    }

}
