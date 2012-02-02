package fr.anzymus.spellcast.core.spells.damaging.fire;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.Wizards;
import fr.anzymus.spellcast.core.spells.enchantments.ResistHeatSpell;
import fr.anzymus.spellcast.core.turn.Decision;

public class FireStormSpellTest {

    FireStormSpell fireStorm = new FireStormSpell();
    ResistHeatSpell resistHeat = new ResistHeatSpell();
    
    @Test
    public void should_remove_5_health_points() {
        Wizard attacker = Wizards.create();
        Wizard target = Wizards.create();
        fireStorm.castTo(attacker, target);
        assertEquals(target.getInitialHealth() - 5, target.getHealth());
    }
    
    @Test
    public void should_not_hurt_a_resist_heat_wizard_with_a_firestorm() {
        Wizard attacker = new Wizard("attackerPlayer");
        Wizard attacked = new Wizard("attackedPlayer");
        LivingEntity target = Wizards.create();

        assertEquals(target.getInitialHealth(), target.getHealth());
        new Decision(attacked, resistHeat).to(target);
        new Decision(attacker, fireStorm).to(target);
        
        assertEquals(target.getInitialHealth(), target.getHealth());
    }
    
}
