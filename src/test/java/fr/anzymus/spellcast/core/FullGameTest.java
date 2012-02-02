package fr.anzymus.spellcast.core;

import static fr.anzymus.spellcast.core.Hand.left;
import static fr.anzymus.spellcast.core.Hand.right;
import static fr.anzymus.spellcast.core.gestures.Gesture.clap;
import static fr.anzymus.spellcast.core.gestures.Gesture.digit_pointing;
import static fr.anzymus.spellcast.core.gestures.Gesture.fingers;
import static fr.anzymus.spellcast.core.gestures.Gesture.palm;
import static fr.anzymus.spellcast.core.gestures.Gesture.snap;
import static fr.anzymus.spellcast.core.gestures.Gesture.stab;
import static fr.anzymus.spellcast.core.gestures.Gesture.wave;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import fr.anzymus.spellcast.core.creature.Creature;
import fr.anzymus.spellcast.core.spells.Spell;
import fr.anzymus.spellcast.core.spells.damaging.CauseHeavyWoundsSpell;
import fr.anzymus.spellcast.core.spells.damaging.LightningBoltShorterVersionSpell;
import fr.anzymus.spellcast.core.spells.damaging.MissileSpell;
import fr.anzymus.spellcast.core.spells.damaging.fire.FireStormSpell;
import fr.anzymus.spellcast.core.spells.enchantments.AmnesiaSpell;
import fr.anzymus.spellcast.core.spells.enchantments.AntiSpellSpell;
import fr.anzymus.spellcast.core.spells.enchantments.ConfusionSpell;
import fr.anzymus.spellcast.core.spells.enchantments.InvisibilitySpell;
import fr.anzymus.spellcast.core.spells.enchantments.ResistHeatSpell;
import fr.anzymus.spellcast.core.spells.nonspells.StabSpell;
import fr.anzymus.spellcast.core.spells.nonspells.SurrenderSpell;
import fr.anzymus.spellcast.core.spells.protection.CounterSpellOtherVersionSpell;
import fr.anzymus.spellcast.core.spells.protection.MagicMirrorSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGoblinSpell;
import fr.anzymus.spellcast.core.turn.Decisions;

//@Ignore
public class FullGameTest {

    private Class<? extends Spell> shield = ShieldSpell.class;
    private Class<? extends Spell> missile = MissileSpell.class;
    private Class<? extends Spell> summonGoblin = SummonGoblinSpell.class;
    private Class<? extends Spell> resistHeat = ResistHeatSpell.class;
    private Class<? extends Spell> lightningBolt = LightningBoltShorterVersionSpell.class;
    private Class<? extends Spell> causeHeavyWounds = CauseHeavyWoundsSpell.class;
    private Class<? extends Spell> counterSpellOtherVersion = CounterSpellOtherVersionSpell.class;
    private Class<? extends Spell> amnesia = AmnesiaSpell.class;
    private Class<? extends Spell> confusion = ConfusionSpell.class;
    private Class<? extends Spell> stabSpell = StabSpell.class;
    private Class<? extends Spell> invisibility = InvisibilitySpell.class;
    private Class<? extends Spell> antiSpell = AntiSpellSpell.class;
    private Class<? extends Spell> fireStorm = FireStormSpell.class;
    private Class<? extends Spell> magicMirror = MagicMirrorSpell.class;
    private Class<? extends Spell> surrender = SurrenderSpell.class;

