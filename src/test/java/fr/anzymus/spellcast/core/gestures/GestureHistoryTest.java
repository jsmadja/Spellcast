package fr.anzymus.spellcast.core.gestures;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

public class GestureHistoryTest {

    @Test
    public void should_return_size_of_0() {
        GestureHistory gestureHistory = new GestureHistory();
        assertEquals(0, gestureHistory.size());
    }
    
    @Test
    public void should_return_size_of_1() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap);
        assertEquals(1, gestureHistory.size());
    }
    
    @Test
    public void should_return_empty_list_for_empty_history() {
        GestureHistory gestureHistory = new GestureHistory();
        assertTrue(gestureHistory.getLastGestures(5).isEmpty());
    }

    @Test
    public void should_return_one_element_list_for_one_element_history() {
        GestureHistory gestureHistory = new GestureHistory();
        Gestures gestures = new Gestures(clap, clap);
        gestureHistory.add(gestures);

        Gestures gesturesFromHistory = gestureHistory.getLastGestures(1).get(0);

        assertEquals(gestures, gesturesFromHistory);
    }

    @Test
    public void should_return_two_elements_list_for_two_elements_history() {
        GestureHistory gestureHistory = new GestureHistory();
        Gestures gestures1 = new Gestures(clap, clap);
        Gestures gestures2 = new Gestures(fingers, fingers);
        gestureHistory.add(gestures1);
        gestureHistory.add(gestures2);

        Gestures gestures1FromHistory = gestureHistory.getLastGestures(2).get(0);
        Gestures gestures2FromHistory = gestureHistory.getLastGestures(2).get(1);

        assertEquals(gestures1, gestures1FromHistory);
        assertEquals(gestures2, gestures2FromHistory);
    }
    
    @Test
    public void should_get_only_last_gestures() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap, clap);
        gestureHistory.add(fingers, fingers);
        gestureHistory.add(digit_pointing, fingers);
        
        List<Gestures> lastGestures = gestureHistory.getLastGestures(1);
        assertEquals(1, lastGestures.size());
        assertEquals(digit_pointing, lastGestures.get(0).getLeftHandGesture());
        assertEquals(fingers, lastGestures.get(0).getRightHandGesture());
    }
    
    @Test
    public void should_build_tostring() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap, clap);
        gestureHistory.add(fingers, fingers);
        gestureHistory.add(digit_pointing, fingers);
        assertEquals("Gesture History\nclap clap\nfingers fingers\ndigit_pointing fingers\n", gestureHistory.toString());
    }
    
    @Test
    public void should_not_fail_when_removing_last_gestures_of_empty_history() {
        GestureHistory gestureHistory = new GestureHistory();
        Gestures gestures = gestureHistory.removeLastGestures();
        assertNull(gestures);
    }
    
    @Test
    public void should_remove_last_gestures_of_history_of_size_1() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap);
        assertEquals(1, gestureHistory.size());
        Gestures gestures = gestureHistory.removeLastGestures();
        assertEquals(clap, gestures.getLeftHandGesture());
        assertEquals(nothing, gestures.getRightHandGesture());
        assertEquals(0, gestureHistory.size());
    }
    
    @Test
    public void should_remove_last_gestures_of_history_of_size_2() {
        GestureHistory gestureHistory = new GestureHistory();
        gestureHistory.add(clap);
        gestureHistory.add(snap);
        assertEquals(2, gestureHistory.size());
        Gestures gestures = gestureHistory.removeLastGestures();
        assertEquals(snap, gestures.getLeftHandGesture());
        assertEquals(nothing, gestures.getRightHandGesture());
        assertEquals(1, gestureHistory.size());
    }
}
