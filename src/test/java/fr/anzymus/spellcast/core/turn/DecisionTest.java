package fr.anzymus.spellcast.core.turn;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;

public class DecisionTest {

    @Test
    public void should_be_fluent() {
        Decision decisions = new Decision();
        
        Player player = new Player("wizard");
        Spell spell = new ShieldSpell();
        LivingEntity target = player.getWizard();

        assertFalse(target.hasShield());

        decisions.player(player).castSpell(spell).to(target);
        
        assertTrue(target.hasShield());
    }
    
}
