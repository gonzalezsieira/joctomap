package es.usc.citius.lab.joctomap.util;

import org.junit.Test;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;

import static org.junit.Assert.*;

public class MapFileFormatReaderTest {

    private MapFileReader readerPPM1;
    private MapFileReader readerPPM2;
    private MapFileReader readerPGM1;
    private MapFileReader readerPGM2;

    @Test
    public void test01_readPGM() throws IOException, URISyntaxException{
        this.readerPPM1 = new MapFileReader(new File(MapFileReaderTest.class.getClassLoader().getResource("ecmr_ascii.ppm").toURI()).getAbsolutePath());
        this.readerPPM2 = new MapFileReader(new File(MapFileReaderTest.class.getClassLoader().getResource("ecmr_binary.ppm").toURI()).getAbsolutePath());
        assertEquals(this.readerPPM1.getSizeX(), this.readerPPM2.getSizeX());
        assertEquals(this.readerPPM1.getSizeY(), this.readerPPM2.getSizeY());
        assertEquals(this.readerPPM1.getMaxValue(), this.readerPPM2.getMaxValue());
        assertEquals(this.readerPPM1.getPixels().length, this.readerPPM1.getPixels().length);
        for(int i = 0; i < this.readerPPM1.getPixels().length; i++){
            assertEquals(this.readerPPM1.getPixels().length, this.readerPPM2.getPixels().length);
            for(int j = 0; j < this.readerPPM1.getPixels()[i].length; j++){
                assertEquals(this.readerPPM1.getPixels()[i][j][0], this.readerPPM2.getPixels()[i][j][0]);
                assertEquals(this.readerPPM1.getPixels()[i][j][1], this.readerPPM2.getPixels()[i][j][1]);
                assertEquals(this.readerPPM1.getPixels()[i][j][2], this.readerPPM2.getPixels()[i][j][2]);
            }
        }
    }

    @Test
    public void test03_readPPM() throws IOException, URISyntaxException{
        this.readerPGM1 = new MapFileReader(new File(MapFileReaderTest.class.getClassLoader().getResource("ecmr_ascii.pgm").toURI()).getAbsolutePath());
        this.readerPGM2 = new MapFileReader(new File(MapFileReaderTest.class.getClassLoader().getResource("ecmr_binary.pgm").toURI()).getAbsolutePath());
        assertEquals(this.readerPGM1.getSizeX(), this.readerPGM2.getSizeX());
        assertEquals(this.readerPGM1.getSizeY(), this.readerPGM2.getSizeY());
        assertEquals(this.readerPGM1.getMaxValue(), this.readerPGM2.getMaxValue());
        assertEquals(this.readerPGM1.getPixels().length, this.readerPGM1.getPixels().length);
        for(int i = 0; i < this.readerPGM1.getPixels().length; i++){
            assertEquals(this.readerPGM1.getPixels().length, this.readerPGM2.getPixels().length);
            for(int j = 0; j < this.readerPGM1.getPixels()[i].length; j++){
                assertEquals(this.readerPGM1.getPixels()[i][j][0], this.readerPGM2.getPixels()[i][j][0]);
                assertEquals(this.readerPGM1.getPixels()[i][j][1], this.readerPGM2.getPixels()[i][j][1]);
                assertEquals(this.readerPGM1.getPixels()[i][j][2], this.readerPGM2.getPixels()[i][j][2]);
            }
        }
    }

    @Test
    public void test05_compareFormats() throws IOException, URISyntaxException{
        this.readerPGM1 = new MapFileReader(new File(MapFileReaderTest.class.getClassLoader().getResource("ecmr_ascii.pgm").toURI()).getAbsolutePath());
        this.readerPPM1 = new MapFileReader(new File(MapFileReaderTest.class.getClassLoader().getResource("ecmr_ascii.ppm").toURI()).getAbsolutePath());
        assertEquals(this.readerPGM1.getSizeX(), this.readerPPM1.getSizeX());
        assertEquals(this.readerPGM1.getSizeY(), this.readerPPM1.getSizeY());
        assertEquals(this.readerPGM1.getMaxValue(), this.readerPPM1.getMaxValue());
        assertEquals(this.readerPGM1.getPixels().length, this.readerPGM1.getPixels().length);
        for(int i = 0; i < this.readerPGM1.getPixels().length; i++){
            assertEquals(this.readerPGM1.getPixels().length, this.readerPPM1.getPixels().length);
            for(int j = 0; j < this.readerPGM1.getPixels()[i].length; j++){
                assertEquals(this.readerPGM1.getPixels()[i][j][0], this.readerPPM1.getPixels()[i][j][0]);
                assertEquals(this.readerPGM1.getPixels()[i][j][1], this.readerPPM1.getPixels()[i][j][1]);
                assertEquals(this.readerPGM1.getPixels()[i][j][2], this.readerPPM1.getPixels()[i][j][2]);
            }
        }
    }

}
