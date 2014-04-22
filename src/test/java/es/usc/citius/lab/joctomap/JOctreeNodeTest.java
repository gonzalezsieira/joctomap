package es.usc.citius.lab.joctomap;

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

public class JOctreeNodeTest {

	private static JOctreeNode octreeNode; //used in tests
	
	
	/**
	 * Initializes 
	 */
	@BeforeClass
	public static void initialize(){
		//store node of the readed octree
		octreeNode = JOctreeTest.getOctree().search(0d, 0d, 0d, 0);
	}
	
	/**
	 * Test if the occupancy of the node is between 0 and 1
	 */
	@Test
	public void getOccupancyTest() {
		double occ = octreeNode.getOccupancy();
		assertTrue("Occupancy be between 0 and 1 > 1", occ >=0 && occ <= 1);
	}

}
