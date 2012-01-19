package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.Creatures;
import fr.anzymus.spellcast.core.spells.Wizards;

public class ParalysisSpellTest {

    ParalysisSpell paralysisSpell = new ParalysisSpell();
    
    @Test
    public void should_paralysis_wizard() {
        Wizard wizard = Wizards.create();
        assertFalse(wizard.isParalyzing());
        paralysisSpell.castTo(wizard,wizard);
        assertTrue(wizard.isParalyzing());
    }
    
    @Test
    public void should_paralysis_creature() {
        Wizard wizard = Wizards.create();
        Creature creature = Creatures.create();
        assertFalse(creature.isParalyzing());
        paralysisSpell.castTo(wizard, creature);
        assertTrue(creature.isParalyzing());
    }
}
