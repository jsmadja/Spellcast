package fr.anzymus.spellcast.core.turn;

import static org.junit.Assert.assertNotNull;

import org.junit.Test;


public class RandomConfusionDecisionTest {

    RandomConfusionDecision randomConfusionDecision = new RandomConfusionDecision();
    
    @Test
    public void should_random_confusion_decision() {
        ConfusionDecision confusionDecision = randomConfusionDecision.random();
        System.out.println("confused hand: "+confusionDecision.getHand());
        System.out.println("confused gesture: "+confusionDecision.getGesture());
        assertNotNull(confusionDecision.getHand());
        assertNotNull(confusionDecision.getGesture());
    }
    
}
