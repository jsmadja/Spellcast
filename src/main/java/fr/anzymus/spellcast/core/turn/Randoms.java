package fr.anzymus.spellcast.core.turn;

public class Randoms {

    public static int getRandomInteger(int max) {
        if(max <=0) {
            return 0;
        }
        int randomNumber = ((int) (Math.random() * 10D))%max;
        return randomNumber;
    }

}
