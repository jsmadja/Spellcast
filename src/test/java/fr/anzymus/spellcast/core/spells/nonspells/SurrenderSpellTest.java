package fr.anzymus.spellcast.core.spells.nonspells;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;

public class SurrenderSpellTest {

    SurrenderSpell surrenderSpell = new SurrenderSpell();
    
    @Test
    public void should_surrend_a_wizard() {
        Wizard player = Wizards.create();
        assertFalse(player.hasSurrendered());
        surrenderSpell.castTo(player, player);
        assertTrue(player.hasSurrendered());
    }
    
}
