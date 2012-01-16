package fr.anzymus.spellcast.core.gestures;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import org.junit.Test;


public class GesturesTest {

    @Test
    public void should_format_gestures() {
        Gestures gestures = new Gestures(clap, digit_pointing);
        assertEquals("clap digit_pointing", gestures.toString());
    }
    
    @Test
    public void should_verify_getters() {
        Gestures gestures = new Gestures(clap, digit_pointing);
        assertEquals(clap, gestures.getLeftHandGesture());
        assertEquals(digit_pointing, gestures.getRightHandGesture());
    }
    
    @Test
    public void should_be_equal() {
        Gestures gestures1 = new Gestures(clap, digit_pointing);
        Gestures gestures2 = new Gestures(clap, digit_pointing);
        assertEquals(gestures1, gestures2);
    }
    
    @Test
    public void should_not_be_equal() {
        Gestures gestures1 = new Gestures(clap, digit_pointing);
        Gestures gestures2 = new Gestures(snap, digit_pointing);
        assertFalse(gestures1.equals(gestures2));
    }
    
    @Test
    public void should_not_be_equal_With_string() {
        Gestures gestures1 = new Gestures(clap, digit_pointing);
        assertFalse(gestures1.equals(""));
    }
    
}
