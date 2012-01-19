package fr.anzymus.spellcast.core.spells.summons;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Goblin;
import fr.anzymus.spellcast.core.spells.Wizards;

public class SummonGoblinSpellTest {

    SummonGoblinSpell summonGoblinSpell = new SummonGoblinSpell();
    
    @Test
    public void should_summon_a_goblin() {
        Wizard target = Wizards.create();
        Wizard attacker = Wizards.create();
        
        List<Creature> creatures = target.getCreatures();
        assertTrue(creatures.isEmpty());
        summonGoblinSpell.castTo(attacker, target);
        assertEquals(1, target.getCreatures().size());
        Creature creature = target.getCreatures().get(0);
        assertTrue(creature instanceof Goblin);
        assertTrue(creature.getName().endsWith("the Goblin"));
    }
    
}
