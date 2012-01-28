package fr.anzymus.spellcast.core.spells;

import java.util.Comparator;

public class SortSpellsByGestureLengthDesc implements Comparator<Spell> {

    @Override
    public int compare(Spell spell1, Spell spell2) {
        if(spell1.getGestures().size() == spell2.getGestures().size()) {
            return 0;
        }
        if(spell1.getGestures().size() < spell2.getGestures().size()) {
            return 1;
        }
        return -1;
    }

}
