package fr.anzymus.spellcast.core.spells.protection;

import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.gestures.SpellGestures;

public class CounterSpellOtherVersionSpell extends CounterSpellSpell {

    public CounterSpellOtherVersionSpell() {
        spellGestures = new SpellGestures();
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(snap);
    }
    
}
