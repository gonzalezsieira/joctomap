package es.usc.citius.lab.joctomap.iterators;

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

import es.usc.citius.lab.joctomap.JOctreeTest;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.octree.JOctreeNode;
import es.usc.citius.lab.motionplanner.core.Point3D;

public class LeafBBXIteratorTest {

	private static JOctree octree;
	
	/**
	 * Initialize elements for the test.
	 */
	@BeforeClass
	public static void initialize(){
		octree = JOctreeTest.getOctree();
	}
	
	/**
	 * Tests if the octree iterator is executed properly, and the information retrieval works as expected.
	 */
	@Test
	public void createIteratorEmptyOctree() {
		octree.updateNode(0, 0, 0, true);
		OctreeIterator iterator = octree.leafBBXIterator(new Point3D(-10, -10, -10), new Point3D(10, 10, 10), 0);
		assertTrue(iterator.getPointer() != 0);
		int counter = 0;
		while(iterator.hasNext()){
			//test information retrieval methods
			Point3D coordinate = iterator.coordinate();
			int depth = iterator.depth();
			JOctreeKey key = iterator.key();
			double size = iterator.size();
			assertEquals("keyToCoord() has different values", octree.keyToCoord(key, depth), coordinate);
			assertEquals("sizeAtDepth has different values", octree.getNodeSize(depth), size, octree.getResolution() / 100000);
			JOctreeNode node = iterator.node();
			//test methods of node
			assertTrue("numChildren must be > 0", node.getNumChildren() >= 0);
			assertEquals("numChildren and children.size() has not the same value", node.getNumChildren(), node.getChildren().size());
			assertTrue("occupancy must be > 0", node.getOccupancy() >= 0);
			//advance iterator (last thing to do)
			iterator.next();
			counter++;
		}
		assertTrue("It is needed to have >= 1 iterations by definition", counter >= 1);
	}

}
