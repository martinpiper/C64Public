package TestBDD;

import com.replicanet.cukesplus.junit.CucumberPlus;
import cucumber.api.CucumberOptions;
import org.junit.runner.RunWith;

@RunWith(CucumberPlus.class)
@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue"} , tags = {"~@ignore" , "@java"} , format = {"pretty" , "html:target/cucumber"} , features = {"features"})
//@CucumberOptions(monochrome = true , glue = {"TestGlue", "MazeGlue"} , tags = {"~@ignore" , "@java"} , format = {"pretty" , "html:target/cucumber"} , features = {"features/MazeSolving4.feature"})
public class RunBDDTest
{
}
