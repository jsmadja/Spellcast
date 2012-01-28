package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.PlayerNotCreatedException;
import fr.anzymus.spellcast.core.spells.damaging.MissileSpell;
import fr.anzymus.spellcast.core.spells.protection.CounterSpellSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;

public class CounterSpellTurnTest {

    @Test
    public void should_not_be_hit_by_missile_when_protected_by_counter_spell() throws PlayerNotCreatedException {
        Game game = new Game();
        Player attacker = game.createNewPlayer("player1");
        Player counterSpelledPlayer = game.createNewPlayer("player2");
     
        game.beginTurn();
        attacker.makeGesture(nothing, nothing);
        counterSpelledPlayer.makeGesture(wave, nothing);
        game.validateTurn();
        game.endTurn();

        game.beginTurn();
        attacker.makeGesture(snap, nothing);
        counterSpelledPlayer.makeGesture(palm, nothing);
        Decisions decisions = game.validateTurn();
        decisions.cast(counterSpelledPlayer, ShieldSpell.class).to(counterSpelledPlayer.getWizard());
        game.endTurn();
        
        game.beginTurn();
        attacker.makeGesture(digit_pointing, nothing);
        counterSpelledPlayer.makeGesture(palm, nothing);
        decisions = game.validateTurn();
        decisions.cast(attacker, MissileSpell.class).to(counterSpelledPlayer.getWizard());
        decisions.cast(counterSpelledPlayer, CounterSpellSpell.class).to(counterSpelledPlayer.getWizard());
        game.endTurn();
        
        assertEquals(counterSpelledPlayer.getWizard().getInitialHealth(), counterSpelledPlayer.getWizard().getHealth());
    }
    
}
