package fr.anzymus.spellcast.core.spells.nonspells;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;

public class SurrenderSpellTest {

    SurrenderSpell surrenderSpell = new SurrenderSpell();
    
    @Test
    public void should_surrend_a_wizard() {
        Wizard wizard = Wizards.create();
        Player player = wizard.getOwner();
        assertFalse(player.hasSurrendered());
        surrenderSpell.castTo(wizard);
        assertTrue(player.hasSurrendered());
    }
    
}
