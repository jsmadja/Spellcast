package fr.anzymus.spellcast.core.spells.damaging.fire;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.damaging.DamagingSpell;


public class FireballSpell extends DamagingSpell {

    private Logger log = LoggerFactory.getLogger(FireballSpell.class);
    
    public FireballSpell() {
        spellGestures.add(fingers);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(digit_pointing);
        spellGestures.add(digit_pointing);
    }
    
    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        if(!target.isResistHeat()) {
            super.castTo(attacker, target);
        } else {
            log.info(target+" resists to heat");
        }
    }
    
    protected int getDamagePoints() {
        return 5;
    }

    @Override
    public String description() {
        return "The subject of this spell is hit by a ball of fire and sustains 5 points of damage unless he is resistant to fire. If at the same time an 'ice storm' prevails, the subject of the 'fireball' is instead not harmed by either spell, although the storm will affect others as normal. If directed at an ice elemental, the fireball will destroy it before it can attack, but has no other effect on the creatures.";
    }

}
