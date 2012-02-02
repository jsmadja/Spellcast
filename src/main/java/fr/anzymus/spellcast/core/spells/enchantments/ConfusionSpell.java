package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class ConfusionSpell extends AbstractSpell {

    public ConfusionSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(snap);
        spellGestures.add(fingers);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        target.setConfused(true);
    }

    @Override
    public String description() {
        return "If the subject of this spell is a wizard, next turn he writes down his gestures as usual and after exposing them, rolls 2 dice to determine which gesture is superseded due to his being confused. The first die indicates left hand on 1-3, right on 4-6. The second roll determines what the gesture for that hand shall be replaced with: 1=C, 2=D, 3=F, 4=P, 5=S, 6=W. If the subject of the spell is a monster, it attacks at random that turn. If the subject is also the subject of any of: 'amnesia', 'charm person', 'charm monster', 'paralysis' or 'fear', none of the spells work.";
    }

}
