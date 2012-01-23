package fr.anzymus.spellcast.core.spells.damaging.fire;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;

public class FireStormSpell extends FireDamagingSpell {

    public FireStormSpell() {
        spellGestures.add(snap);
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(clap);
    }

    protected int getDamagePoints() {
        return 5;
    }

    @Override
    public String description() {
        return "Everything not resistant to heat sustains 5 points of damage that turn. The spell cancels wholly, causing no damage, with either an 'ice storm' or an ice elemental. It will destroy but not be destroyed by a fire elemental. Two 'fire storms' act as one.";
    }

}
