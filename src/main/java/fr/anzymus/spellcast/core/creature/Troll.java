package fr.anzymus.spellcast.core.creature;

import fr.anzymus.spellcast.core.Wizard;

public class Troll extends Creature {

    public Troll(Wizard wizard) {
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
