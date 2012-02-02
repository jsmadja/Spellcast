package fr.anzymus.spellcast.core.spells.damaging.fire;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;
import fr.anzymus.spellcast.core.spells.enchantments.ResistHeatSpell;
import fr.anzymus.spellcast.core.turn.Decision;


public class FireDamagingSpellTest {

    FireDamagingSpell fireDamaging = new FireballSpell();
    ResistHeatSpell resistHeat = new ResistHeatSpell();
    
    @Test
    public void should_not_hurt_a_resist_heat_wizard_with_a_fire_damaging_spell() {
        Wizard attacker = new Wizard("attackerPlayer");
        Wizard attacked = new Wizard("attackedPlayer");
        LivingEntity target = Wizards.create();

        assertEquals(target.getInitialHealth(), target.getHealth());
        new Decision(attacked, resistHeat).to(target);
        new Decision(attacker, fireDamaging).to(target);
        
        assertEquals(target.getInitialHealth(), target.getHealth());
    }

}
