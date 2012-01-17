package fr.anzymus.spellcast.core.spells.damaging;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public abstract class DamagingSpell extends AbstractSpell {

    @Override
    public void castTo(LivingEntity target) {
        target.removeHealth(getDamagePoints());
    }

    protected abstract int getDamagePoints();

}
