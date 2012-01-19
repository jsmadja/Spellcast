package fr.anzymus.spellcast.core;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.enchantments.ParalysisSpell;

public class WizardTest {

    @Test
    public void should_cast_paralysis_spell() {
        Wizard wizard = new Wizard(new Player("name"));
        wizard.makeGesture(fingers, nothing);
        wizard.makeGesture(fingers, nothing);
        wizard.makeGesture(fingers, nothing);
        List<Spell> spells = wizard.castSpells();
        assertTrue(spells.get(0) instanceof ParalysisSpell);
    }
    
    @Test
    public void should_remove_health() {
        Wizard wizard = new Wizard(new Player("name"));
        int initialHealth = wizard.getHealth();
        wizard.removeHealth(3);
        assertEquals(initialHealth - 3, wizard.getHealth());
    }
    
    @Test
    public void should_not_remove_more_than_min_health() {
        Wizard wizard = new Wizard(new Player("name"));
        wizard.removeHealth(30);
        assertEquals(0, wizard.getHealth());
    }
}
