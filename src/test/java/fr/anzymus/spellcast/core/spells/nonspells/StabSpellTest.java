package fr.anzymus.spellcast.core.spells.nonspells;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.Wizards;

public class StabSpellTest {

    StabSpell stabSpell = new StabSpell();
    
    @Test
    public void should_remove_one_health_point() {
        LivingEntity wizard = Wizards.create();
        stabSpell.castTo(wizard);
        assertEquals(wizard.getInitialHealth() - 1, wizard.getHealth());
    }
    
}
