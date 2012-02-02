package fr.anzymus.spellcast.core.spells.nonspells;

import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class SurrenderSpell extends AbstractSpell {

    public SurrenderSpell() {
        spellGestures.add(palm, palm);
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
        if(target instanceof Wizard) {
            Wizard wizard = (Wizard) target;
            wizard.surrender();
        }
    }

    @Override
    public String description() {
        return "This is not a spell and consequently cannot be cast at anyone. The wizard making these gestures, irrespective of whether they terminate spells or not, surrenders and the contest is over. The surrendering wizard is deemed to have lost unless his gestures completed spells which killed his opponent. Two simultaneous surrenders count as a draw. It is a skill for wizards to work their spells so that they never accidentally perform 2 P gestures simultaneously. Wizards can be killed as they surrender if hit with appropriate spells or attacked physically, but the \"referees\" will cure any diseases, poisons etc immediately after the surrender for them.";
    }
    
    @Override
    public Integer getPriority() {
        return -1;
    }
    
}
