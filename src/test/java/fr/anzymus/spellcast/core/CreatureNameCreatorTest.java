package fr.anzymus.spellcast.core;

import static org.junit.Assert.assertEquals;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class CreatureNameCreatorTest {

    @InjectMocks
    CreatureNameCreator nameCreator;

    @Test
    public void create_goblin_name() {
        nameCreator.setGoblinPrefixes("sni");
        String name = nameCreator.createGoblinName();
        assertEquals("sni the Goblin", name);
    }

    @Test(timeout=100)
    public void create_goblin_name_with_random_prefix() {
        nameCreator.setGoblinPrefixes("sni", "sna","fer");

        boolean snifFound = false;
        boolean snaFound = false;
        boolean ferFound = false;

        while (!snifFound || !snaFound || !ferFound) {
            String name = nameCreator.createGoblinName();
            snifFound |= "sni the Goblin".equals(name);
            snaFound |= "sna the Goblin".equals(name);
            ferFound |= "fer the Goblin".equals(name);
        }
    }

}
