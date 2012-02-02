package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class DiseaseSpell extends AbstractSpell {

    public DiseaseSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(snap);
        spellGestures.add(fingers);
        spellGestures.add(fingers);
        spellGestures.add(fingers);
        spellGestures.add(clap);
    }
    
    @Override
    public String description() {
        return "The subject of this spell immediately contracts a deadly (non-contagious) disease which will kill him at the end of 6 turns counting from the one upon which the spell is cast. The malady is cured by 'remove enchantment' or 'cure heavy wounds' or 'dispel magic' in the meantime.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
