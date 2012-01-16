package fr.anzymus.spellcast.core.spells;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;

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
