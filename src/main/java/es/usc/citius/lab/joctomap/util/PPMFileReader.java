package es.usc.citius.lab.joctomap.util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class PPMFileReader {

	private String format;
	private int[][][] pixels;
	private int sizeX;
	private int sizeY;
	private int maxValue;
	
	/**
	 * Constructor of the class, that reads the information in the PPM file and stores it in variables
	 * to be read later.
	 * 
	 * @param path to the PPM file
	 * @throws {@link FileNotFoundException} when the specified path does not exist
	 * @throws {@link IOException} when an I/O error occurs
	 */
	public PPMFileReader(String path) throws FileNotFoundException, IOException{
		//instantiate new reader to fill the information of the file
		BufferedReader reader = new BufferedReader(new FileReader(path));
		//skip the first line of the file
        format = reader.readLine();
        //skip all lines that begin with "#"
        String line = reader.readLine();
        while (line.startsWith("#")) {
            line = reader.readLine();
        }
        //split line to get dimensions of the map
        String[] dimensions = line.split(" ");
        //store dimensions of the map
        sizeX = Integer.parseInt(dimensions[0]);
        sizeY = Integer.parseInt(dimensions[1]);
        //max value of the file
        maxValue = Integer.parseInt(reader.readLine());
        //initialize matrix
        pixels = new int[sizeX][sizeY][3];
        //iterate to recover the values of the pixels
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
            	//add points to the matrix
            	pixels[x][y][0] = Integer.parseInt(reader.readLine());
            	pixels[x][y][1] = Integer.parseInt(reader.readLine());
            	pixels[x][y][2] = Integer.parseInt(reader.readLine());
            }
        }
		//close input stream
		reader.close();
	}
	
	
	public int getSizeX() {
		return sizeX;
	}
	
	public int getSizeY() {
		return sizeY;
	}
	
	public int getMaxValue() {
		return maxValue;
	}
	
	public String getFormat() {
		return format;
	}
	
	public int[][][] getPixels() {
		return pixels;
	}
	
}
