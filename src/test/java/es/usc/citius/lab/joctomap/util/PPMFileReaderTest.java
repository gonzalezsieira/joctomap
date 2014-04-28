package es.usc.citius.lab.joctomap.util;

import static org.junit.Assert.*;

import java.io.FileNotFoundException;
import java.io.IOException;

import org.junit.BeforeClass;
import org.junit.Test;

import es.usc.citius.lab.joctomap.util.PPMFileReader;

/**
 * Tests the reader of PPM files with an example file, testing if the reader information matches
 * with the header of the file.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class PPMFileReaderTest {

	private static PPMFileReader reader;
	
	/**
	 * Reads a ppm file in src/test/resources
	 * 
	 * @throws FileNotFoundException if file not found
	 * @throws IOException if I/O error occurs
	 */
	@BeforeClass
	public static void initialize() throws FileNotFoundException, IOException{
		reader = new PPMFileReader("src/test/resources/ecmr.ppm");
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
	 * Tests the max value of the map file.
	 */
	@Test
	public void maxValueTest(){
		assertTrue("Max value does not match with the header of the file", reader.getMaxValue() == 255);
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
					assertTrue("Pixel must be between 0 and the max value", reader.getPixels()[x][y][z] >= 0 && reader.getPixels()[x][y][z] <= reader.getMaxValue());
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
