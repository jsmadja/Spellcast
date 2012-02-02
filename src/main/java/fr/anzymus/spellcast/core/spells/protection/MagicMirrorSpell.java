package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class MagicMirrorSpell extends AbstractSpell {

    public MagicMirrorSpell() {
        spellGestures.add(clap);
        spellGestures.add(wave, wave);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "Any spell cast at the subject of this spell is reflected back at the caster of that spell for that turn only. This includes spells like 'missile' and 'lightning bolt' but does not include attacks by monsters already in existence, or stabs from wizards. Note that certain spells are cast by wizards usually upon themselves, e.g. spells from this section and the \"Summons\" section, in which case the mirror would have no effect. It is countered totally, with no effect whatsoever, if the subject is the simultaneous subject of either a 'counter-spell' or 'dispel magic'. It has no effect on spells which affect more than one person, such as 'fire storm' and 2 mirrors cast at someone simultaneously combine to form a single mirror.";
    }
    
}
