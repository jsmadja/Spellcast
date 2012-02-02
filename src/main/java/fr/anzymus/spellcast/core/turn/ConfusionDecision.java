package fr.anzymus.spellcast.core.turn;

import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.gestures.Gesture;

public class ConfusionDecision {

    private Hand hand;
    private Gesture gesture;
    
    public ConfusionDecision(Hand hand, Gesture gesture) {
        this.hand = hand;
        this.gesture = gesture;
    }

    public Hand getHand() {
        return hand;
    }
    
    public Gesture getGesture() {
        return gesture;
    }
    
}
