package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class DispelMagicSpell extends AbstractSpell {

    public DispelMagicSpell() {
        spellGestures.add(clap);
        spellGestures.add(digit_pointing);
        spellGestures.add(palm);
        spellGestures.add(wave);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "This spell acts as a combination of 'counter-spell' and 'remove enchantment', but its effects are universal rather than limited to the subject of the spell. It will stop any spell cast in the same turn from working (apart from another dispel magic spell which combines with it for the same result), and will remove all enchantments from all beings before they have effect. In addition, all monsters are destroyed although they can attack that turn. 'Counter-spells' and 'magic mirrors' have no effect. The spell will not work on stabs or surrenders. As with a 'counter-spell' it also acts as a 'shield' for its subject.";
    }
    
}
