package es.usc.citius.lab.joctomap;

import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

@RunWith(Suite.class)
@SuiteClasses({ 
	JOctreeTest.class, 
	JOctreeNodeTest.class 
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
