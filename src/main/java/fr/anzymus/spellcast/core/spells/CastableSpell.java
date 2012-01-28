package fr.anzymus.spellcast.core.spells;

import fr.anzymus.spellcast.core.Hand;

public class CastableSpell {

    private Spell spell;
    private Hand hand;

    public CastableSpell(Spell spell, Hand hand) {
        this.spell = spell;
        this.hand = hand;
    }

    public Spell getSpell() {
        return spell;
    }

    public Hand getHand() {
        return hand;
    }

    @Override
    public String toString() {
        return spell.name()+" can be casted with "+hand+" hand"+(hand==Hand.both?"s":"");
    }
    
}
