package fr.anzymus.spellcast.core;

import static org.apache.commons.lang3.StringUtils.isBlank;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.base.Joiner;

import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.CastableSpell;
import fr.anzymus.spellcast.core.spells.Spells;
import fr.anzymus.spellcast.core.turn.Decision;

public class Game {

    private Logger log = LoggerFactory.getLogger(Game.class);
    
    private List<Player> players = new ArrayList<Player>();

    private Integer turn=0;

    private boolean printSpellsAtBeginning = false;
    
    public Game() {
        if(printSpellsAtBeginning ) {
        Spells.printSpellsInForwardOrder();
        }
    }
    
    public Player createNewPlayer(String name) throws PlayerNotCreatedException {
        if(isBlank(name)) {
            throw new PlayerNotCreatedException("You must supply a valid name");
        }
        Player player = new Player(name);
        if(players.contains(player)) {
            throw new PlayerNotCreatedException("A player called "+name+" already exist");
        }
        players.add(player);
        return player;
    }

    public List<Player> getPlayers() {
        return players ;
    }

    public void beginTurn() {
        if(players.size() < 2) {
            throw new IllegalStateException("Game cannot begin without at least 2 players");
        }
        
        turn++;
        log.info("----------------------------------------------------------");
        log.info(turn.toString());
        List<String> wizardNames = new ArrayList<String>();
        for (Player player : players) {
            Wizard wizard = player.getWizard();
            wizardNames.add(wizard.getName()+" "+wizard.getHealth()+" HP");
        }
        log.info(Joiner.on('\t').join(wizardNames));
    }

    public List<Decision> validateTurn() {
        spellPreprocess();
        List<Decision> decisions = new ArrayList<Decision>();
        for (Player player : players) {
            List<CastableSpell> spellsToCast = detectSpellsToCast(player);
            for(CastableSpell spellToCast:spellsToCast) {
                decisions.add(new Decision(player, spellToCast));
            }
        }
        printDecisions(decisions);
        return decisions;
    }

    private void printDecisions(List<Decision> decisions) {
        for(Decision decision:decisions) {
            log.info(decision.toString());
        }
    }

    private List<CastableSpell> detectSpellsToCast(Player player) {
        Wizard wizard = player.getWizard();
        Gestures lastGestures = wizard.getGestureHistory().lastGestures();
        if (lastGestures == null) {
            throw new IllegalStateException("A turn cannot end if player "+player+" has not made any gesture");
        }
        log.info(lastGestures.toString());
        List<CastableSpell> spellsToCast = wizard.castSpells();
        return spellsToCast;
    }

    private void spellPreprocess() {
        repeatIdenticallyGesturesForAmnesics();
    }

    private void repeatIdenticallyGesturesForAmnesics() {
        for (Player player : players) {
            Wizard wizard = player.getWizard();
            if(wizard.isAmnesic()) {
                wizard.replaceGesturesByLastGestures();
                wizard.setAmnesic(false);
            }
        }
    }

    private void spellPostProcess() {
        removeShields();
    }

    private void removeShields() {
        for (Player player : players) {
            Wizard wizard = player.getWizard();
            wizard.setShield(false);
            List<Creature> creatures = wizard.getCreatures();
            for (Creature creature : creatures) {
                creature.setShield(false);
            }
        }
    }

    public void endTurn() {
        spellPostProcess();
    }

}
