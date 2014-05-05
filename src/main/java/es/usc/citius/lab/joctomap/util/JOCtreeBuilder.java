package es.usc.citius.lab.joctomap.util;

import java.io.FileNotFoundException;
import java.io.IOException;

import org.apache.commons.cli.CommandLine;

import es.usc.citius.lab.joctomap.octree.JOctree;

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

	public String getName() {
		return "ppmot";
	}

	public void execute(CommandLine args) {
		//retrieve input args
		String[] inputArgs = args.getOptionValues("i");
		//process the ppm to generate an octree
		JOctree octree = octreeFromPPM(inputArgs[0], Double.parseDouble(inputArgs[1]), Double.parseDouble(inputArgs[2]));
		//write octree to file (.ot extension mandatory)
		String outputPath = args.getOptionValue("o");
		if(!outputPath.endsWith(".ot")) { outputPath = outputPath.concat(".ot"); }
		octree.write(outputPath);
	}
	
	/**
	 * Instantiates a new {@link JOctree} and puts the information contained in a {@link PPMFileReader}.
	 * @param input where the PPM file is located
	 * @param sizeX max. size of the X dimension of the map
	 * @param sizeY max. size of the Y dimension of the map
	 * @return
	 */
	public static JOctree octreeFromPPM(String input, double sizeX, double sizeY){
		//read the ppm file 
		PPMFileReader reader = null;
		//open file and read data from
		try{
			JOctomapLogger.info("Reading file " + input + "...");
			reader = new PPMFileReader(input);
		} catch(FileNotFoundException ex){
			JOctomapLogger.severe("Could not open the file specified: " + ex);
		} catch (IOException ex) {
			JOctomapLogger.severe("An I/O error occured processing the file: " + ex);
		}
		JOctomapLogger.info("Reading PPM file " + input);
		//instantiate new octree
		JOctree octree = JOctree.create(0.125);
		double resX = sizeX/(double) (reader.getPixels().length - 1);
		double resY = sizeY/(double) (reader.getPixels()[0].length - 1);
		//iterate over the read pixels to update the information of the nodes
		for(int x = 0; x < reader.getPixels().length; x++) {
			for(int y = 0; y < reader.getPixels()[x].length; y++){
				int[] rgb = reader.getPixels()[x][y];
				//occupied case: one of the color components reaches the maximum value of the file
				boolean occupied = rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 0;
				//update occupancy information
				octree.updateNode(resX * x, resY * y, 0d, occupied);
			}
		}
		return octree;
	}
	
}
