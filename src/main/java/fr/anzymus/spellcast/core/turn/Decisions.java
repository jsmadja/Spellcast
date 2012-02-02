package fr.anzymus.spellcast.core.turn;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import fr.anzymus.spellcast.core.Hand;
import fr.anzymus.spellcast.core.Wizard;
import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spell;

public class Decisions implements Iterable<Decision> {

    private Logger log = LoggerFactory.getLogger(Decisions.class);

    private List<Decision> decisions = new ArrayList<Decision>();

    public Decision cast(Wizard wizard, Class<? extends Spell> spellClass) {
        for (Decision decision : decisions) {
            if (isWizardDecision(decision)) {
                CastableSpell castableSpell = decision.getCastableSpell();
                Spell spell = castableSpell.getSpell();
                Class<? extends Spell> decisionSpellClass = spell.getClass();
                if (decision.getWizard().equals(wizard) && decisionSpellClass.equals(spellClass)) {
                    return decision;
                }
            }
        }
        log.error(wizard.getGestureHistory().toString());
        throw new IllegalAccessError(wizard.getName() + " is not able to cast " + spellClass.getSimpleName());
    }
    
    public Decision cast(Wizard wizard, Class<? extends Spell> spellClass, Hand hand) {
        for (Decision decision : decisions) {
            if (isWizardDecision(decision)) {
                CastableSpell castableSpell = decision.getCastableSpell();
                Spell spell = castableSpell.getSpell();
                Class<? extends Spell> decisionSpellClass = spell.getClass();
                if (decision.getWizard().equals(wizard) && decisionSpellClass.equals(spellClass) && hand == castableSpell.getHand()) {
                    return decision;
                }
            }
        }
        log.error(wizard.getGestureHistory().toString());
        throw new IllegalAccessError(wizard.getName() + " is not able to cast " + spellClass.getSimpleName());
    }

    private boolean isWizardDecision(Decision decision) {
        return !(decision instanceof CreatureDecision);
    }

    public void add(Decision decision) {
        decisions.add(decision);
    }

    public void printDecisions() {
        for (Decision decision : decisions) {
            log.info(decision.toString());
        }
    }

    public Decision attack(Creature creature) {
        for (Decision decision : decisions) {
            if (decision instanceof CreatureDecision) {
                CreatureDecision creatureDecision = (CreatureDecision) decision;
                Wizard wizard = creatureDecision.getWizard();
                List<Creature> creatures = wizard.getCreatures();
                for (Creature creature2 : creatures) {
                    if (creature2.equals(creature)) {
                        return decision;
                    }
                }
            }
        }
        throw new IllegalAccessError(creature.getName() + " is not able to attack");
    }

    @Override
    public Iterator<Decision> iterator() {
        return decisions.iterator();
    }

    public void orderByPriority() {
        if (decisions != null && decisions.size() > 1) {
            Collections.sort(decisions, new Comparator<Decision>() {
                @Override
                public int compare(Decision o1, Decision o2) {
                    CastableSpell castableSpell1 = o1.getCastableSpell();
                    CastableSpell castableSpell2 = o2.getCastableSpell();
                    if (castableSpell1 != null && castableSpell2 != null) {
                        Spell spell1 = castableSpell1.getSpell();
                        Spell spell2 = castableSpell2.getSpell();
                        if (spell1 != null && spell2 != null) {
                            return spell1.getPriority().compareTo(spell2.getPriority());
                        }
                    }
                    return 0;
                }
            });
        }
    }

    public int size() {
        return decisions.size();
    }
}
