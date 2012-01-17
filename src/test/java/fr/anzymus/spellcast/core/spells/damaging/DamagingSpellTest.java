package fr.anzymus.spellcast.core.spells.damaging;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;

public class DamagingSpellTest {

    DamagingSpell causeHeavyWoundsSpell = new CauseHeavyWoundsSpell();
    
    @Test
    public void should_remove_3_health_points() {
        Wizard wizard = Wizards.create();
        causeHeavyWoundsSpell.castTo(wizard);
        assertEquals(wizard.getInitialHealth() - 3, wizard.getHealth());
    }
    
}
