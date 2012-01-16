package fr.anzymus.spellcast.core.creature;

import static org.junit.Assert.assertSame;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;

public class CreatureTest {

    @Test
    public void should_have_a_master() {
        Wizard wizard = Wizards.create();
        Creature creature = new Goblin(wizard);
        assertSame(wizard, creature.getMaster());
    }
    
}
