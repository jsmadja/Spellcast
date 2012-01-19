package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;

public class InvisibilitySpellTest {

    InvisibilitySpell invisibilitySpell = new InvisibilitySpell();

    @Test
    public void should_cast_invisibility() {
        Wizard target = Wizards.create();
        Wizard attacker = Wizards.create();
        
        assertFalse(target.isInvisible());
        invisibilitySpell.castTo(attacker, target);
        assertTrue(target.isInvisible());
    }
    
}
