package fr.anzymus.spellcast.core.gestures;

public class Gestures {

    private Gesture leftHandGesture;
    private Gesture rightHandGesture;

    public Gestures(Gesture leftHandGesture, Gesture rightHandGesture) {
        this.leftHandGesture = leftHandGesture;
        this.rightHandGesture = rightHandGesture;
    }

    public Gesture getLeftHandGesture() {
        return leftHandGesture;
    }
    
    public Gesture getRightHandGesture() {
        return rightHandGesture;
    }
    
    public void setLeftHandGesture(Gesture leftHandGesture) {
        this.leftHandGesture = leftHandGesture;
    }
    
    public void setRightHandGesture(Gesture rightHandGesture) {
        this.rightHandGesture = rightHandGesture;
    }
    
    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Gestures) {
            Gestures g = (Gestures)obj;
            return leftHandGesture == g.leftHandGesture && rightHandGesture == g.rightHandGesture;
        }
        return false;
    }
    
    @Override
    public String toString() {
        return leftHandGesture.toString() +" "+ rightHandGesture.toString();
    }
}
