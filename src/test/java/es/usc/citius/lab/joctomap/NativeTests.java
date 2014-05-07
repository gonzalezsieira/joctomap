package es.usc.citius.lab.joctomap;

import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

import es.usc.citius.lab.joctomap.distance.JOctreeDistanceMapTest;

/**
 * Order the execution of all native tests in the library.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@RunWith(Suite.class)
@SuiteClasses({ 
	JOctreeTest.class, 
	JOctreeNodeTest.class,
	JOctreeDistanceMapTest.class
})
public class NativeTests {
	
	/**
	 * Load octomap native library before all tests
	 */
	@BeforeClass
	public static void setUp(){
		System.loadLibrary("joctomap");
	}
}
