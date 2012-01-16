package fr.anzymus.spellcast.core.gestures;

import static fr.anzymus.spellcast.core.gestures.Gesture.anything;
import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;

import java.util.List;

import org.junit.Test;

public class SpellGesturesTest {

    @Test
    public void should_have_0_gesture_at_beginning() {
        SpellGestures spellGestures = new SpellGestures();
        assertEquals(0, spellGestures.size());
    }

    @Test
    public void should_add_left_gesture_and_anything_gesture_for_right_hand() {
        SpellGestures spellGestures = new SpellGestures();
        spellGestures.add(clap);
        Gestures gestures = spellGestures.get(0);
        assertEquals(clap, gestures.getLeftHandGesture());
        assertEquals(anything, gestures.getRightHandGesture());
    }
    
    @Test
    public void should_add_left_gesture_and_right_gesture() {
        SpellGestures spellGestures = new SpellGestures();
        spellGestures.add(clap, fingers);
        Gestures gestures = spellGestures.get(0);
        assertEquals(clap, gestures.getLeftHandGesture());
        assertEquals(fingers, gestures.getRightHandGesture());
    }
    
    @Test
    public void should_transform_as_list() {
        SpellGestures spellGestures = new SpellGestures();
        spellGestures.add(clap, fingers);
        spellGestures.add(digit_pointing, wave);
        List<Gestures> list = spellGestures.asList();
        assertEquals(clap, list.get(0).getLeftHandGesture());
        assertEquals(fingers, list.get(0).getRightHandGesture());
        assertEquals(digit_pointing, list.get(1).getLeftHandGesture());
        assertEquals(wave, list.get(1).getRightHandGesture());
    }
}
