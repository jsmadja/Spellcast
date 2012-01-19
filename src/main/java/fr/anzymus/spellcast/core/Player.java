package fr.anzymus.spellcast.core;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.spells.Spell;

public class Player {

    private Logger log = LoggerFactory.getLogger(Player.class);

    private Wizard wizard = new Wizard(this);
    private String name;
    private boolean surrender;

    public Player(String name) {
        this.name = name;
        wizard.setName(name);
    }

    public Wizard getWizard() {
        return wizard;
    }
    
    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Player) {
            return name.equals(((Player)obj).name);
        }
        return false;
    }

    public void makeGesture(Gesture leftHandGesture, Gesture rightHandGesture) {
        wizard.makeGesture(leftHandGesture, rightHandGesture);
    }

    public void surrender() {
        this.surrender = true;
    }
    
    public boolean hasSurrendered() {
        return surrender;
    }
    
    @Override
    public String toString() {
        return name;
    }

    public void cast(Spell spell, LivingEntity target) {
        if(spell != null) {
            log.info(this+" cast "+spell.getClass().getSimpleName()+" to "+target);
            spell.castTo(wizard, target);
        }
    }
}
