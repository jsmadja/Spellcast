package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;

public class MissileSpell extends DamagingSpell {

    public MissileSpell() {
        this.spellGestures.add(snap);
        this.spellGestures.add(digit_pointing);
    }
    
    protected int getDamagePoints() {
        return 1;
    }
    
    @Override
    public String description() {
        return "This spell creates a material object of hard substance which is hurled towards the subject of the spell and causes him 1 point of damage. The spell is thwarted by a 'shield' in addition to the usual 'counter-spell', 'dispel magic' and 'magic mirror' (the latter causing it to hit whoever cast it instead).";
    }

}
