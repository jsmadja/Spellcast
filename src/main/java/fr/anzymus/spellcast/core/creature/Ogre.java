package fr.anzymus.spellcast.core.creature;

import fr.anzymus.spellcast.core.Wizard;

public class Ogre extends Creature {

    public Ogre(Wizard wizard) {
        super(wizard);
    }

    @Override
    public int getAttackPoint() {
        return 2;
    }

    @Override
    public int getInitialHealth() {
        return 2;
    }

}
