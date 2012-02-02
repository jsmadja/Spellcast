package fr.anzymus.spellcast.core.spells.enchantments;

import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.spells.AbstractSpell;

public class ProtectionFromEvilSpell extends AbstractSpell {

    public ProtectionFromEvilSpell() {
        spellGestures.add(wave);
        spellGestures.add(wave);
        spellGestures.add(palm);
    }
    
    @Override
    public String description() {
        return "For this turn and the following 3 turns the subject of this spell is protected as if using a 'shield' spell, thus leaving both hands free. Concurrent 'shield' spells offer no further protection and compound 'protection from evil' spells merely overlap offering no extra cover.";
    }

    @Override
    public void castTo(LivingEntity attacker, LivingEntity target) {
    }

}
