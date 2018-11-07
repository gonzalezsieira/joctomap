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

import java.io.File;
import static org.junit.Assert.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URISyntaxException;

import org.junit.BeforeClass;
import org.junit.Test;

/**
 * Tests the reader of PPM files with an example file, testing if the reader information matches
 * with the header of the file.
 * 
 * @author Adrián González Sieira {@literal <adrian.gonzalez@usc.es>}
 */
public class PPMFileReaderTest {

	private PPMFileReader reader;
	
        /**
	 * Constructor which loads a resource
	 * 
	 * @throws FileNotFoundException if file not found
	 * @throws IOException if I/O error occurs
         * @throws URISyntaxException if URI bad formatted
	 */
        public PPMFileReaderTest() throws IOException, URISyntaxException{
            this.reader = new PPMFileReader(new File(PPMFileReaderTest.class.getClassLoader().getResource("ecmr.ppm").toURI()).getAbsolutePath());
        }
	
	/**
	 * Tests the dimension of the read map file.
	 */
	@Test
	public void dimensionsTest() {
		assertTrue("X size does not match with the header of the file", reader.getSizeX() == 504);
		assertTrue("Y size does not match with the header of the file", reader.getSizeY() == 473);
	}
	
	/**
	 * Tests the dimension of the matrix of pixels.
	 */
	@Test
	public void pixelsTest(){
		assertTrue("Pixel X dimension is " + reader.getPixels().length + " but header specifies 504", reader.getPixels().length == 504);
		assertTrue("Pixel Y dimension is " + reader.getPixels()[0].length + " but header specifies 473", reader.getPixels()[0].length == 473);
		assertTrue("Pixel Z dimension is " + reader.getPixels()[0][0].length + " but header specifies 3", reader.getPixels()[0][0].length == 3);
		for(int x = 0; x < reader.getPixels().length; x++){
			for(int y = 0; y < reader.getPixels()[x].length; y++){
				for(int z = 0; z < reader.getPixels()[x][y].length; z++){
					assertTrue("Pixel must be between 0 and the max value", reader.getPixels()[x][y][z] >= 0);
				}	
			}	
		}
	}

	/**
	 * Tests the max value of the map file.
	 */
	@Test
	public void getFormatTest(){
		assertTrue("Format does not match with the header of the file", reader.getFormat().equals("P3"));
	}
}
