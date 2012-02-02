package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class RaiseDeadSpell extends AbstractSpell {

    public RaiseDeadSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(fingers);
        spellGestures.add(wave);
        spellGestures.add(clap);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "The subject of this spell is usually a recently-dead (not yet decomposing) human corpse though it may be used on a dead monster. When the spell is cast, life is instilled back into the corpse and any damage which it has sustained is cured until the owner is back to his usual state of health. A 'remove enchantment' effect is also manifest so any 'diseases' or 'poisons' will be neutralized (plus any other enchantments). If swords, knives, etc still remain in the corpse when it is raised, they will of course cause it damage as usual. The subject will be able to act as normal immediately under the influence of the caster of the 'raise dead' spell, so that next turn he can gesture, fight etc (in fact he will be able to fight on the turn upon which the spell is cast.) If the spell is cast on a live individual, the effect is that of a 'cure light wounds' recovering 5 points of damage, or as many as have been sustained if less than 5. This is the only spell which affects corpses properly. It therefore cannot be stopped in such circumstances by a 'counter-spell'. A 'dispel magic' spell will prevent it, since it affects all spells no matter what their subject. Once alive the subject is treated as normal.";
    }
    
}
