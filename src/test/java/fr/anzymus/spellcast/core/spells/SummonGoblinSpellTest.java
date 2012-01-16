package fr.anzymus.spellcast.core.spells;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;

public class SummonGoblinSpellTest {

    SummonGoblinSpell summonGoblinSpell = new SummonGoblinSpell();
    
    @Test
    public void should_summon_a_goblin() {
        Wizard wizard = Wizards.create();
        List<Creature> creatures = wizard.getCreatures();
        assertTrue(creatures.isEmpty());
        summonGoblinSpell.castTo(wizard);
        assertEquals(1, wizard.getCreatures().size());
        Creature creature = wizard.getCreatures().get(0);
        assertTrue(creature instanceof Goblin);
    }
    
}
