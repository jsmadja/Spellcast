package fr.anzymus.spellcast.core.spells.nonspells;

import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class StabSpell extends AbstractSpell {

    public StabSpell() {
        spellGestures.add(stab);
    }

    public void castTo(LivingEntity target) {
        target.removeHealth(1);
    }

    @Override
    public String description() {
        return "This is not a spell but an attack which can be directed at any individual monster or wizard. Unless protected in that turn by a 'shield' spell or another spell with the same effect, the being stabbed suffers 1 point of damage. The wizard only has one knife so can only stab with one hand in any turn, although which hand doesn't matter. The stab cannot be reflected by a 'magic mirror' or stopped by 'dispel magic' (although its 'shield' effect *could* stop the stab). Wizards are not allowed to stab themselves and must choose a target for the stab. Knives cannot be thrown.";
    }

}
