/**
 * Copyright (C) 2014-2017 Adrián González Sieira (adrian.gonzalez@usc.es)
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

import java.io.FileNotFoundException;
import java.io.IOException;

import org.apache.commons.cli.CommandLine;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeNode;
import es.usc.citius.lab.joctomap.util.JOctomapLogger;
import es.usc.citius.lab.joctomap.util.PPMFileReader;
import java.text.DecimalFormat;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;

/**
 * Builder for {@link JOctree}. Main functionality of this class is to instantiate 
 * the octrees from previous information. The following are included:
 * <ul>
 * 		<li>Obtain a new {@link JOctree} from a PPM image</li>
 * </ul>
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class JOCtreeBuilder extends Module{

    @Override
    public void execute(CommandLine args) {
        //retrieve input args
        String[] inputArgs = args.getOptionValues("i");
        //process the ppm to generate an octree
        JOctree octree = octreeFromPPM(
                inputArgs[0], 
                Float.parseFloat(inputArgs[1]),
                Float.parseFloat(inputArgs[2]),
                Integer.parseInt(inputArgs[3])
        );
        //write octree to file (.ot extension mandatory)
        String outputPath = args.getOptionValue("o");
        if(!outputPath.endsWith(".ot")) { outputPath = outputPath.concat(".ot"); }
        JOctomapLogger.fine("Saving to " + outputPath);
        octree.write(outputPath);
    }

    /**
     * Instantiates a new {@link JOctree} and puts the information contained in a {@link PPMFileReader}.
     * @param input where the PPM file is located
     * @param resolution min size of the cells
     * @param sizeX max. size of the X dimension of the map
     * @param maxDepthCell number of levels which a cell is allowed to compact
     * @return
     */
    public static JOctree octreeFromPPM(String input, float resolution, float sizeX, int maxDepthCell){
        //read the ppm file 
        PPMFileReader reader = null;
        //open file and read data from
        try {
            JOctomapLogger.fine("Reading file " + input + "...");
            reader = new PPMFileReader(input);
        } catch (FileNotFoundException ex) {
            JOctomapLogger.severe("Could not open the file specified: " + ex);
        } catch (IOException ex) {
            JOctomapLogger.severe("An I/O error occured processing the file: " + ex);
        }
        //instantiate new octree
        JOctree octree = JOctree.create(resolution);
        float resolutionPPM = sizeX / (reader.getPixels().length - 1);
        float sizeY = (reader.getPixels()[0].length - 1) * resolutionPPM;
        float sizeZ = octree.getNodeSize(maxDepthCell);
        JOctomapLogger.fine("Generating octomap structure with following params:"
                + "\n\t* Resolution: " + resolution
                + "\n\t* Dimensions: [" + sizeX + ", " + sizeY + ", " + sizeZ + "]");
        //to show percentage of operation done
        DecimalFormat formatter = new DecimalFormat("0.00%");
        int iterations = (int) (sizeZ / octree.getResolution()) * 2;
        //iterate over the read pixels to update the information of the nodes
        int currentIteration = 0;
        for(float z = 0f; z < sizeZ; z += octree.getResolution() / 2f){
            for (float x = 0; x < sizeX; x += octree.getResolution() / 2f) {
                for (float y = 0; y < sizeY; y += octree.getResolution() / 2f) {
                    int[] rgb = reader.getPixels()[Math.round(x / resolutionPPM)][Math.round(y / resolutionPPM)];
                    //occupied case: one of the color components reaches the maximum value of the file
                    boolean occupied = rgb[0] < 10 && rgb[1] < 10 && rgb[2] < 10;
                    //update occupancy information until we get an absolute value for the occupancy (1 or 0)
                    Double previousOccupancy = null;
                    do {
                        JOctreeNode node = octree.updateNode(x, sizeY - y, z, occupied);
                        if(previousOccupancy != null && Double.compare(node.getOccupancy(), previousOccupancy) == 0){
                            break;
                        }
                        previousOccupancy = node.getOccupancy();
                    } while (true);
                }
            }
            currentIteration++;
            JOctomapLogger.fine("\r" + formatter.format((float) currentIteration / (float) iterations));
        }
        JOctomapLogger.fine("Pruning octomap...");
        //prunes octree
        octree.updateInnerOccupancy();
        octree.prune();
        return octree;
    }

    @Override
    public Options getModuleOptions() {
        Options op = getGenericOptions();
        op.addOption(
            Option.builder("i")
                .required()
                .desc("Input .ppm file, resolution, size X (m), and max depth of cells. Size Y will be calculated automatically to maintain proportions.")
                .numberOfArgs(4)
                .longOpt("input")
                .argName("ppmFile> <resolution> <sizeX> <maxDepthCell")
                .build()
        );
        op.addOption(
            Option.builder("o")
                .required()
                .desc("Output .ot file")
                .hasArg()
                .longOpt("output")
                .argName("ot")
                .build()
        );
        return op;
    }
	
}
