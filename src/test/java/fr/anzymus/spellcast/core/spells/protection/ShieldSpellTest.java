package fr.anzymus.spellcast.core.spells.protection;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Wizards;


public class ShieldSpellTest {

    Spell protectionSpell = new ShieldSpell();
 
    @Test
    public void should_add_shield_to_a_living_entity() {
        LivingEntity wizard1 = Wizards.create();
        assertFalse(wizard1.hasShield());
        protectionSpell.castTo(wizard1, wizard1);
        assertTrue(wizard1.hasShield());
    }
    
}
