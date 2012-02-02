package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.Game;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.WizardNotCreatedException;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.enchantments.ConfusionSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGoblinSpell;


public class ConfusionTurnTest {

    private Class<? extends Spell> confusion = ConfusionSpell.class;

    @Test
    public void should_confuse_a_wizard() throws WizardNotCreatedException {
        
        Game game = new Game();
        Wizard player = game.createNewWizard("player1");
        Wizard confusedPlayer = game.createNewWizard("player2");
       
        game.beginTurn();
        player.makeGesture(Gesture.digit_pointing, Gesture.nothing);
        confusedPlayer.makeGesture(Gesture.nothing, Gesture.nothing);
        game.validateTurn();
        game.endTurn();
        
        game.beginTurn();
        player.makeGesture(Gesture.snap, Gesture.nothing);
        confusedPlayer.makeGesture(Gesture.nothing, Gesture.nothing);
        game.validateTurn();
        game.endTurn();
        
        game.beginTurn();
        player.makeGesture(Gesture.fingers, Gesture.nothing);
        confusedPlayer.makeGesture(Gesture.nothing, Gesture.nothing);
        Decisions decisions = game.validateTurn();
        decisions.cast(player, confusion).to(confusedPlayer);
        game.endTurn();
        
        game.beginTurn();
        player.makeGesture(nothing, nothing);
        confusedPlayer.makeGesture(nothing, nothing);
        decisions = game.validateTurn();
        if(decisions.size() != 0) {
            decisions.cast(confusedPlayer, ShieldSpell.class).to(confusedPlayer);
        }
        game.endTurn();
        
        Gestures confusedPlayerLastGestures = confusedPlayer.getGestureHistory().getLastGestures();
        Gesture leftHandGesture = confusedPlayerLastGestures.getLeftHandGesture();
        Gesture rightHandGesture = confusedPlayerLastGestures.getLeftHandGesture();
        assertFalse ((!leftHandGesture.equals(nothing) && rightHandGesture.equals(nothing)) || (leftHandGesture.equals(nothing) && !rightHandGesture.equals(nothing)));
    }
    
    @Test
    public void should_confuse_a_creature() throws WizardNotCreatedException {
        
        Game game = new Game();
        Wizard player = game.createNewWizard("player1");
        Wizard player2 = game.createNewWizard("player2");
       
        game.beginTurn();
        player.makeGesture(nothing, nothing);
        player2.makeGesture(snap, nothing);
        game.validateTurn();
        game.endTurn();
        
        game.beginTurn();
        player.makeGesture(digit_pointing, nothing);
        player2.makeGesture(fingers, nothing);
        game.validateTurn();
        game.endTurn();
        
        game.beginTurn();
        player.makeGesture(snap, nothing);
        player2.makeGesture(wave, nothing);
        Decisions decisions = game.validateTurn();
        decisions.cast(player2, SummonGoblinSpell.class).to(player2);
        game.endTurn();
        
        Creature creature = player2.getCreatures().get(0);
        
        game.beginTurn();
        player.makeGesture(fingers, nothing);
        player2.makeGesture(nothing, nothing);
        decisions = game.validateTurn();
        decisions.attack(creature).to(player);
        decisions.cast(player, confusion).to(creature);
        game.endTurn();
        
        LivingEntity lastTarget = creature.getLastTarget();
        assertTrue(player.equals(lastTarget) || player2.equals(lastTarget));
    }
}