    @Test
    public void full_scenario() throws WizardNotCreatedException {

        // Turn BLACK WHITE
        Game game = new Game();
        Wizard black = game.createNewWizard("BLACK");
        Wizard white = game.createNewWizard("WHITE");

        // Left Right Damage Left Right Damage
        // 1 W W + 0 W + P 0
        game.beginTurn();
        black.makeGesture(wave, wave);
        white.makeGesture(wave, palm);
        Decisions decisions = game.validateTurn();
        decisions.cast(white, shield).to(white);
        game.endTurn();
        assertEquals(14, black.getHealth());
        assertEquals(14, white.getHealth());

        // 2 W P | W | D
        game.beginTurn();
        black.makeGesture(wave, palm);
        white.makeGesture(wave, digit_pointing);
        decisions = game.validateTurn();
        decisions.cast(black, shield).to(black);
        game.endTurn();
        assertEquals(14, black.getHealth());
        assertEquals(14, white.getHealth());

        // 3 D F | F | + W
        game.beginTurn();
        black.makeGesture(digit_pointing, fingers);
        white.makeGesture(fingers, wave);
        game.validateTurn();
        game.endTurn();
        assertEquals(14, black.getHealth());
        assertEquals(14, white.getHealth());

        // 4 D + D x P x | W 3
        game.beginTurn();
        black.makeGesture(digit_pointing, digit_pointing);
        white.makeGesture(palm, wave);
        decisions = game.validateTurn();
        // white becomes fire resistant
        decisions.cast(white, resistHeat).to(white);
        decisions.cast(black, causeHeavyWounds).to(white);

        game.endTurn();
        assertEquals(14, black.getHealth());
        assertEquals(11, white.getHealth());

        // 5 F | P x S x S +
        game.beginTurn();
        black.makeGesture(fingers, palm);
        white.makeGesture(snap, snap);
        decisions = game.validateTurn();
        decisions.cast(black, shield).to(black);
        decisions.cast(white, counterSpellOtherVersion).to(black);
        game.endTurn();
        assertEquals(14, black.getHealth());
        assertEquals(11, white.getHealth());

        // 6 F x P 1 W D x
        game.beginTurn();
        black.makeGesture(fingers, palm);
        white.makeGesture(wave, digit_pointing);
        decisions = game.validateTurn();
        decisions.cast(black, amnesia).to(white);
        decisions.cast(white, missile).to(black);
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        // 7 S D W + + D
        game.beginTurn();
        black.makeGesture(snap, digit_pointing);
        white.makeGesture(wave, digit_pointing);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        // 8 S W W | | S +
        game.beginTurn();
        black.makeGesture(snap, wave);
        white.makeGesture(wave, snap);
        game.validateTurn();
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        // 9 F F + S x x F |
        game.beginTurn();
        black.makeGesture(fingers, fingers);
        white.makeGesture(snap, fingers);
        decisions = game.validateTurn();
        decisions.cast(white, counterSpellOtherVersion).to(white);
        decisions.cast(white, confusion).to(black);
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        adaptBlackGesturesDueToConfusion(black);

        // 10 P x (C) x D + W x+
        game.beginTurn();
        black.makeGesture(palm, clap);
        white.makeGesture(digit_pointing, wave);
        decisions = game.validateTurn();
        decisions.cast(white, summonGoblin).to(white);
        decisions.cast(white, missile).to(black);
        decisions.cast(black, shield).to(black);
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        Creature goblin = white.getCreatures().get(0);

        // 11 Stab x P P x| D |
        game.beginTurn();
        black.makeGesture(stab, palm);
        white.makeGesture(palm, digit_pointing);
        decisions = game.validateTurn();
        decisions.cast(black, shield).to(black);
        decisions.attack(goblin).to(black);
        decisions.cast(black, stabSpell).to(goblin);
        decisions.cast(white, shield).to(white);
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        // 12 P + S P x D |
        game.beginTurn();
        black.makeGesture(palm, snap);
        white.makeGesture(palm, digit_pointing);
        decisions = game.validateTurn();
        decisions.cast(black, shield).to(black);
        decisions.cast(white, amnesia).to(black);
        game.endTurn();
        assertEquals(13, black.getHealth());
        assertEquals(11, white.getHealth());

        // 13 P | S 6 C C x
        game.beginTurn();
        black.makeGesture(digit_pointing, palm);
        white.makeGesture(clap, clap);
        decisions = game.validateTurn();
        decisions.cast(black, shield).to(black);
        decisions.cast(white, lightningBolt).to(black);
        game.endTurn();
        assertEquals(8, black.getHealth());
        assertEquals(11, white.getHealth());

        // 14 W | W S + + S
        game.beginTurn();
        black.makeGesture(wave, wave);
        white.makeGesture(snap, snap);
        game.validateTurn();
        game.endTurn();
        assertEquals(8, black.getHealth());
        assertEquals(11, white.getHealth());

        // 15 + S x + S W | | P x
        game.beginTurn();
        black.makeGesture(snap, snap);
        white.makeGesture(wave, palm);
        decisions = game.validateTurn();
        decisions.cast(white, shield).to(white);
        decisions.cast(black, invisibility).to(black);
        game.endTurn();
        assertEquals(8, black.getHealth());
        assertEquals(11, white.getHealth());

        // 16 x D x D W | x F 5
        game.beginTurn();
        black.makeGesture(digit_pointing, digit_pointing);
        white.makeGesture(wave, fingers);
        decisions = game.validateTurn();
        decisions.cast(black, missile, left).to(white);
        decisions.cast(black, missile, right).to(white);
        decisions.cast(white, antiSpell).to(black);
        game.endTurn();
        assertEquals(8, black.getHealth());
        assertEquals(9, white.getHealth());

        // 17 W + S + 11 C x C
        game.beginTurn();
        black.makeGesture(wave, snap);
        white.makeGesture(clap, clap);
        decisions = game.validateTurn();
        decisions.cast(white, fireStorm).to(black);
        game.endTurn();
        assertEquals(3, black.getHealth());
        assertEquals(9, white.getHealth());

        // 18 D | P | + S + S
        game.beginTurn();
        black.makeGesture(digit_pointing, palm);
        white.makeGesture(snap, snap);
        decisions = game.validateTurn();
        decisions.cast(black, shield).to(black);
        game.endTurn();
        assertEquals(3, black.getHealth());
        assertEquals(9, white.getHealth());

        // 19 D | F x 13 x D x D
        game.beginTurn();
        black.makeGesture(digit_pointing, fingers);
        white.makeGesture(digit_pointing, digit_pointing);
        decisions = game.validateTurn();
        decisions.cast(black, antiSpell).to(black);
        decisions.cast(white, missile, left).to(black);
        decisions.cast(white, missile, right).to(black);
        game.endTurn();
        assertEquals(1, black.getHealth());
        assertEquals(9, white.getHealth());

        // 20 C x + C S + S 10
        game.beginTurn();
        black.makeGesture(clap, clap);
        white.makeGesture(snap, snap);
        decisions = game.validateTurn();
        decisions.cast(black, lightningBolt).to(white);
        game.endTurn();
        assertEquals(1, black.getHealth());
        assertEquals(4, white.getHealth());

        // 21 W x W P | W
        game.beginTurn();
        black.makeGesture(wave, wave);
        white.makeGesture(palm, wave);
        decisions = game.validateTurn();
        decisions.cast(white, shield).to(white);
        decisions.cast(black, magicMirror).to(black);
        game.endTurn();
        assertEquals(1, black.getHealth());
        assertEquals(4, white.getHealth());

        // 22 P P F x W
        game.beginTurn();
        black.makeGesture(palm, palm);
        white.makeGesture(fingers, wave);
        decisions = game.validateTurn();
        decisions.cast(black, surrender).to(black);
        decisions.cast(white, antiSpell).to(black);
        game.endTurn();
        assertTrue(black.hasSurrendered());
    }

    private void adaptBlackGesturesDueToConfusion(Wizard black) {
        black.getGestureHistory().removeLastGestures();
        black.getGestureHistory().add(palm, clap);
    }

}
