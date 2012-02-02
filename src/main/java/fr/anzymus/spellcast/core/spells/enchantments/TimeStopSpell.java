package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class TimeStopSpell extends AbstractSpell {

    public TimeStopSpell() {
        spellGestures.add(snap);
        spellGestures.add(palm);
        spellGestures.add(palm);
        spellGestures.add(clap);
    }
    
    @Override
    public String description() {
        return "The subject of this spell immediately takes an extra turn, on which no-one can see or know about unless they are harmed. All non-affected beings have no resistance to any form of attack, e.g. a wizard halfway through the duration of a 'protection from evil' spell can be harmed by a monster which has had its time stopped. Time-stopped monsters attack whoever their controller instructs, and time-stopped elementals affect everyone, resistance to heat or cold being immaterial in that turn.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
