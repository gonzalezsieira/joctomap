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
package es.usc.citius.lab.joctomap.util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

/**
 * Class which parses a PPM file (ASCII format).
 *
 *  @author Adrián González Sieira {@literal <adrian.gonzalez@usc.es>}
 */
public class PPMFileReader {

	private String format;
	private int[][][] pixels;
	private int sizeX;
	private int sizeY;
	
	/**
	 * Constructor of the class, that reads the information in the PPM file and stores it in variables
	 * to be read later.
	 * 
	 * @param path to the PPM file
	 * @throws FileNotFoundException when the specified path does not exist
	 * @throws IOException when an I/O error occurs
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

	/**
	 * @return size of the map in pixels, coordinate X
	 */
	public int getSizeX() {
		return sizeX;
	}

	/**
	 * @return size of the map in pixels, coordinate Y
	 */
	public int getSizeY() {
		return sizeY;
	}

	/**
	 * @return retrieve file format
	 */
	public String getFormat() {
		return format;
	}

	/**
	 * @return parsed content of the file
	 */
	public int[][][] getPixels() {
		return pixels;
	}
	
}
