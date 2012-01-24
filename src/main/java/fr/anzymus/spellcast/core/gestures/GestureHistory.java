package fr.anzymus.spellcast.core.gestures;

import static fr.anzymus.spellcast.core.gestures.Gesture.nothing;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

public class GestureHistory {

    private Stack<Gestures> gestureStack = new Stack<Gestures>();

    public Gestures lastGestures() {
        return gestureStack.peek();
    }

    public void add(Gesture leftGesture) {
        add(new Gestures(leftGesture, nothing));
    }

    public void add(Gesture leftGesture, Gesture rightGesture) {
        add(new Gestures(leftGesture, rightGesture));
    }
    
    public void add(Gestures gestures) {
        gestureStack.add(gestures);
    }

    public List<Gestures> getLastGestures(int i) {
        if(gestureStack.isEmpty()) {
            return new ArrayList<Gestures>();
        }
        int size = gestureStack.size();
        int begin = size<i?0:(size-i);
        int end = size;
        return gestureStack.subList(begin, end);
    }
    

    public Gestures getLastGestures() {
        List<Gestures> lastGestures = getLastGestures(1);
        return lastGestures.isEmpty() ? null : lastGestures.get(0);
    }
    
    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder("Gesture History").append("\n");
        for(Gestures gestures:gestureStack) {
            Gesture leftHandGesture = gestures.getLeftHandGesture();
            Gesture rightHandGesture = gestures.getRightHandGesture();
            builder.append(leftHandGesture).append(" ").append(rightHandGesture).append("\n");
        }
        return builder.toString();
    }

    public int size() {
        return gestureStack.size();
    }

    public Gestures removeLastGestures() {
        if(!gestureStack.isEmpty()) {
            return gestureStack.pop();
        }
        return null;
    }

}
