package es.usc.citius.lab.joctomap.util;

import java.io.FileNotFoundException;
import java.io.IOException;

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
public abstract class JOCtreeBuilder {

	/**
	 * Instantiates a new {@link JOctree} and puts the information contained in 
	 * @param input route of the PPM file (this will add the extension if necessary)
	 * @param output route of the output .ot file (this will add the extension if necessary)
	 * @param sizeX max. size of the X dimension of the map
	 * @param sizeY max. size of the Y dimension of the map
	 * @return
	 */
	public static JOctree octreeFromPPM(String input, String output, double sizeX, double sizeY){
		//instantiate new octree
		JOctree octree = JOctree.create(0.125);
		PPMFileReader reader = null;
		//open file and read data from
		try{
			reader = new PPMFileReader(input);
		} catch(FileNotFoundException ex){
			JOctomapLogger.severe("Could not open the file specified: " + ex);
		} catch (IOException ex) {
			JOctomapLogger.severe("An I/O error occured processing the file: " + ex);
		}
		double resX = sizeX/(double) (reader.getPixels().length - 1);
		double resY = sizeY/(double) (reader.getPixels()[0].length - 1);
		//iterate over the read pixels to update the information of the nodes
		for(int x = 0; x < reader.getPixels().length; x++) {
			for(int y = 0; y < reader.getPixels()[x].length; y++){
				int[] rgb = reader.getPixels()[x][y];
				//occupied case: one of the color components reaches the maximum value of the file
				boolean occupied = rgb[0] == reader.getMaxValue() || rgb[1] == reader.getMaxValue() || rgb[2] == reader.getMaxValue();
				//update occupancy information
				octree.updateNode(resX * x, resY * y, 0d, occupied);
			}
		}
		String outputPath = output;
		//modify output 
		if(!outputPath.endsWith(".ot")) { outputPath = outputPath.concat(".ot"); }
		//write octree
		octree.write(outputPath);
		return octree;
	}
	
}
