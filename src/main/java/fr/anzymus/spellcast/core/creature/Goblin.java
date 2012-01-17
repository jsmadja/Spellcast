package fr.anzymus.spellcast.core.creature;

import fr.anzymus.spellcast.core.Wizard;

public class Goblin extends Creature {

    public Goblin(Wizard wizard) {
        super(wizard);
    }

    @Override
    public int getInitialHealth() {
        return 1;
    }

    @Override
    public int getAttackPoint() {
        return 1;
    }

}
