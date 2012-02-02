package fr.anzymus.spellcast.core;

import static org.apache.commons.lang3.StringUtils.isBlank;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.base.Joiner;

import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.gestures.Gesture;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spells;
import fr.anzymus.spellcast.core.turn.ConfusionDecision;
import fr.anzymus.spellcast.core.turn.CreatureDecision;
import fr.anzymus.spellcast.core.turn.Decision;
import fr.anzymus.spellcast.core.turn.Decisions;
import fr.anzymus.spellcast.core.turn.RandomConfusionDecision;

public class Game {

    private Logger log = LoggerFactory.getLogger(Game.class);

    private List<Wizard> wizards = new ArrayList<Wizard>();

    private Integer turn = 0;

    private boolean printSpellsAtBeginning = false;

    private Decisions decisions;

    private RandomConfusionDecision randomConfusionDecision = new RandomConfusionDecision();

    public Game() {
        if (printSpellsAtBeginning) {
            Spells.printSpellsInForwardOrder();
        }
    }

    public Wizard createNewWizard(String name) throws WizardNotCreatedException {
        if (isBlank(name)) {
            throw new WizardNotCreatedException("You must supply a valid name");
        }
        Wizard wizard = new Wizard(name);
        if (wizards.contains(wizard)) {
            throw new WizardNotCreatedException("A wizard called " + name + " already exist");
        }
        wizards.add(wizard);
        return wizard;
    }

    public List<Wizard> getWizards() {
        return wizards;
    }

    public void beginTurn() {
        if (wizards.size() < 2) {
            throw new IllegalStateException("Game cannot begin without at least 2 players");
        }

        turn++;
        log.info("----------------------------------------------------------");
        log.info(turn.toString());
        List<String> wizardNames = new ArrayList<String>();
        for (Wizard wizard : wizards) {
            wizardNames.add(wizard.toString());
        }
        log.info(Joiner.on('\t').join(wizardNames));
    }

    public Decisions validateTurn() {
        spellPreprocess();
        decisions = new Decisions();
        for (Wizard wizard : wizards) {
            List<CastableSpell> spellsToCast = detectSpellsToCast(wizard);
            for (CastableSpell spellToCast : spellsToCast) {
                decisions.add(new Decision(wizard, spellToCast));
            }
            List<Creature> creatures = wizard.getCreatures();
            for (Creature creature : creatures) {
                decisions.add(new CreatureDecision(wizard, creature));
            }
        }
        decisions.printDecisions();
        return decisions;
    }

    private List<CastableSpell> detectSpellsToCast(Wizard wizard) {
        Gestures lastGestures = wizard.getGestureHistory().getLastGestures();
        if (lastGestures == null) {
            throw new IllegalStateException("A turn cannot end if wizard " + wizard.getName()
                    + " has not made any gesture");
        }
        log.info(lastGestures.toString());
        List<CastableSpell> spellsToCast = wizard.castSpells();
        return spellsToCast;
    }

    private void spellPreprocess() {
        repeatIdenticallyGesturesForAmnesics();
        changeGestureForConfuseds();
    }

    private void repeatIdenticallyGesturesForAmnesics() {
        for (Wizard wizard : wizards) {
            if (wizard.isAmnesic()) {
                log.info(wizard.getName() + " is amnesic and repeat his last gestures");
                wizard.replaceGesturesByLastGestures();
                wizard.setAmnesic(false);
            }
        }
    }

    private void changeGestureForConfuseds() {
        for (Wizard wizard : wizards) {
            if (wizard.isConfused()) {
                ConfusionDecision randomDecision = randomConfusionDecision.random();
                Hand hand = randomDecision.getHand();
                Gesture gesture = randomDecision.getGesture();
                log.info(wizard.getName() + " is confused and his " + hand + " makes a " + gesture);
                wizard.replaceGestureBy(hand, gesture);
                wizard.setConfused(false);
            }
        }
    }

    private void spellPostProcess() {
        removeEnchantments();
    }

    private void removeEnchantments() {
        for (Wizard wizard : wizards) {
            wizard.setShield(false);
            wizard.setCounterSpell(false);
            wizard.setConfused(false);
            List<Creature> creatures = wizard.getCreatures();
            for (Creature creature : creatures) {
                creature.setShield(false);
                wizard.setCounterSpell(false);
                creature.setConfused(false);
            }
        }
    }

    public void endTurn() {
        applyDecisions();
        spellPostProcess();
        livingEntitiesPostProcess();
    }

    private void livingEntitiesPostProcess() {
        List<Wizard> deadWizards = new ArrayList<Wizard>();
        for (Wizard wizard : wizards) {
            List<Creature> deadCreatures = new ArrayList<Creature>();
            for(Creature creature:wizard.getCreatures()) {
                if(creature.getHealth() <= 0) {
                    log.info(creature.getName() +" died");
                    deadCreatures.add(creature);
                }
            }
            wizard.getCreatures().removeAll(deadCreatures);
            if(wizard.getHealth() <= 0) {
                log.info(wizard.getName() +" died");
                deadWizards.add(wizard);
            }
        }
        wizards.removeAll(deadWizards);
    }

    private void applyDecisions() {
        decisions.orderByPriority();
        for (Decision decision : decisions) {
            decision.apply(wizards);
        }
    }

}
