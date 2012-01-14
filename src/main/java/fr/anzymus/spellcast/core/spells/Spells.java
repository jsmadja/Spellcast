package fr.anzymus.spellcast.core.spells;

import java.util.ArrayList;
import java.util.List;

public class Spells {

    public static List<Spell> createList() {
        List<Spell> spells = new ArrayList<Spell>();
        spells.add(new ParalysisSpell());
        spells.add(new SurrenderSpell());
        spells.add(new InvisibilitySpell());
        spells.add(new StabSpell());
        return spells;
    }

}
