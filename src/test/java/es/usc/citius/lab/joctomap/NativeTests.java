package es.usc.citius.lab.joctomap;

import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

import es.usc.citius.lab.joctomap.iterators.LeafBBXIteratorTest;

/**
 * Order the execution of all native tests in the library.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@RunWith(Suite.class)
@SuiteClasses({ 
	JOctreeTest.class, 
	JOctreeNodeTest.class,
	LeafBBXIteratorTest.class
	//disabled due to high memory consumption
	//JOctreeDistanceMapTest.class 
})
public class NativeTests {
	
}
