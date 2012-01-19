package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.util.List;
import java.util.Map;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.PlayerNotCreatedException;
import fr.anzymus.spellcast.core.spells.Spell;

public class ShieldTurnTest {

    @Test
    public void should_have_a_shield_only_for_a_turn() throws PlayerNotCreatedException {
        Game game = new Game();
        Player player1 = game.createNewPlayer("player1");
        Player player2 = game.createNewPlayer("player2");
        game.beginTurn();
        player1.makeGesture(palm, nothing);
        player2.makeGesture(nothing, nothing);
        game.validateTurn();
        assertFalse(player2.getWizard().hasShield());
    }
    
    @Test
    public void should_not_stab_a_wizard_who_has_a_shield() throws PlayerNotCreatedException {
        Game game = new Game();
        Player player1 = game.createNewPlayer("player1");
        Player player2 = game.createNewPlayer("player2");
        game.beginTurn();
        player1.makeGesture(palm, nothing);
        player2.makeGesture(stab, nothing);
        Map<Player, List<Spell>> choices = game.validateTurn();
        
        Spell shield = choices.get(player1).get(0);
        player1.cast(shield, player1.getWizard());
       
        Spell stab = choices.get(player2).get(0);
        player2.cast(stab, player1.getWizard());
        
        game.endTurn();
        
        assertEquals(player1.getWizard().getInitialHealth(), player1.getWizard().getHealth());
    }
    
}
