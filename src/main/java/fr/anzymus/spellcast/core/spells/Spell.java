package fr.anzymus.spellcast.core.spells;

import java.util.List;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.gestures.GestureHistory;
import fr.anzymus.spellcast.core.gestures.Gestures;

public interface Spell {

    boolean apply(GestureHistory gestureHistory);

    List<Gestures> getGestures();
    
    void castTo(LivingEntity attacker, LivingEntity target);
    
    String description();

    boolean isUsableOnlyOnceByGame();

}
