package fr.anzymus.spellcast.core;

import static org.apache.commons.lang3.StringUtils.isBlank;

import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.base.Joiner;

import fr.anzymus.spellcast.core.gestures.Gestures;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.Spells;

public class Game {

    private Logger log = LoggerFactory.getLogger(Game.class);
    
    private List<Player> players = new ArrayList<Player>();

    private Integer turn=0;
    
    public Game() {
        Spells.printSpellsInForwardOrder();
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

    public void endTurn() {
        for (Player player : players) {
            Wizard wizard = player.getWizard();
            Gestures lastGestures = wizard.getGestureHistory().lastGestures();
            log.info(lastGestures.toString());
            Spell spell = wizard.castSpell();
            if(spell != null) {
                LivingEntity target = otherPlayer(player);
                log.info(player+" cast "+spell.getClass().getSimpleName()+" to "+target);
                spell.castTo(target);
            }
            
        }
    }

    private LivingEntity otherPlayer(Player player) {
        for(Player p:players) {
            if(!p.equals(player)) {
                return p.getWizard();
            }
        }
        return null;
    }

}
