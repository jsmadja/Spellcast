package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.PlayerNotCreatedException;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.nonspells.SurrenderSpell;

public class AmnesiaTurnTest {

    @Test
    public void should_repeat_identically_the_gestures_when_a_wizard_is_amnesic() throws PlayerNotCreatedException {
        Game game = new Game();
        Player player1 = game.createNewPlayer("player1");
        Player amnesicPlayer = game.createNewPlayer("player2");
        
        game.beginTurn();
        player1.makeGesture(digit_pointing, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        game.validateTurn();
        game.endTurn();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        game.validateTurn();
        game.endTurn();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        List<Decision> decisions = game.validateTurn();
        decisions.get(0).to(amnesicPlayer.getWizard());
        
        assertTrue(amnesicPlayer.getWizard().isAmnesic());
        game.endTurn();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(palm, palm);
        decisions = game.validateTurn();
        for (Decision decision : decisions) {
            Spell spell = decision.getCastableSpell().getSpell();
            assertFalse(spell instanceof SurrenderSpell);
        }
        game.endTurn();
        
        assertFalse(amnesicPlayer.getWizard().isAmnesic());
    }
    
}
