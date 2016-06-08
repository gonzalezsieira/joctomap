/**
 * Copyright (C) 2014-2016 Adrián González Sieira (adrian.gonzalez@usc.es)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package es.usc.citius.lab.joctomap.module;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.util.AdjacencyMap;
import es.usc.citius.lab.joctomap.util.JOctomapLogger;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
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
        AdjacencyMap map = AdjacencyMap.create(octree);
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
                        .hasArg()
                        .longOpt("input")
                        .build()
        );
        op.addOption(
                Option.builder("o")
                        .required()
                        .desc("Output file")
                        .hasArg()
                        .longOpt("output")
                        .build()
        );
        return op;
    }
    
}
