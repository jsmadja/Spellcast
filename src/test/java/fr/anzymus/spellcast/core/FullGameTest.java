package fr.anzymus.spellcast.core;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;

import java.util.List;

import org.junit.Ignore;
import org.junit.Test;

import fr.anzymus.spellcast.core.turn.Decision;

@Ignore
public class FullGameTest {

    @Test
    public void full_scenario() throws PlayerNotCreatedException {
        
        //Turn            BLACK                         WHITE
        Game game = new Game();
        Player black = game.createNewPlayer("BLACK");
        Player white = game.createNewPlayer("WHITE");
        Wizard blackWizard = black.getWizard();
        Wizard whiteWizard = white.getWizard();
        
//        Left    Right    Damage       Left    Right    Damage
//        1        W       W +      0            W +     P        0
        game.beginTurn();
        black.makeGesture(wave, wave);
        white.makeGesture(wave, palm);
        game.validateTurn();
        game.endTurn();
        assertEquals(14, blackWizard.getHealth());
        assertEquals(14, whiteWizard.getHealth());
        
//        2        W       P |                   W |     D
        game.beginTurn();
        black.makeGesture(wave, palm);
        white.makeGesture(wave, digit_pointing);
        game.validateTurn();
        game.endTurn();
        assertEquals(14, blackWizard.getHealth());
        assertEquals(14, whiteWizard.getHealth());

//        3        D       F |                   F |   + W
        game.beginTurn();
        black.makeGesture(digit_pointing, fingers);
        white.makeGesture(fingers, wave);
        game.validateTurn();
        game.endTurn();
        assertEquals(14, blackWizard.getHealth());
        assertEquals(14, whiteWizard.getHealth());

//        4        D     + D x                   P x   | W        3
        game.beginTurn();
        black.makeGesture(digit_pointing, digit_pointing);
        white.makeGesture(palm, wave);
        List<Decision> decisions = game.validateTurn();
        // white becomes fire resistant
        decisions.get(0).to(whiteWizard);
        decisions.get(2).to(whiteWizard);
        
        game.endTurn();
        assertEquals(14, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());
        
//        5        F     | P x                   S     x S +
        game.beginTurn();
        black.makeGesture(fingers, palm);
        white.makeGesture(snap, snap);
        game.validateTurn();
        game.endTurn();
        assertEquals(14, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());

//        6        F     x P        1            W       D x
        game.beginTurn();
        black.makeGesture(fingers, palm);
        white.makeGesture(wave, digit_pointing);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());

//        7        S       D                     W +   + D
        game.beginTurn();
        black.makeGesture(snap, digit_pointing);
        white.makeGesture(wave, digit_pointing);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());

//        8        S       W                     W |   | S +
        game.beginTurn();
        black.makeGesture(snap, wave);
        white.makeGesture(wave, snap);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());

//        9        F       F                   + S x   x F |
        game.beginTurn();
        black.makeGesture(fingers, fingers);
        white.makeGesture(snap, fingers);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());

//        10        P x    (C)                  x D  +    W x+
        game.beginTurn();
        black.makeGesture(palm, clap);
        white.makeGesture(digit_pointing, wave);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());

//        11       Stab   x P                     P x|    D  |
        game.beginTurn();
        black.makeGesture(stab, palm);
        white.makeGesture(palm, digit_pointing);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, blackWizard.getHealth());
        assertEquals(11, whiteWizard.getHealth());
    }
    
}
