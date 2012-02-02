package fr.anzymus.spellcast.core.spells;

import org.apache.commons.lang3.RandomStringUtils;

import fr.anzymus.spellcast.core.Wizard;

public class Wizards {

    public static Wizard create() {
        return new Wizard("Wizard "+RandomStringUtils.randomAlphabetic(7));
    }

}
