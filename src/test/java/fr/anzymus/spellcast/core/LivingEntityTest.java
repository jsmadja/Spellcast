package fr.anzymus.spellcast.core;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.spells.Wizards;

public class LivingEntityTest {

    @Test
    public void should_remove_health() {
        LivingEntity livingEntity = Wizards.create();
        int initialHealth = livingEntity.getHealth();
        livingEntity.removeHealth(3);
        assertEquals(initialHealth - 3, livingEntity.getHealth());
    }
    
    @Test
    public void should_not_remove_more_health_than_initial() {
        LivingEntity livingEntity = Wizards.create();
        int initialHealth = livingEntity.getHealth();
        livingEntity.removeHealth(initialHealth * 2);
        assertEquals(0, livingEntity.getHealth());
    }
}
