package fr.anzymus.spellcast.core;

public abstract class LivingEntity {

    protected int health;
    
    private boolean paralyzing;
    
    protected LivingEntity() {
        health = getInitialHealth();
    }
    
    public abstract int getInitialHealth();

    public void removeHealth(int healtPoint) {
        health -= healtPoint;
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
}
