package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;

public class ResistHeatSpellTest {

    ResistHeatSpell resistHeat = new ResistHeatSpell();
    
    @Test
    public void should_add_heat_resistance_to_wizard() {
        Wizard wizard = Wizards.create();
        assertFalse(wizard.isResistHeat());
        resistHeat.castTo(wizard,wizard);
        assertTrue(wizard.isResistHeat());
    }
    
}
