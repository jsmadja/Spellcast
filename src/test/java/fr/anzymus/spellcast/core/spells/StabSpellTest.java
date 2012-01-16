package fr.anzymus.spellcast.core.spells;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;

public class StabSpellTest {

    StabSpell stabSpell = new StabSpell();
    
    @Test
    public void should_remove_one_health_point() {
        LivingEntity wizard = Wizards.create();
        stabSpell.castTo(wizard);
        assertEquals(wizard.getInitialHealth() - 1, wizard.getHealth());
    }
    
}
