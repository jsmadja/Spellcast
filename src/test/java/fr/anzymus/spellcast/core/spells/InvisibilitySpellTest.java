package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.gestures.GestureHistory;


public class InvisibilitySpellTest {

    InvisibilitySpell invisibilitySpell = new InvisibilitySpell();

    @Test
    public void should_print_invisibility_description() {
        assertEquals("P-P-(w-(s", invisibilitySpell.toString());
    }
    
    @Test
    public void should_apply_with_left_hand() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(palm);
        gestureHistory.add(palm);
        gestureHistory.add(wave,wave);
        gestureHistory.add(snap,snap);
         assertTrue(invisibilitySpell.apply(gestureHistory));
    }
    
    @Test
    public void should_apply_with_right_hand() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(nothing, palm);
        gestureHistory.add(nothing, palm);
        gestureHistory.add(wave,wave);
        gestureHistory.add(snap,snap);
         assertTrue(invisibilitySpell.apply(gestureHistory));
    }
    
    @Test
    public void should_apply_with_big_history() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(nothing, palm);
        gestureHistory.add(nothing, palm);
        gestureHistory.add(wave,wave);
        gestureHistory.add(snap,snap);
         assertTrue(invisibilitySpell.apply(gestureHistory));
    }
}
