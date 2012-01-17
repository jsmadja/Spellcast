package fr.anzymus.spellcast.core.creature;

import fr.anzymus.spellcast.core.Wizard;

public class Elemental extends Creature {

    public Elemental(Wizard wizard) {
        super(wizard);
    }

    @Override
    public int getAttackPoint() {
        return 3;
    }

    @Override
    public int getInitialHealth() {
        return 3;
    }

}
