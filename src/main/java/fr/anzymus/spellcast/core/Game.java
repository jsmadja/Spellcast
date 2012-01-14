package fr.anzymus.spellcast.core;

import static org.apache.commons.lang3.StringUtils.isBlank;

import java.util.ArrayList;
import java.util.List;

public class Game {

    private List<Player> players = new ArrayList<Player>();

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

}
