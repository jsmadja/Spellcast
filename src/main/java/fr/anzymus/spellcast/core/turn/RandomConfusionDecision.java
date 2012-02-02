package fr.anzymus.spellcast.core.turn;

import static fr.anzymus.spellcast.core.Hand.left;
import static fr.anzymus.spellcast.core.Hand.right;
import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.gestures.Gesture;

public class RandomConfusionDecision {

    private Gesture[] gestures = {clap, digit_pointing, fingers, palm, snap, wave};
    private Hand[] hands = {left, right};

    public ConfusionDecision random() {
        Hand hand = randomHand();
        Gesture gesture = randomGesture();
        return new ConfusionDecision(hand, gesture);
    }

    private Gesture randomGesture() {
        int randomNumber = Randoms.getRandomInteger(6);
        return gestures[randomNumber];
    }

    private Hand randomHand() {
        int randomNumber = Randoms.getRandomInteger(2);
        return hands[randomNumber];
    }

}
