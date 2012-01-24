package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.Creatures;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Wizards;


public class AmnesiaSpellTest {

    Spell amnesia = new AmnesiaSpell();
    
    @Test
    public void should_cast_amnesia_on_a_wizard() {
        LivingEntity attacker = Wizards.create();
        LivingEntity target = Wizards.create();
        assertFalse(target.isAmnesic());
        amnesia.castTo(attacker, target);
        assertTrue(target.isAmnesic());
    }
    
    @Test
    public void should_cast_amnesia_on_a_creature() {
        LivingEntity attacker = Wizards.create();
        LivingEntity target = Creatures.create();
        assertFalse(target.isAmnesic());
        amnesia.castTo(attacker, target);
        assertTrue(target.isAmnesic());
    }
}
