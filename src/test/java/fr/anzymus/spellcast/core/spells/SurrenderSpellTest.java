package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.gestures.GestureHistory;

public class SurrenderSpellTest {

    SurrenderSpell surrenderSpell = new SurrenderSpell();
    
    @Test
    public void should_apply_to_surrender() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(palm, palm);
        assertTrue(surrenderSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_apply_to_surrender_big_history() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap, clap);
        gestureHistory.add(palm, palm);
        assertTrue(surrenderSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_not_apply_to_surrender_because_of_right_hand() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(palm, clap);
        assertFalse(surrenderSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_not_apply_to_surrender_because_of_left_hand() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap, palm);
        assertFalse(surrenderSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_print_surrender_description() {
        assertEquals("(p", surrenderSpell.toString());
    }
}
