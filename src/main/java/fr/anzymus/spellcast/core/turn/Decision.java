package fr.anzymus.spellcast.core.turn;

import java.util.List;

import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spell;

public class Decision {

    private Wizard wizard;
    private CastableSpell castableSpell;
    private LivingEntity target;

    public Decision(Wizard wizard, CastableSpell spell) {
        this.wizard = wizard;
        this.castableSpell = spell;
    }
    
    public Decision(Wizard wizard, Spell spell) {
        this.wizard = wizard;
        this.castableSpell = new CastableSpell(spell, Hand.left);
    }

    public Decision() {
    }

    public Decision wizard(Wizard wizard) {
        this.wizard = wizard;
        return this;
    }

    public Decision castableSpell(CastableSpell spell) {
        this.castableSpell = spell;
        return this;
    }

    public void to(LivingEntity target) {
        this.target = target;
    }

    public CastableSpell getCastableSpell() {
        return castableSpell;
    }

    public Wizard getWizard() {
        return wizard;
    }
    
    public LivingEntity getTarget() {
        return target;
    }

    public void setTarget(LivingEntity target) {
        this.target = target;
    }
    
    @Override
    public String toString() {
        return  wizard.getName()+" could cast "+castableSpell.getSpell().name()+" with his "+castableSpell.getHand()+" hand";
    }

    public void apply(List<Wizard> wizards) {
        Spell spell = castableSpell.getSpell();
        if(target == null) {
            String spellName = spell.name();
            throw new IllegalStateException("Player "+wizard.getName()+" must choose a target for his "+spellName+" spell");
        }
        wizard.cast(spell, target);        
    }
    
}
