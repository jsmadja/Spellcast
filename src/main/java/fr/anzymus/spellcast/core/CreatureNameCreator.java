package fr.anzymus.spellcast.core;

public class CreatureNameCreator {

    private String[] goblinPrefixes = { "sni", "sna", "fer", "fi", "fir", "por", "per", "snu", "al", "an", "erl",
            "lep", "fru", "fri", "ig", "eg", "thi", "tha" };

    public void setGoblinPrefixes(String... goblinPrefixes) {
        this.goblinPrefixes = goblinPrefixes;
    }

    public String createGoblinName() {
        int i = (int) (Math.random() * 100) % goblinPrefixes.length;
        return goblinPrefixes[i] + " the Goblin";
    }

}
