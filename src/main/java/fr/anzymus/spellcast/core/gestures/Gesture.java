package fr.anzymus.spellcast.core.gestures;

public enum Gesture {
    
    fingers("F"), //
    palm("P"), //
    wave("W"), //
    snap("S"), //
    digit_pointing("D"), //
    clap("C"), //
    nothing(" "), //
    stab("stab"),
    anything("anything", false);

    private String description;
    private boolean playable = true;

    private Gesture(String description) {
        this.description = description;
    }
    
    private Gesture(String description, boolean playable) {
        this(description);
        this.playable = playable;
    }
    
    public String getDescription() {
        return description;
    }
    
    public boolean isPlayable() {
        return playable;
    }
}
