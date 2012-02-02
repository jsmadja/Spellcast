package fr.anzymus.spellcast.core.spells.enchantments;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.Wizards;

public class ConfusionSpellTest {

    ConfusionSpell confusion = new ConfusionSpell();
    
    @Test
    public void should_cast_confusion() {
        LivingEntity attacker = Wizards.create();
        LivingEntity target = Wizards.create();
        
        assertFalse(target.isConfused());
        
        confusion.castTo(attacker, target);
        
        assertTrue(target.isConfused());
    }
    
}
