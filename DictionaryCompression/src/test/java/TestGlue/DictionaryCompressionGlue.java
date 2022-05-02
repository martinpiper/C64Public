package TestGlue;

import com.replicanet.compression.Main;
import cucumber.api.java.en.Given;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class DictionaryCompressionGlue {
    @Given("^compress files with dictionary size \"(.*)\" and target filename \"(.*)\" with skip \"(.*)\":$")
    public void compress_files(String size, String filename, String skip, List<String> names) throws Exception {
        ArrayList<String> args = new ArrayList<String>();
        args.add(size);
        args.add(filename);
        args.add(skip);
        args.addAll(names);
        String argsArray[] = args.toArray(new String[args.size()]);
        Main.main(argsArray);
    }
}
