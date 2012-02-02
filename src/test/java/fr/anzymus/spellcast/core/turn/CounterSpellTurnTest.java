package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.WizardNotCreatedException;
import fr.anzymus.spellcast.core.spells.damaging.MissileSpell;
import fr.anzymus.spellcast.core.spells.protection.CounterSpellSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;

public class CounterSpellTurnTest {

    @Test
    public void should_not_be_hit_by_missile_when_protected_by_counter_spell() throws WizardNotCreatedException {
        Game game = new Game();
        Wizard attacker = game.createNewWizard("player1");
        Wizard counterSpelledPlayer = game.createNewWizard("player2");
     
        game.beginTurn();
        attacker.makeGesture(nothing, nothing);
        counterSpelledPlayer.makeGesture(wave, nothing);
        game.validateTurn();
        game.endTurn();

        game.beginTurn();
        attacker.makeGesture(snap, nothing);
        counterSpelledPlayer.makeGesture(palm, nothing);
        Decisions decisions = game.validateTurn();
        decisions.cast(counterSpelledPlayer, ShieldSpell.class).to(counterSpelledPlayer);
        game.endTurn();
        
        game.beginTurn();
        attacker.makeGesture(digit_pointing, nothing);
        counterSpelledPlayer.makeGesture(palm, nothing);
        decisions = game.validateTurn();
        decisions.cast(attacker, MissileSpell.class).to(counterSpelledPlayer);
        decisions.cast(counterSpelledPlayer, CounterSpellSpell.class).to(counterSpelledPlayer);
        game.endTurn();
        
        assertEquals(counterSpelledPlayer.getInitialHealth(), counterSpelledPlayer.getHealth());
    }
    
}
