package fr.anzymus.spellcast.core.turn;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;


public class RandomsTest {

    @Test
    public void should_get_a_random_number_between_0_and_10() {
        int value = Randoms.getRandomInteger(10);
        assertTrue(value >= 0 && value<=10);
    }
    
    @Test
    public void should_get_a_0_when_max_is_negative() {
        int value = Randoms.getRandomInteger(-10);
        assertEquals(0, value);
    }
    
}
