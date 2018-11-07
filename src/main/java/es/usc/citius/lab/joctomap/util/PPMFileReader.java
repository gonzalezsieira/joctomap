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

import java.io.*;
import java.util.Scanner;

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
		FileInputStream inputStream = new FileInputStream(path);
		Scanner scanner = new Scanner(inputStream);
		int linesHeader = 0;
		//skip the first line of the file
        format = scanner.nextLine(); linesHeader++;
        //skip all lines that begin with "#"
        String line = scanner.nextLine(); linesHeader++;
        while (line.startsWith("#")) {
            line = scanner.nextLine(); linesHeader++;
        }
        //split line to get dimensions of the map
        String[] dimensions = line.split(" ");
        //store dimensions of the map
        sizeX = Integer.parseInt(dimensions[0]);
        sizeY = Integer.parseInt(dimensions[1]);
        //max value of the file
        maxValue = Integer.parseInt(scanner.nextLine()); linesHeader++;
        //close scanner & input streams
		inputStream.close();
		//re-open input stream and skip header
		inputStream = new FileInputStream(path);
		DataInputStream dataInputStream = new DataInputStream(inputStream);
		// discard number of lines in the header
		while (linesHeader > 0) {
			char c;
			do {
				c = (char)(dataInputStream.readUnsignedByte());
			} while (c != '\n');
			linesHeader--;
		}
        //initialize pixel matrix
        pixels = new int[sizeX][sizeY][3];
        if(format.equals("P2")){
        	readPGMASCII(dataInputStream);
		}
		else if(format.equals("P3")) {
			readPPMASCII(dataInputStream);
		}
		else if(format.equals("P5")){
			readPGMBinary(dataInputStream);
		}
		else if(format.equals("P6")){
			readPPMBinary(dataInputStream);
		}
		else{
			throw new UnsupportedOperationException("PPM/PGM format not supported - Expected headers: P2/P3/P5/P6");
		}
	}

	/**
	 * Routine to read ASCII PGM file (P2)
	 * @param dataInputStream
	 * @throws IOException
	 */
	public void readPGMASCII(DataInputStream dataInputStream) throws IOException{
		JOctomapLogger.fine("Reading ASCII PGM file...");
		Scanner scanner = new Scanner(dataInputStream);
		//iterate to recover the values of the pixels
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				//read the single-value grayscale
				int value = Integer.parseInt(scanner.nextLine());
				if (value > maxValue)
					throw new IllegalStateException("PGM file received an unexpected value: " + value + "; max. expected was " + maxValue);
				pixels[x][y][0] = value;
				pixels[x][y][1] = value;
				pixels[x][y][2] = value;
			}
		}
	}

	/**
	 * Routine to read data of ASCII PPM file (P3)
	 *
	 * @param dataInputStream
	 * @throws IOException
	 */
	public void readPPMASCII(DataInputStream dataInputStream) throws IOException{
		JOctomapLogger.fine("Reading ASCII PPM file...");
		Scanner scanner = new Scanner(dataInputStream);
		//iterate to recover the values of the pixels
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				//add points to the matrix
				pixels[x][y][0] = Integer.parseInt(scanner.nextLine());
				pixels[x][y][1] = Integer.parseInt(scanner.nextLine());
				pixels[x][y][2] = Integer.parseInt(scanner.nextLine());
			}
		}
	}

	/**
	 * Routine to read Binary PGM file (P5)
	 *
	 * @param dataInputStream
	 * @throws IOException
	 */
	public void readPGMBinary(DataInputStream dataInputStream) throws IOException{
		JOctomapLogger.fine("Reading Binary PGM file...");
		//iterate to recover the values of the pixels
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
					//read the single-value grayscale
					int value = dataInputStream.readUnsignedByte();
					if (value > maxValue)
						throw new IllegalStateException("PGM file received an unexpected value: " + value + "; max. expected was " + maxValue);
					pixels[x][y][0] = value;
					pixels[x][y][1] = value;
					pixels[x][y][2] = value;
			}
		}
	}

	/**
	 * Routine to read Binary PPM file (P6)
	 * @param dataInputStream
	 * @throws IOException
	 */
	public void readPPMBinary(DataInputStream dataInputStream) throws IOException{
		JOctomapLogger.fine("Reading Binary PPM file...");
		//iterate to recover the values of the pixels
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				//read the single-value grayscale
				int value = dataInputStream.readUnsignedByte();
				if (value > maxValue)
					throw new IllegalStateException("PGM file received an unexpected value: " + value + "; max. expected was " + maxValue);
				pixels[x][y][0] = value;

				value = dataInputStream.readUnsignedByte();
				if (value > maxValue)
					throw new IllegalStateException("PGM file received an unexpected value: " + value + "; max. expected was " + maxValue);
				pixels[x][y][1] = value;

				value = dataInputStream.readUnsignedByte();
				if (value > maxValue)
					throw new IllegalStateException("PGM file received an unexpected value: " + value + "; max. expected was " + maxValue);
				pixels[x][y][2] = value;
			}
		}
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
