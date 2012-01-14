package fr.anzymus.spellcast.core.spells;

import static fr.anzymus.spellcast.core.gestures.Gesture.anything;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.Gestures;

public class SpellGestures implements Iterable<Gestures>{

    private List<Gestures> gestures = new ArrayList<Gestures>();
    
    public void add(Gesture leftHandGesture) {
        gestures.add(new Gestures(leftHandGesture, anything));
    }

    public void add(Gesture leftHandGesture, Gesture rightHandGesture) {
        gestures.add(new Gestures(leftHandGesture, rightHandGesture));
    }

    public List<Gestures> asList() {
        return gestures;
    }

    public int size() {
        return gestures.size();
    }

    @Override
    public Iterator<Gestures> iterator() {
        return gestures.iterator();
    }

    public Gestures get(int i) {
        return gestures.get(i);
    }

}
