package fr.anzymus.spellcast.core.spells.damaging;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;


public class IceStormSpell extends DamagingSpell {

    public IceStormSpell() {
        spellGestures.add(wave);
        spellGestures.add(snap);
        spellGestures.add(snap);
        spellGestures.add(clap);
    }
    
    protected int getDamagePoints() {
        return 5;
    }

    @Override
    public String description() {
        return "Everything not resistant to cold sustains 5 points of damage that turn. The spell cancels wholly, causing no damage, with either a 'fire storm' or a fire elemental, and will cancel locally with a 'fireball'. It will destroy but not be destroyed by an ice elemental. Two 'ice storms' act as one.";
    }

}
