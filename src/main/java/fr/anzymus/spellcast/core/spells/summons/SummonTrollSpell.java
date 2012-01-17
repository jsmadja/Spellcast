package fr.anzymus.spellcast.core.spells.summons;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Troll;

public class SummonTrollSpell extends SummonSpell {

    public SummonTrollSpell() {
        spellGestures.add(fingers);
        spellGestures.add(palm);
        spellGestures.add(snap);
        spellGestures.add(fingers);
        spellGestures.add(wave);
    }
    
    protected Creature summonCreature(Wizard owner) {
        return new Troll(owner);
    }

    @Override
    public String description() {
        return "This spell is the same as 'summon goblin' but the troll created inflicts and is destroyed by 3 points of damage rather than 1.";
    }

}
