package fr.anzymus.spellcast.core.turn;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.spells.Spell;

public class Decision {

    private Player player;
    private Spell spell;

    public Decision(Player player, Spell spell) {
        this.player = player;
        this.spell = spell;
    }

    public Decision() {
    }

    public Decision player(Player player) {
        this.player = player;
        return this;
    }

    public Decision castSpell(Spell spell) {
        this.spell = spell;
        return this;
    }

    public void to(LivingEntity target) {
        player.cast(spell, target);
    }

}
