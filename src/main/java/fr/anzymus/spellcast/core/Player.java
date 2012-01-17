package fr.anzymus.spellcast.core;

import fr.anzymus.spellcast.core.gestures.Gesture;

public class Player {

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
}
