package fr.anzymus.spellcast.core.turn;

import java.util.List;

import fr.anzymus.spellcast.core.LivingEntity;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;

public class CreatureDecision extends Decision {

    private Creature creature;
    private Wizard wizard;

    public CreatureDecision(Wizard wizard, Creature creature) {
        this.creature = creature;
        this.wizard = wizard;
    }

    @Override
    public String toString() {
        return creature+" could attack somebody";
    }

    public Wizard getWizard() {
        return wizard;
    }
    
    public Creature getCreature() {
        return creature;
    }
    
    public void apply(List<Wizard> wizards) {
        if(getTarget() == null) {
            throw new IllegalStateException("Creature "+creature.getName()+" must choose a target to attack");
        }
        if (creature.isConfused()) {
            chooseRandomTarget(wizards);
        }
        creature.attack(getTarget());
    }

    private void chooseRandomTarget(List<Wizard> wizards) {
        LivingEntity randomTarget = pickRandomTarget(wizards);
        setTarget(randomTarget);
    }
    
    private LivingEntity pickRandomTarget(List<Wizard> wizards) {
        int size = wizards.size();
        int random = Randoms.getRandomInteger(size);
        return wizards.get(random);
    }

}
