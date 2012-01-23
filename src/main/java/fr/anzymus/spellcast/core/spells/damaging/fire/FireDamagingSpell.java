package fr.anzymus.spellcast.core.spells.damaging.fire;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.damaging.DamagingSpell;

public abstract class FireDamagingSpell extends DamagingSpell {

    private Logger log = LoggerFactory.getLogger(FireDamagingSpell.class);

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        if (!target.isResistHeat()) {
            super.castTo(attacker, target);
        } else {
            log.info(target + " resists to heat");
        }
    }
    
}
