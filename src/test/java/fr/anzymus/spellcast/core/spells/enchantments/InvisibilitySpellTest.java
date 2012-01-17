package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;
import fr.anzymus.spellcast.core.spells.enchantments.InvisibilitySpell;

public class InvisibilitySpellTest {

    InvisibilitySpell invisibilitySpell = new InvisibilitySpell();

    @Test
    public void should_cast_invisibility() {
        Wizard wizard = Wizards.create(); 
        assertFalse(wizard.isInvisible());
        invisibilitySpell.castTo(wizard);
        assertTrue(wizard.isInvisible());
    }
    
}
