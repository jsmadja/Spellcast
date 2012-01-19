package fr.anzymus.spellcast.core.spells.summons;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public abstract class SummonSpell extends AbstractSpell {

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        if(target instanceof Wizard) {
            Wizard wizard = (Wizard) target;
            Creature creature = summonCreature(wizard);
            wizard.summonCreature(creature);
        }
    }

    protected abstract Creature summonCreature(Wizard owner);

}
