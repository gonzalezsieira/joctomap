package es.usc.citius.lab.joctomap.module;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.util.AdjacencyMap;
import es.usc.citius.lab.joctomap.util.JOctomapLogger;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;

/**
 * Builder for {@link AdjacencyMap} taking a {@link JOctree}. 
 * This class instantiates an adjacency map taking the information of an octree
 * and stores the result in the file specified.
 * <ul>
 * 		<li>Obtain a new {@link JOctree} from a PPM image</li>
 * </ul>
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class AdjacencyMapBuilder extends Module{

    @Override
    public void execute(CommandLine args) {
        //load octree
        JOctomapLogger.info("Loading octree: " + args.getOptionValue("i"));
        JOctree octree = JOctree.read(args.getOptionValue("i"));
        //Creating adjacency map
        JOctomapLogger.info("Creating adjacency map...");
        AdjacencyMap map = new AdjacencyMap(octree);
        //store result
        JOctomapLogger.info("Storing to file: " + args.getOptionValue("o"));
        map.write(args.getOptionValue("o"));
        //release memory
        octree.dispose();
    }

    @Override
    public Options getModuleOptions() {
        Options op = getGenericOptions();
        op.addOption(
                Option.builder("i")
                        .required()
                        .desc("Input octree file")
                        .numberOfArgs(1)
                        .longOpt("input")
                        .build()
        );
        op.addOption(
                Option.builder("o")
                        .required()
                        .desc("Output file")
                        .numberOfArgs(1)
                        .longOpt("output")
                        .build()
        );
        return op;
    }
    
}
