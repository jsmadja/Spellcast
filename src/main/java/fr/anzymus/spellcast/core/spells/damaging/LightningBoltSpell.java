package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;


public class LightningBoltSpell extends DamagingSpell {

    public LightningBoltSpell() {
        spellGestures.add(digit_pointing);
        spellGestures.add(fingers);
        spellGestures.add(fingers);
        spellGestures.add(digit_pointing);
        spellGestures.add(digit_pointing);
    }
    
    protected int getDamagePoints() {
        return 5;
    }

    @Override
    public String description() {
        return "The subject of this spell is hit by a bolt of lightning and sustains 5 points of damage. Resistance to heat or cold is irrelevant. There are 2 gesture combinations for the spell, but the shorter one may be used only once per day (i.e. per battle) by any wizard. The longer one may be used without restriction. A 'shield' spell offers no defence.";
    }

}
