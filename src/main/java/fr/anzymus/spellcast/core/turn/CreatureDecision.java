package fr.anzymus.spellcast.core.turn;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;

public class CreatureDecision extends Decision {

    private Creature creature;
    private Wizard wizard;

    public CreatureDecision(Wizard wizard, Creature creature) {
        this.creature = creature;
        this.wizard = wizard;
    }

    @Override
    public String toString() {
        return creature+" could attack somebody";
    }

    public Wizard getWizard() {
        return wizard;
    }
    
    public void apply() {
        creature.attack(getTarget());
    }
    
}
