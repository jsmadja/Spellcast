package fr.anzymus.spellcast.core.creature;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;

public abstract class Creature extends LivingEntity {

    private Logger log = LoggerFactory.getLogger(Creature.class);
    
    private Wizard master;

    private LivingEntity lastTarget = null;
    
    public Creature(Wizard wizard) {
        this.master = wizard;
    }

    public Wizard getMaster() {
        return master;
    }

    public void attack(LivingEntity target) {
        if(isAmnesic()) {
            log.info(getName()+" is amnesic and attacks its last target");
            target = lastTarget;
        }
        log.info(getName() +" attacks "+target);
        target.removeHealth(getAttackPoint());
        lastTarget = target;
    }

    public abstract int getAttackPoint();
    
    @Override
    public boolean equals(Object o) {
        if(o instanceof Creature) {
            Creature c = (Creature) o;
            return c.master.equals(master) && c.getName().equals(getName());
        }
        return false;
    }
    
}
