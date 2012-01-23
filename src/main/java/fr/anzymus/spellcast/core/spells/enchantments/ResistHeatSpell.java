package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class ResistHeatSpell extends AbstractSpell {

    public ResistHeatSpell() {
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(fingers);
        spellGestures.add(palm);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        target.setResistHeat(true);
    }

    @Override
    public String description() {
        return "The subject of this spell becomes totally resistant to all forms of heat attack ('fireball', 'fire storm' and fire elementals). Only 'dispel magic' or 'remove enchantment' will terminate this resistance once started (although a 'counter-spell' will prevent it from working if cast at the subject at the same time as this spell). A 'resist heat' cast directly on a fire elemental will destroy it before it can attack that turn, but there is no effect on ice elementals.";
    }

}
