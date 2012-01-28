package fr.anzymus.spellcast.core.spells;

import fr.anzymus.spellcast.core.CreatureNameCreator;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;

public class Creatures {

    private static final CreatureNameCreator NAME_CREATOR = new CreatureNameCreator();

    public static Creature create() {
        Wizard wizard = Wizards.create();
        Goblin goblin = new Goblin(wizard);
        String name = NAME_CREATOR.createGoblinName();
        goblin.setName(name);
        return goblin;
    }

}
