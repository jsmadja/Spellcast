package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.Creatures;
import fr.anzymus.spellcast.core.spells.Wizards;
import fr.anzymus.spellcast.core.spells.enchantments.ParalysisSpell;

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
