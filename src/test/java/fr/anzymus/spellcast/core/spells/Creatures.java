package fr.anzymus.spellcast.core.spells;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;

public class Creatures {

    public static Creature create() {
        Wizard wizard = Wizards.create();
        return new Goblin(wizard);
    }

}
