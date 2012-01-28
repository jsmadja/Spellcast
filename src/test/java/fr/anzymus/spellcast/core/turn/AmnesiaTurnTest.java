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
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.PlayerNotCreatedException;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.enchantments.AmnesiaSpell;
import fr.anzymus.spellcast.core.spells.nonspells.SurrenderSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGoblinSpell;

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
        Decisions decisions = game.validateTurn();
        decisions.cast(player1, ShieldSpell.class).to(player1.getWizard());
        game.endTurn();

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(nothing, nothing);
        decisions = game.validateTurn();
        decisions.cast(player1, AmnesiaSpell.class).to(amnesicPlayer.getWizard());
        game.endTurn();
        assertTrue(amnesicPlayer.getWizard().isAmnesic());

        game.beginTurn();
        player1.makeGesture(palm, nothing);
        amnesicPlayer.makeGesture(palm, palm);
        decisions = game.validateTurn();
        decisions.cast(player1, ShieldSpell.class).to(player1.getWizard());
        try {
            decisions.cast(amnesicPlayer, SurrenderSpell.class).to(player1.getWizard());
            fail("amnesic player can't surrender");
        } catch(IllegalAccessError e) {
            
        }
        game.endTurn();
        
        assertFalse(amnesicPlayer.getWizard().isAmnesic());
    }
    
    @Test
    public void should_repeat_identically_the_last_attack_when_a_creature_is_amnesic() throws PlayerNotCreatedException {
        Game game = new Game();
        Player player1 = game.createNewPlayer("player1");
        Player player2 = game.createNewPlayer("player2");
        Player player3 = game.createNewPlayer("player3");

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
        decisions.cast(player1, SummonGoblinSpell.class).to(player1.getWizard());
        decisions.cast(player3, ShieldSpell.class).to(player3.getWizard());
        game.endTurn();

        Creature goblin = player1.getWizard().getCreatures().get(0);

        // 4. the goblin attacks player 2
        game.beginTurn();
        player1.makeGesture(nothing, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, palm);
        decisions = game.validateTurn();
        decisions.cast(player3, AmnesiaSpell.class).to(goblin);
        decisions.attack(goblin).to(player2.getWizard());
        game.endTurn();
        
        assertEquals(13, player2.getWizard().getHealth());
        
        // the goblin attacks player 3 but it will attack player 2 due to amnesia
        game.beginTurn();
        player1.makeGesture(nothing, nothing);
        player2.makeGesture(nothing, nothing);
        player3.makeGesture(nothing, nothing);
        decisions = game.validateTurn();
        decisions.attack(goblin).to(player3.getWizard());
        game.endTurn();
        
        assertEquals(12, player2.getWizard().getHealth());
        assertEquals(player3.getWizard().getInitialHealth(), player3.getWizard().getHealth());

    }
}
