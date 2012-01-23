package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.PlayerNotCreatedException;
import fr.anzymus.spellcast.core.Wizard;

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
        Wizard wizard1 = player1.getWizard();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        player2.makeGesture(stab, nothing);
        List<Decision> decisions = game.validateTurn();
        decisions.get(0).to(wizard1);
        decisions.get(1).to(wizard1);
        game.endTurn();
        
        assertEquals(wizard1.getInitialHealth(), wizard1.getHealth());
    }
    
}
