package fr.anzymus.spellcast.core;

public abstract class LivingEntity {

    protected int health;
    
    private boolean paralyzing;
    
    private String name;
    
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
}
