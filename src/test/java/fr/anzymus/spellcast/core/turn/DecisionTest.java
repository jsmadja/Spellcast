package fr.anzymus.spellcast.core.turn;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;

public class DecisionTest {

    @Test
    public void should_be_fluent() {
        Decision decisions = new Decision();
        
        Player player = new Player("wizard");
        CastableSpell spell = new CastableSpell(new ShieldSpell(), Hand.left);
        LivingEntity target = player.getWizard();

        assertFalse(target.hasShield());

        decisions.player(player).castableSpell(spell).to(target);
        
        assertTrue(target.hasShield());
    }
    
}
