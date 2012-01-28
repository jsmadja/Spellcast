package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class CounterSpellOtherVersionSpell extends AbstractSpell {

    public CounterSpellOtherVersionSpell() {
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(snap);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        
    }

    @Override
    public String description() {
        return "Any other spell cast upon the subject in the same turn has no effect whatever. In the case of blanket-type spells, which affect more than 1 person, the subject of the 'counter-spell' alone is protected. For example, a 'fire storm' spell could kill off a monster but not if it was simultaneously the subject of a 'counter-spell' although everyone else would be affected as usual unless they had their own protection. The 'counter-spell' will cancel all the spells cast at the subject for that turn including 'remove enchantment' and 'magic mirror' but not 'dispel magic' or 'finger of death'. It will combine with another spell of its own type for the same effect as if it were alone. The 'counter-spell' will also act as a 'shield' on the final gesture in addition to its other properties, but the shield effect is on the same subject as its other effect. The spell has 2 alternative gesture sequences, either of which may, be used at any time.";
    }

}
