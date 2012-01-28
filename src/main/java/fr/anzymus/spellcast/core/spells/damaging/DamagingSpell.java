package fr.anzymus.spellcast.core.spells.damaging;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public abstract class DamagingSpell extends AbstractSpell {

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        if(!target.hasCounterSpell()) {
            target.removeHealth(getDamagePoints());
        }
    }

    protected abstract int getDamagePoints();

}
