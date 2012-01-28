package fr.anzymus.spellcast.core.spells;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGoblinSpell;

public class SortSpellsByGestureLengthDescTest {

    SortSpellsByGestureLengthDesc comparator = new SortSpellsByGestureLengthDesc();
    
    @Test
    public void should_order_by_gesture_length_desc() {
        Spell shield = new ShieldSpell();
        Spell summonGoblin = new SummonGoblinSpell();
        assertEquals(1, comparator.compare(shield, summonGoblin));
    }
    
}
