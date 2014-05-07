package es.usc.citius.lab.joctomap.distance;

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

import es.usc.citius.lab.joctomap.JOctreeTest;
import es.usc.citius.lab.joctomap.octree.JOctree;

/**
 * Executes tests over the {@link JOctreeDistanceMap} class. Tests
 * are executed in ascending order given their name.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeDistanceMapTest {

	private static JOctree octree;
	private static JOctreeDistanceMap distanceMap;
	
	/**
	 * Retrieves the empty octree initialized in {@link JOctreeTest}.
	 */
	@BeforeClass
	public static void initialize(){
		octree = JOctreeTest.getOctree();
	}
	
	/**
	 * Executes the creation of the distance map.
	 */
	@Test
	public void test01_Create() {
		distanceMap = JOctreeDistanceMap.create(octree, 10);
	}
	
	/**
	 * Executes the update of the distance map.
	 */
	@Test
	public void test01_Update() {
		distanceMap.update();
	}


}
