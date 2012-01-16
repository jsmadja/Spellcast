package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Goblin;

public class SummonGoblinSpell extends AbstractSpell {

    public SummonGoblinSpell() {
        this.spellGestures.add(snap);
        this.spellGestures.add(fingers);
        this.spellGestures.add(wave);
    }
    
    @Override
    public void castTo(LivingEntity target) {
        if(target instanceof Wizard) {
            Wizard wizard = (Wizard) target;
            wizard.summonCreature(new Goblin(wizard));
        }
    }

    @Override
    public String description() {
        return "This spell creates a goblin under the control of the subject upon whom the spell is cast (or if cast on a monster, *its* controller, even if the monster later dies or changes loyalty). The goblin can attack immediately and its victim can be any any wizard or other monster the controller desires, stating which at the time he writes his gestures. It does 1 point of damage to its victim per turn and is destroyed after 1 point of damage is inflicted upon it. The summoning spell cannot be cast at an elemental, and if cast at something which doesn't exist, the spell has no effect.";
    }

}
