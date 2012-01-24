package fr.anzymus.spellcast.core.turn;

import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spell;

public class Decision {

    private Player player;
    private CastableSpell castableSpell;

    public Decision(Player player, CastableSpell spell) {
        this.player = player;
        this.castableSpell = spell;
    }
    
    public Decision(Player player, Spell spell) {
        this.player = player;
        this.castableSpell = new CastableSpell(spell, Hand.left);
    }

    public Decision() {
    }

    public Decision player(Player player) {
        this.player = player;
        return this;
    }

    public Decision castableSpell(CastableSpell spell) {
        this.castableSpell = spell;
        return this;
    }

    public void to(LivingEntity target) {
        player.cast(castableSpell.getSpell(), target);
    }

    public CastableSpell getCastableSpell() {
        return castableSpell;
    }

    public Player getPlayer() {
        return player;
    }

    @Override
    public String toString() {
        return  player+" could cast "+castableSpell.getSpell().name()+" with his "+castableSpell.getHand()+" hand";
    }
    
}
