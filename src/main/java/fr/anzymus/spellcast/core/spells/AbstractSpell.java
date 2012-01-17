package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.anything;

import java.util.ArrayList;
import java.util.List;

import com.google.common.base.Joiner;

import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.gestures.SpellGestures;

public abstract class AbstractSpell implements Spell {

    protected SpellGestures spellGestures = new SpellGestures();

    protected boolean usableOnlyOnce;

    public List<Gestures> getGestures() {
        return spellGestures.asList();
    }
    
    @Override
    public boolean apply(GestureHistory gestureHistory) {
        if(gestureHistory == null) {
            return false;
        }
        List<Gestures> wizardGestures = gestureHistory.getLastGestures(spellGestures.size());
        if(wizardGestures.size() < spellGestures.size()) {
            return false;
        }
        if(isOneHandedSpell()) {
            return applyToLeftHand(wizardGestures) || applyToRightHand(wizardGestures);
        } else {
            return applyToHands(wizardGestures);
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
    
    public boolean isUsableOnlyOnce() {
        return usableOnlyOnce;
    }
}
