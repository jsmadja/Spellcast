package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;

public class ParalysisSpellTest {

    ParalysisSpell paralysisSpell = new ParalysisSpell();
    
    @Test
    public void should_not_apply_for_null_gesture_history() {
        assertFalse(paralysisSpell.apply(null));
    }
    
    @Test
    public void should_not_apply_for_empty_gesture_history() {
        assertFalse(paralysisSpell.apply(new GestureHistory()));
    }
    
    @Test
    public void should_not_apply_for_invalid_gesture_history() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap, clap);
        assertFalse(paralysisSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_apply_for_valid_gesture_history_in_left_hand() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(new Gestures(fingers, nothing));
        gestureHistory.add(new Gestures(fingers, nothing));
        gestureHistory.add(new Gestures(fingers, nothing));
        assertTrue(paralysisSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_apply_for_valid_gesture_history_in_right_hand() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(new Gestures(nothing,fingers));
        gestureHistory.add(new Gestures(nothing,fingers));
        gestureHistory.add(new Gestures(nothing,fingers));
        assertTrue(paralysisSpell.apply(gestureHistory));
    }
    
    @Test
    public void should_have_to_string() {
        assertEquals("F-F-F", paralysisSpell.toString());
    }
}
