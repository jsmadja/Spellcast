package fr.anzymus.spellcast.core;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Wizards;
import fr.anzymus.spellcast.core.spells.enchantments.ParalysisSpell;
import fr.anzymus.spellcast.core.spells.enchantments.ResistHeatSpell;

public class WizardTest {

    @Test
    public void should_cast_paralysis_spell() {
        Wizard wizard = new Wizard(new Player("name"));
        wizard.makeGesture(fingers, nothing);
        wizard.makeGesture(fingers, nothing);
        wizard.makeGesture(fingers, nothing);
        List<CastableSpell> spells = wizard.castSpells();
        assertTrue(spells.get(0).getSpell() instanceof ParalysisSpell);
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
    
    @Test
    public void should_repeat_last_gestures() {
        Wizard wizard = Wizards.create();
        wizard.makeGesture(snap, clap);
        wizard.makeGesture(palm, palm);
        
        assertEquals(2, wizard.getGestureHistory().size());
        Gestures lastGesture = wizard.getGestureHistory().getLastGestures();
        assertEquals(palm, lastGesture.getLeftHandGesture());
        assertEquals(palm, lastGesture.getRightHandGesture());

        wizard.replaceGesturesByLastGestures();

        assertEquals(2, wizard.getGestureHistory().size());
        lastGesture = wizard.getGestureHistory().getLastGestures();
        assertEquals(snap, lastGesture.getLeftHandGesture());
        assertEquals(clap, lastGesture.getRightHandGesture());
    }
    
    @Test
    public void should_cast_the_longer_spell() {
        Wizard wizard = Wizards.create();
        wizard.makeGesture(wave, nothing);
        wizard.makeGesture(wave, nothing);
        wizard.makeGesture(fingers, nothing);
        wizard.makeGesture(palm, nothing);
        List<CastableSpell> castableSpells = wizard.castSpells();
        for (CastableSpell castableSpell : castableSpells) {
            System.out.println(castableSpell);
        }
        assertEquals(1, castableSpells.size());
        assertEquals(ResistHeatSpell.class, castableSpells.get(0).getSpell().getClass());
    }
}
