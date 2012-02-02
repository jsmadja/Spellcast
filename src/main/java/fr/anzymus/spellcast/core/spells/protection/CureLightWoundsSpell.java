package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class CureLightWoundsSpell extends AbstractSpell {

    public CureLightWoundsSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(fingers);
        spellGestures.add(wave);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

    @Override
    public String description() {
        return "If the subject has received damage then he is cured by 1 point as if that point had not been inflicted. Thus, for example, if a wizard was at 10 points of damage and was hit simultaneously by a 'cure light wounds' and a 'lightning bolt' he would finish that turn on 14 points rather than 15 (or 9 if there had been no 'lightning bolt'). The effect is not removed by a 'dispel magic' or 'remove enchantment'.";
    }
    
}
