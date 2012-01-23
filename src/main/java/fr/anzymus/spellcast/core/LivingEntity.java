package fr.anzymus.spellcast.core;

public abstract class LivingEntity {

    protected int health;
    
    private boolean paralyzing;
    
    private boolean shield;
    
    private boolean magicMirror;
    
    private String name;

    private boolean resistHeat;
    
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

    public void setResistHeat(boolean resistHeaht) {
        this.resistHeat = true;
    }
    
    public boolean isResistHeat() {
        return resistHeat;
    }
}
