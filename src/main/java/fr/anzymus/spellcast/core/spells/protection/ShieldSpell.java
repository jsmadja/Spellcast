package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class ShieldSpell extends AbstractSpell {

    public ShieldSpell() {
        spellGestures.add(palm);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        target.setShield(true);
    }

    @Override
    public String description() {
        return "This spell protects the subject from all attacks from monsters (that is, creatures created by a summons class spell), from missile spells, and from stabs by wizards. The shield lasts for that turn only, but one shield will cover all such attacks made against the subject that turn.";
    }

}
