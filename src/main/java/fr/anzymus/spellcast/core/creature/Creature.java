package fr.anzymus.spellcast.core.creature;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;

public abstract class Creature extends LivingEntity {

    private Wizard master;

    public Creature(Wizard wizard) {
        this.master = wizard;
    }

    public Wizard getMaster() {
        return master;
    }
    
}
