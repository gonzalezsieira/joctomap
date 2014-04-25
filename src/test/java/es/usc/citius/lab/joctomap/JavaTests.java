package es.usc.citius.lab.joctomap;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

/**
 * Order the execution of all java tests in the library.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@RunWith(Suite.class)
@SuiteClasses({ 
	PPMFileReaderTest.class 
})
public class JavaTests {
	//empty suite
}
