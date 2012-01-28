package fr.anzymus.spellcast.core.spells;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import fr.anzymus.spellcast.core.spells.damaging.CauseHeavyWoundsSpell;
import fr.anzymus.spellcast.core.spells.damaging.CauseLightWoundsSpell;
import fr.anzymus.spellcast.core.spells.damaging.FingerOfDeathSpell;
import fr.anzymus.spellcast.core.spells.damaging.IceStormSpell;
import fr.anzymus.spellcast.core.spells.damaging.LightningBoltShorterVersionSpell;
import fr.anzymus.spellcast.core.spells.damaging.LightningBoltSpell;
import fr.anzymus.spellcast.core.spells.damaging.MissileSpell;
import fr.anzymus.spellcast.core.spells.damaging.fire.FireStormSpell;
import fr.anzymus.spellcast.core.spells.damaging.fire.FireballSpell;
import fr.anzymus.spellcast.core.spells.enchantments.AmnesiaSpell;
import fr.anzymus.spellcast.core.spells.enchantments.InvisibilitySpell;
import fr.anzymus.spellcast.core.spells.enchantments.ParalysisSpell;
import fr.anzymus.spellcast.core.spells.enchantments.ResistHeatSpell;
import fr.anzymus.spellcast.core.spells.nonspells.StabSpell;
import fr.anzymus.spellcast.core.spells.nonspells.SurrenderSpell;
import fr.anzymus.spellcast.core.spells.protection.CounterSpellOtherVersionSpell;
import fr.anzymus.spellcast.core.spells.protection.CounterSpellSpell;
import fr.anzymus.spellcast.core.spells.protection.ShieldSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonElementalSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGiantSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonGoblinSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonOgreSpell;
import fr.anzymus.spellcast.core.spells.summons.SummonTrollSpell;

public class Spells {

    private static Comparator<Spell> byGestureLengthDesc = new SortSpellsByGestureLengthDesc();

    public static List<Spell> createList() {
        List<Spell> spells = new ArrayList<Spell>();
        spells.add(new AmnesiaSpell());
        spells.add(new CauseHeavyWoundsSpell());
        spells.add(new CauseLightWoundsSpell());
        spells.add(new CounterSpellSpell());
        spells.add(new CounterSpellOtherVersionSpell());
        spells.add(new FingerOfDeathSpell());
        spells.add(new FireballSpell());
        spells.add(new FireStormSpell());
        spells.add(new IceStormSpell());
        spells.add(new InvisibilitySpell());
        spells.add(new LightningBoltShorterVersionSpell());
        spells.add(new LightningBoltSpell());
        spells.add(new MissileSpell());
        spells.add(new ParalysisSpell());
        spells.add(new ResistHeatSpell());
        spells.add(new ShieldSpell());
        spells.add(new StabSpell());
        spells.add(new SummonElementalSpell());
        spells.add(new SummonGiantSpell());
        spells.add(new SummonGoblinSpell());
        spells.add(new SummonOgreSpell());
        spells.add(new SummonTrollSpell());
        spells.add(new SurrenderSpell());
        Collections.sort(spells, byGestureLengthDesc);
        return spells;
    }

    public static void printSpellsInForwardOrder() {
        for(Spell spell:createList()) {
            System.out.println(spell.toString()+" "+spell.getClass().getSimpleName());
        }
    }

}
