package fr.anzymus.spellcast.core.spells.nonspells;

import static org.junit.Assert.assertEquals;

import org.junit.Before;
import org.junit.Test;

import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.WizardNotCreatedException;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Wizards;

public class StabSpellTest {

    Wizard player = Wizards.create();;
    Wizard target;
    Spell stab = new StabSpell();

    @Before
    public void init() {
        target = Wizards.create();
    }
    
    @Test
    public void should_remove_one_health_point() {
        player.cast(stab, target);
        assertEquals(target.getInitialHealth() - 1, target.getHealth());
    }
    
    @Test
    public void should_not_stab_a_wizard_who_has_a_shield() throws WizardNotCreatedException {
        target.setShield(true);
        player.cast(stab, target);
        assertEquals(target.getInitialHealth(), target.getHealth());
    }
    
    @Test
    public void should_not_stab_a_wizard_who_has_a_magic_mirror() throws WizardNotCreatedException {
        target.setMagicMirror(true);
        player.cast(stab, target);
        assertEquals(target.getInitialHealth(), target.getHealth());
    }
    
    @Test
    public void should_not_stab_himselft() throws WizardNotCreatedException {
        player.cast(stab, player);
        assertEquals(player.getInitialHealth(), player.getHealth());
    }
}
