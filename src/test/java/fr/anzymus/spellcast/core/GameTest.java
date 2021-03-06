package fr.anzymus.spellcast.core;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Wizards;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.turn.Decisions;

public class GameTest {

    @Test
    public void should_have_empty_player_at_beginning() {
        Game game = new Game();
        List<Wizard> players = game.getWizards();
        assertTrue(players.isEmpty());
    }
    
    @Test
    public void should_add_a_new_player() throws Exception {
        Game game = new Game();
        
        Wizard player = game.createNewWizard("player");
        
        assertNotNull(player);
        assertEquals(1, game.getWizards().size());
    }
    
    @Test(expected = WizardNotCreatedException.class)
    public void should_not_create_a_player_if_another_player_with_same_name_already_exist() throws Exception {
        Game game = new Game();
        game.createNewWizard("player");
        game.createNewWizard("player");
    }
    
    @Test(expected = WizardNotCreatedException.class)
    public void should_not_create_a_player_with_null_name() throws Exception {
        Game game = new Game();
        game.createNewWizard(null);
    }
    
    @Test(expected = WizardNotCreatedException.class)
    public void should_not_create_a_player_with_empty_name() throws Exception {
        Game game = new Game();
        game.createNewWizard("");
    }
    
    @Test(expected = WizardNotCreatedException.class)
    public void should_not_create_a_player_with_blank_name() throws Exception {
        Game game = new Game();
        game.createNewWizard("     ");
    }
    
    @Test
    public void should_make_gesture_for_his_wizard() throws Exception {
        Game game = new Game();
        Wizard player = game.createNewWizard("player");
        player.makeGesture(fingers, palm);
        GestureHistory gestureHistory = player.getGestureHistory();
        Gestures gestures = gestureHistory.getLastGestures();
        assertEquals(fingers, gestures.getLeftHandGesture());
        assertEquals(palm, gestures.getRightHandGesture());
    }
    
    @Test
    public void should_make_multiple_gesture_for_his_wizard() throws Exception {
        Game game = new Game();
        Wizard player = game.createNewWizard("player");
        player.makeGesture(fingers, palm);
        player.makeGesture(snap, wave);
        GestureHistory gestureHistory = player.getGestureHistory();
        Gestures gestures = gestureHistory.getLastGestures();
        assertEquals(snap, gestures.getLeftHandGesture());
        assertEquals(wave, gestures.getRightHandGesture());
    }
    
    @Test
    public void should_execute_spell_to_a_target() {
        Spell shieldSpell = new ShieldSpell();
        Wizard target = Wizards.create();
        shieldSpell.castTo(target,target);
        assertTrue(target.hasShield());
    }
    
    @Test
    public void should_cast_same_spell_with_each_hand() throws WizardNotCreatedException {
        Game game = new Game();
        Wizard player = game.createNewWizard("player");
        Wizard player2 = game.createNewWizard("player2");
        game.beginTurn();
        player.makeGesture(snap, snap);
        player2.makeGesture(nothing, nothing);
        game.validateTurn();
        game.endTurn();
        
        game.beginTurn();
        player.makeGesture(digit_pointing, digit_pointing);
        player2.makeGesture(nothing, nothing);
        Decisions decisions = game.validateTurn();
        assertEquals(2, decisions.size());
    }
    
}
