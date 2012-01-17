package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.anything;
import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.damaging.CauseHeavyWoundsSpell;
import fr.anzymus.spellcast.core.spells.enchantments.InvisibilitySpell;
import fr.anzymus.spellcast.core.spells.enchantments.ParalysisSpell;
import fr.anzymus.spellcast.core.spells.nonspells.SurrenderSpell;

public class AbstractSpellTest {

    Spell surrenderSpell = new SurrenderSpell();
    Spell invisibilitySpell = new InvisibilitySpell();
    Spell paralysisSpell = new ParalysisSpell();
    Spell causeHeavyWoundsSpell = new CauseHeavyWoundsSpell();
    
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

    @Test
    public void should_transform_as_list() {
        List<Gestures> gestures = paralysisSpell.getGestures();
        assertEquals(new Gestures(fingers, anything), gestures.get(0));
        assertEquals(new Gestures(fingers, anything), gestures.get(1));
        assertEquals(new Gestures(fingers, anything), gestures.get(2));
    }
    

    @Test
    public void should_recognize_spell() {
        GestureHistory gestureHistory= new GestureHistory();
        gestureHistory.add(wave, wave);
        gestureHistory.add(wave, palm);
        gestureHistory.add(digit_pointing, fingers);
        gestureHistory.add(digit_pointing, digit_pointing);
        
        boolean apply = causeHeavyWoundsSpell.apply(gestureHistory);
        assertTrue(apply);
    }

}
