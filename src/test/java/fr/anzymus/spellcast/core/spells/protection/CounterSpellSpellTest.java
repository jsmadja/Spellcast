package fr.anzymus.spellcast.core.spells.protection;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.Wizards;

public class CounterSpellSpellTest {

    @Test
    public void should_set_counter_spell_on_a_wizard() {
        CounterSpellSpell spell = new CounterSpellSpell();
        LivingEntity attacker = Wizards.create();
        assertFalse(attacker.hasCounterSpell());
        spell.castTo(attacker, attacker);
        assertTrue(attacker.hasCounterSpell());
    }
    
}
