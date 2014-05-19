package es.usc.citius.lab.joctomap.iterators;

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.util.Point3D;

public class LeafBBXIteratorTest {

	private static JOctree emptyOctree;
	
	/**
	 * Initialize elements for the test.
	 */
	@BeforeClass
	public static void initialize(){
		emptyOctree = JOctree.create(0.2);
	}
	
	/**
	 * Tests if the empty octree iterator has no next elements.
	 */
	@Test
	public void createIteratorEmptyOctree() {
		emptyOctree.updateNode(0, 0, 0, true);
		LeafBBXIterator emptyIterator = emptyOctree.leafBBXIterator(new Point3D(-10, -10, -10), new Point3D(10, 10, 10), 0);
		int counter = 0;
		while(emptyIterator.hasNext()){
			emptyIterator.next();
			counter++;
		}
		assertTrue(emptyIterator.getPointer() != 0);
		assertTrue("In an empty octree, iterator must only retrieve the root node", counter == 1);
	}

}
