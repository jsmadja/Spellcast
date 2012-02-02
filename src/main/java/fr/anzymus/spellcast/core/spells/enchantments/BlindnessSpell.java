package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class BlindnessSpell extends AbstractSpell {

    public BlindnessSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(wave);
        spellGestures.add(fingers);
        spellGestures.add(fingers);
        spellGestures.add(digit_pointing, digit_pointing);
    }
    
    @Override
    public String description() {
        return "For the next 3 turns not including the one in which the spell was cast, the subject is unable to see. If he is a wizard, he cannot tell what his opponent's gestures are, although he must be informed of any which affect him (e.g. summons spells, 'missile' etc cast at him) but not 'counter- spells' to his own 'attacks. Indeed he will not know if his own spells work unless they also affect him (e.g. a 'fire storm' when he isn't resistant to fire.) He can control his monsters (e.g. \"Attack whatever it was that just attacked me\"). Blinded monsters are instantly destroyed and cannot attack in that turn.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
