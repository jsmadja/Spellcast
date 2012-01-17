package fr.anzymus.spellcast.core.spells.summons;

import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.creature.Elemental;

public class SummonElementalSpell extends SummonSpell {

    public SummonElementalSpell() {
        spellGestures.add(clap);
        spellGestures.add(snap);
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(snap);
    }
    
    protected Creature summonCreature(Wizard owner) {
        return new Elemental(owner);
    }

    @Override
    public String description() {
        return "This spell creates either a fire elemental or an ice elemental at the discretion of the person upon whom the spell is cast after he has seen all the gestures made that turn. Elementals must be cast at someone and cannot be \"shot off\" harmlessly at some inanimate object. The elemental will, for that turn and until destroyed, attack everyone who is not resistant to its type (heat or cold), causing 3 points of damage per turn. The elemental takes 3 points of damage to be killed but may be destroyed by spells of the opposite type (e.g. 'fire storm', 'resist cold' or 'fireball' will kill an ice elemental), and will also neutralize the canceling spell. Elementals will not attack on the turn they are destroyed by such a spell. An elemental will also be engulfed and destroyed by a storm of its own type but, in such an event, the storm is not neutralized although the elemental still does not attack in that turn. 2 elementals of the opposite type will also destroy each other before attacking, and 2 of the same type will join together to form a single elemental of normal strength. Note that only wizards or monsters resistant to the type of elemental, or who are casting a spell which has the effect of a 'shield' do not get attacked by the elemental. Casting a 'fireball' upon yourself when being attacked by an ice elemental is no defence! (Cast it at the elemental...)";
    }

}
