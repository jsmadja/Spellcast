package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class HasteSpell extends AbstractSpell {

    public HasteSpell() {
        spellGestures.add(palm);
        spellGestures.add(wave);
        spellGestures.add(palm);
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(clap);
        }
    
    @Override
    public String description() {
        return "For the next 3 turns, the subject (but not his monsters if a wizard) makes an extra set of gestures due to being speeded up. This takes effect in the following turn so that instead of giving one pair of gestures, 2 are given, the effect of both being taken simultaneously at the end of the turn. Thus a single 'counter-spell' from his adversary could cancel 2 spells cast by the hastened wizard on 2 half-turns if the phasing is right. Non-hastened wizards and monsters can see everything the hastened individual is doing. Hastened monsters can change target in the extra turns if desired.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
