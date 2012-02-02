package fr.anzymus.spellcast.core;

public abstract class LivingEntity {

    protected int health;
    
    private boolean paralyzing;
    
    private boolean shield;
    
    private boolean magicMirror;
    
    private String name;

    private boolean resistHeat;

    private boolean amnesic;

    private boolean counterSpell;
    
    private boolean confused;

    private boolean dispelMagic;
    
    protected LivingEntity() {
        health = getInitialHealth();
    }
    
    public abstract int getInitialHealth();

    public void removeHealth(int healthPoint) {
        health -= healthPoint;
        if(health < 0) {
            health = 0;
        }
    }    

    public int getHealth() {
        return health;
    }
    
    public boolean isParalyzing() {
        return paralyzing;
    }
    
    public void setParalyzing(boolean paralyzing) {
        this.paralyzing = paralyzing;
    }
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    @Override
    public String toString() {
        return name;
    }

    public void setShield(boolean shield) {
        this.shield = shield;
    }

    public boolean hasShield() {
        return shield;
    }

    public void setMagicMirror(boolean magicMirror) {
        this.magicMirror = magicMirror;
    }

    public boolean hasMagicMirror() {
        return magicMirror;
    }

    public void setResistHeat(boolean resistHeat) {
        this.resistHeat = resistHeat;
    }
    
    public boolean isResistHeat() {
        return resistHeat;
    }

    public boolean isAmnesic() {
        return amnesic;
    }

    public void setAmnesic(boolean amnesic) {
        this.amnesic = amnesic;
    }

    public boolean hasCounterSpell() {
        return counterSpell;
    }

    public void setCounterSpell(boolean counterSpell) {
        this.counterSpell = counterSpell;
    }

    public void setConfused(boolean confused) {
        this.confused = confused;
    }

    public boolean isConfused() {
        return confused;
    }

    public boolean hasDispelMagic() {
        return dispelMagic;
    }
    
    public void setDispelMagic(boolean dispelMagic) {
        this.dispelMagic = dispelMagic;
    }
}
