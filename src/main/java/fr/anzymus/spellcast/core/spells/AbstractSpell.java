package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.anything;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;

import com.google.common.base.Joiner;

import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.gestures.SpellGestures;

public abstract class AbstractSpell implements Spell {

    protected SpellGestures spellGestures = new SpellGestures();

    protected boolean usableOnlyOnceByGame;

    public List<Gestures> getGestures() {
        return spellGestures.asList();
    }
    
    @Override
    public Hand apply(GestureHistory gestureHistory) {
        if(gestureHistory == null) {
            return Hand.none;
        }
        List<Gestures> wizardGestures = gestureHistory.getLastGestures(spellGestures.size());
        if(wizardGestures.size() < spellGestures.size()) {
            return Hand.none;
        }
        if(isOneHandedSpell()) {
            if(applyToLeftHand(wizardGestures)) {
                return Hand.left;
            }
            if(applyToRightHand(wizardGestures)) {
                return Hand.right;
            }
            return Hand.none;
        } else {
            return applyToHands(wizardGestures)?Hand.both:Hand.none;
        }
    }

    private boolean isOneHandedSpell() {
        for(Gestures gestures:spellGestures) {
            if (gestures.getRightHandGesture() != anything) {
                return false;
            }
        }
        return true;
    }

    private boolean applyToHands(List<Gestures> wizardGestures) {
        return checkIfSpellBeginsWithLeftHand(wizardGestures) || checkIfSpellBeginsWithRightHand(wizardGestures);
    }

    private boolean checkIfSpellBeginsWithLeftHand(List<Gestures> wizardGestures) {
        for(int i=0;i<spellGestures.size();i++) {
            Gesture expectedLeftGesture = spellGestures.get(i).getLeftHandGesture();
            Gesture expectedRightGesture = spellGestures.get(i).getRightHandGesture();
            Gesture wizardLeftGesture = wizardGestures.get(i).getLeftHandGesture();
            Gesture wizardRightGesture = wizardGestures.get(i).getRightHandGesture();
            if(expectedLeftGesture != wizardLeftGesture) {
                return false;
            }
            if(expectedRightGesture != anything && expectedRightGesture != wizardRightGesture) {
                return false;
            }
        }
        return true;
    }

    private boolean checkIfSpellBeginsWithRightHand(List<Gestures> wizardGestures) {
        for(int i=0;i<spellGestures.size();i++) {
            Gesture expectedLeftGesture = spellGestures.get(i).getLeftHandGesture();
            Gesture expectedRightGesture = spellGestures.get(i).getRightHandGesture();
            Gesture wizardLeftGesture = wizardGestures.get(i).getLeftHandGesture();
            Gesture wizardRightGesture = wizardGestures.get(i).getRightHandGesture();
            if(expectedLeftGesture != wizardRightGesture) {
                return false;
            }
            if(expectedRightGesture != anything && expectedRightGesture != wizardLeftGesture) {
                return false;
            }
        }
        return true;
    }

    private boolean applyToLeftHand(List<Gestures> wizardGestures) {
        for(int i=0;i<spellGestures.size();i++) {
            Gesture expectedLeftGesture = spellGestures.get(i).getLeftHandGesture();
            Gesture expectedRightGesture = spellGestures.get(i).getRightHandGesture();
            Gesture wizardLeftGesture = wizardGestures.get(i).getLeftHandGesture();
            Gesture wizardRightGesture = wizardGestures.get(i).getRightHandGesture();
            if(expectedLeftGesture != wizardLeftGesture) {
                return false;
            }
            if(expectedRightGesture != wizardRightGesture && expectedRightGesture != anything) {
                return false;
            }
        }
        return true;
    }
    
    private boolean applyToRightHand(List<Gestures> wizardGestures) {
        for(int i=0;i<spellGestures.size();i++) {
            Gesture expectedLeftGesture = spellGestures.get(i).getLeftHandGesture();
            Gesture wizardRightGesture = wizardGestures.get(i).getRightHandGesture();
            if(expectedLeftGesture != wizardRightGesture) {
                return false;
            }
            if(expectedLeftGesture != wizardRightGesture && expectedLeftGesture != anything) {
                return false;
            }
        }
        return true;
    }

    @Override
    public String toString() {
        List<String> descriptions = new ArrayList<String>();
        for(Gestures gesture:spellGestures) {
            Gesture leftHandGesture = gesture.getLeftHandGesture();
            Gesture rightHandGesture = gesture.getRightHandGesture();
            if(leftHandGesture == rightHandGesture) {
                descriptions.add("("+leftHandGesture.getDescription().toLowerCase());
            } else {
                descriptions.add(leftHandGesture.getDescription());
            }
        }
        return Joiner.on('-').join(descriptions);
    }
    
    public boolean isUsableOnlyOnceByGame() {
        return usableOnlyOnceByGame;
    }
    
    @Override
    public Integer getPriority() {
        return Integer.MAX_VALUE;
    }
    
    @Override
    public String name() {
        String className = this.getClass().getSimpleName();
        String spellName = StringUtils.remove(className, "Spell");
        return spellName;
    }
}
