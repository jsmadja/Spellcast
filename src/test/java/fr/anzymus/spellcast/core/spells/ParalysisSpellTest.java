package fr.anzymus.spellcast.core.spells;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;

public class ParalysisSpellTest {

    ParalysisSpell paralysisSpell = new ParalysisSpell();
    
    @Test
    public void should_paralysis_wizard() {
        Wizard wizard = Wizards.create();
        assertFalse(wizard.isParalyzing());
        paralysisSpell.castTo(wizard);
        assertTrue(wizard.isParalyzing());
    }
    
    @Test
    public void should_paralysis_creature() {
        Creature creature = Creatures.create();
        assertFalse(creature.isParalyzing());
        paralysisSpell.castTo(creature);
        assertTrue(creature.isParalyzing());
    }
}
