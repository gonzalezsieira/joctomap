package es.usc.citius.lab.joctomap;

/**
 * Test case for native objects, that ensures the dynamic library
 * to be loaded before the execution of the test cases that
 * extend this class.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class NativeTest {

	/**
	 * Load dynamic library to access the native methods.
	 */
	static {
		System.loadLibrary("joctomap");
	}

}
