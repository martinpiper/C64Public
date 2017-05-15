package TestBDD;

import com.replicanet.cukesplus.junit.CucumberPlus;
import cucumber.api.CucumberOptions;
import org.junit.runner.RunWith;

@RunWith(CucumberPlus.class)
//@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue6502"} , format = {"pretty" , "html:target/cucumber"} , tags = {"~@ignore" , "@c64" , "~@tooLarge"}, features = {"features/"})
//@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue6502"} , format = {"pretty" , "html:target/cucumber"} , tags = {"~@ignore" , "@c64" , "~@tooLarge" , "@MazeSolving"}, features = {"features/"})
//@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue6502"} , format = {"pretty" , "html:target/cucumber"} , tags = {"~@ignore" , "@c64" , "~@tooLarge"}, features = {"features/Memory.feature"})
//@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue6502"} , format = {"pretty" , "html:target/cucumber"} , tags = {"~@ignore" , "@c64" , "~@tooLarge"}, features = {"features/MazeSolving4.feature"})
//@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue6502"} , format = {"pretty" , "html:target/cucumber"} , tags = {"~@ignore" , "@c64" , "~@tooLarge"}, features = {"features/Rand.feature"})
@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue6502"} , format = {"pretty" , "html:target/cucumber"} , tags = {"~@ignore" , "@c64" , "~@tooLarge"}, features = {"features/SubGame.feature"})
public class Run6502BDDTest
{
}
