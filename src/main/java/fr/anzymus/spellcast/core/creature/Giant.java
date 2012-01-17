package fr.anzymus.spellcast.core.creature;

import fr.anzymus.spellcast.core.Wizard;

public class Giant extends Creature {

    public Giant(Wizard wizard) {
        super(wizard);
    }

    @Override
    public int getAttackPoint() {
        return 4;
    }

    @Override
    public int getInitialHealth() {
        return 4;
    }

}
