package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.WizardNotCreatedException;
import fr.anzymus.spellcast.core.spells.nonspells.StabSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;

public class ShieldTurnTest {

    @Test
    public void should_have_a_shield_only_for_a_turn() throws WizardNotCreatedException {
        Game game = new Game();
        Wizard player1 = game.createNewWizard("player1");
        Wizard player2 = game.createNewWizard("player2");
        game.beginTurn();
        player1.makeGesture(palm, nothing);
        player2.makeGesture(nothing, nothing);
        game.validateTurn();
        assertFalse(player2.hasShield());
    }
    
    @Test
    public void should_not_stab_a_wizard_who_has_a_shield() throws WizardNotCreatedException {
        Game game = new Game();
        Wizard player1 = game.createNewWizard("player1");
        Wizard player2 = game.createNewWizard("player2");

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        player2.makeGesture(stab, nothing);
        Decisions decisions = game.validateTurn();
        decisions.cast(player1, ShieldSpell.class).to(player1);
        decisions.cast(player2, StabSpell.class).to(player1);
        game.endTurn();
        
        assertEquals(player1.getInitialHealth(), player1.getHealth());
    }
    
}
