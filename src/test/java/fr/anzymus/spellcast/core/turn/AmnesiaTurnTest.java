package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.WizardNotCreatedException;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.enchantments.AmnesiaSpell;
import fr.anzymus.spellcast.core.spells.nonspells.SurrenderSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGoblinSpell;

public class AmnesiaTurnTest {

    @Test
    public void should_repeat_identically_the_gestures_when_a_wizard_is_amnesic() throws WizardNotCreatedException {
        Game game = new Game();
        Wizard player1 = game.createNewWizard("player1");
        Wizard amnesicPlayer = game.createNewWizard("player2");
        
        game.beginTurn();
        player1.makeGesture(digit_pointing, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        game.validateTurn();
        game.endTurn();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        Decisions decisions = game.validateTurn();
        decisions.cast(player1, ShieldSpell.class).to(player1);
        game.endTurn();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        decisions = game.validateTurn();
        decisions.cast(player1, AmnesiaSpell.class).to(amnesicPlayer);
        game.endTurn();
        assertTrue(amnesicPlayer.isAmnesic());

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(palm, palm);
        decisions = game.validateTurn();
        decisions.cast(player1, ShieldSpell.class).to(player1);
        try {
            decisions.cast(amnesicPlayer, SurrenderSpell.class).to(player1);
            fail("amnesic player can't surrender");
        } catch(IllegalAccessError e) {
            
        }
        game.endTurn();
        
        assertFalse(amnesicPlayer.isAmnesic());
    }
    
    @Test
    public void should_repeat_identically_the_last_attack_when_a_creature_is_amnesic() throws WizardNotCreatedException {
        Game game = new Game();
        Wizard player1 = game.createNewWizard("player1");
        Wizard player2 = game.createNewWizard("player2");
        Wizard player3 = game.createNewWizard("player3");

        // scenario :
        // player1 summons a goblin
        // the goblin attacks player2
        // player3 cast amnesia on goblin
        // the goblin attacks player3
        // but the goblin is amnesic and attack player2 again

        // 1.
        game.beginTurn();
        player1.makeGesture(snap, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, nothing);
        game.validateTurn();
        game.endTurn();
        
        // 2.
        game.beginTurn();
        player1.makeGesture(fingers, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, digit_pointing);
        game.validateTurn();
        game.endTurn();
        
        // 3. the player1 summon a goblin
        game.beginTurn();
        player1.makeGesture(wave, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, palm);
        Decisions decisions = game.validateTurn();
        decisions.cast(player1, SummonGoblinSpell.class).to(player1);
        decisions.cast(player3, ShieldSpell.class).to(player3);
        game.endTurn();

        Creature goblin = player1.getCreatures().get(0);

        // 4. the goblin attacks player 2
        game.beginTurn();
        player1.makeGesture(nothing, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, palm);
        decisions = game.validateTurn();
        decisions.cast(player3, AmnesiaSpell.class).to(goblin);
        decisions.attack(goblin).to(player2);
        game.endTurn();
        
        assertEquals(13, player2.getHealth());
        
        // the goblin attacks player 3 but it will attack player 2 due to amnesia
        game.beginTurn();
        player1.makeGesture(nothing, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, nothing);
        decisions = game.validateTurn();
        decisions.attack(goblin).to(player3);
        game.endTurn();
        
        assertEquals(12, player2.getHealth());
        assertEquals(player3.getInitialHealth(), player3.getHealth());

    }
}
