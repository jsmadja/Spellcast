package fr.anzymus.spellcast.core;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class PlayerTest {

    @Test
    public void should_have_a_wizard_with_14_life_point_at_beginning() {
        Player player = new Player("player");
        assertEquals(14, player.getWizard().getHealth());
    }
    
}
