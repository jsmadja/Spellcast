package fr.anzymus.spellcast.core.spells;

import fr.anzymus.spellcast.core.Player;
import fr.anzymus.spellcast.core.Wizard;

public class Wizards {

    public static Wizard create() {
        return new Wizard(new Player("name"));
    }

}
