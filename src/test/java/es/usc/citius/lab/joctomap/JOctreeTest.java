package es.usc.citius.lab.joctomap;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.octree.JOctreeNode;
import static org.junit.Assert.*;

/**
 * Unit test for octree objects. Ensures the dynamic library
 * to be loaded before the execution of the test cases.
 * 
 * The methods in this class are executed in an ascending ordered way to ensure
 * that the static parameters of the class (that store values used in 
 * later tests) are filled properly.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeTest{

	private static JOctree emptyOctree; //used in tests
	private static JOctree octree; //used in tests
	private static JOctreeKey key1; //used in tests
	private static JOctreeKey key2; //used in tests
	private static Double res; //used in tests
	private static Integer depth; //used in tests
	private static File fileRead;
	private static File fileWrite;
	
	
	/**
	 * Initialize files to use with the tests in this class
	 * 
	 * @throws IOException when an I/O error occurs
	 */
	@BeforeClass
	public static void initialize() throws IOException{
		fileRead = new File("src/test/resources/".concat("fr_campus").concat(
				".ot")).getCanonicalFile();
		fileWrite = new File("fr_campus".concat("_test").concat(".ot"))
		.getCanonicalFile();
	}
	
	@AfterClass
	public static void down() throws IOException{
		// delete the generated file in this test case
		fileWrite.delete();
	}
	
	/**
	 * Accessing {@link JOctree} element of this class after tests are executed.
	 * 
	 * @return readed {@link JOctree}
	 */
	public static JOctree getOctree(){
		return octree;
	}
	
	/**
	 * Instantiate new empty octree with 0.5 resolution
	 */
	@Test
	public void test01_createOctree(){
		emptyOctree = JOctree.create(0.5);
	}
	
	/**
	 * Unit test for reading a .ot file.
	 * @throws IOException when an I/O error occurs 
	 */
	@Test
	public void test02_readFileOt() throws IOException {
		octree = JOctree.read(fileRead.getAbsolutePath());
		assertTrue("Octree direction of memory not assigned",
				octree.getPointer() != 0);
	}

	/**
	 * Unit test for writing a .ot file.
	 * 
	 * @throws IOException
	 */
	@Test
	public void test03_writeFileOtTest() throws IOException {
		// write file to compare sizes
		octree.write(fileWrite.getAbsolutePath());
		// compare by content both files
		assertTrue("Input and output files with the same content",
				FileUtils.contentEquals(fileRead, fileWrite));
	}
	
	/**
	 * Unit test for retrieving a {@link JOctreeKey} from a position.
	 */
	@Test
	public void test04_obtainCellKeyAtPositionTest() {
		key1 = octree.cellKeyAt(0f, 0f, 0f);
		assertTrue("JOctreeKey must not have negative values", key1.getX() >= 0 && key1.getY() >= 0 && key1.getZ() >= 0);
	}

	/**
	 * Unit test for retrieving a {@link JOctreeKey} from a position and depth
	 * 
	 * @param octree {@link JOctree} instance
	 * @return key that identifies the node
	 */
	@Test
	public void test05_obtainCellKeyAtPositionWithDepthTest() {
		key2 = octree.cellKeyAt(0f, 0f, 0f, 1);
		assertTrue("JOctreeKey must not have negative values", key2.getX() >= 0 && key2.getY() >= 0 && key2.getZ() >= 0);
	}
	
	/**
	 * Unit test for testing the adjustement of {@link JOctreeKey} between different
	 * depth levels.
	 */
	@Test
	public void test06_adjustDepthOfKey(){
		JOctreeKey adjustement = octree.adjustKeyAt(key2, 1);
		assertTrue("JOctreeKey must not have negative values", adjustement.getX() >= 0 && adjustement.getY() >= 0 && adjustement.getZ() >= 0);
		assertTrue("Adjusted key are not equal to the retrieved one at the same depth", key2.equals(adjustement));
		assertTrue("Adjusted key has a different hash to the retrieved one at the same depth", key2.hashCode() == adjustement.hashCode());
	}
	
	/**
	 * Test case to recover the resolution of the octree.
	 */
	@Test
	public void test07_getResolutionTest(){
		res = octree.getResolution();
		double res2 = emptyOctree.getResolution();
		assertTrue("Resolution must be > 0", res > 0);
		assertTrue("Resolution different to the created with the empty octree", Double.compare(0.5, res2) == 0); 
	}
	
	/**
	 * Test case to recover the depth of the octree.
	 * 
	 * @param octree {@link JOctree} instance
	 * @return octree maximum depth
	 */
	@Test
	public void test08_getTreeDepthTest(){
		depth = octree.getTreeDepth();
		assertTrue("Tree depth must be > 0", depth > 0);
	}
	
	/**
	 * Test case to recover the node size of the octree. 
	 * Checks if the size at the maximum depth is equal to the
	 * minimum resolution of the octree.
	 */
	@Test
	public void test09_getNodeSizeTest(){
		double size = octree.getNodeSize(depth);
		assertTrue("Node size must be > 0", size > 0);
		assertTrue("Minimum node size must be equal to the octree resolution", Double.compare(res, size) == 0);
	}
	
	/**
	 * Test case to search a node in the octree, given its {@link JOctreeKey} (search
	 * with depth = 0)
	 * 
	 * @param octree {@link JOctree} instance
	 * @param key {@link JOctreeKey} instance
	 */
	@Test
	public void test10_searchWithKeyTest(){
		JOctreeNode node = octree.search(key1, 0);
		assertTrue("Native pointer of node is not assigned", node.getPointer() != 0);
	}
	
	/**
	 * Test case to search a node in the octree, given a 3D position (search
	 * with depth = 0)
	 */
	@Test
	public void test11_searchWith3DPositionTest(){
		JOctreeNode node = octree.search(0f, 0f, 0f, 0);
		assertTrue("Native pointer of node is not assigned", node.getPointer() != 0);
	}
	
	/**
	 * Tests if the node retrieved is the same in the update step and
	 * the search step.
	 */
	@Test
	public void test12_updateNodeTest(){
		JOctreeNode node1 = emptyOctree.updateNode(0d, 0d, 0d, true);
		JOctreeNode node2 = emptyOctree.search(0d, 0d, 0d, 0);
		assertEquals("Updated and retrieved node are not equals ", node1.getPointer(), node2.getPointer());
		assertTrue("Updated node must be occupancy probability > 0", node1.getOccupancy() > 0);
	}
	
	/**
	 * Inserts an "occupied node" update several times, checking
	 * that the probability of occupation must increase with each update.
	 */
	@Test
	public void test13_recursiveUpdateOccupiedNodeTest(){
		JOctreeNode node = emptyOctree.search(0d, 0d, 0d, 0);
		//continue updating the node
		for(int i = 0; i < 10; i++){
			double occ = node.getOccupancy();
			node = emptyOctree.updateNode(0d, 0d, 0d, true);
			double occNew = node.getOccupancy();
			assertTrue("Occupancy must increase with each update", occNew >= occ);
		}
	}
	
	/**
	 * Queries if the node is occupied.
	 */
	@Test
	public void test14_isNodeOccupiedTest(){
		JOctreeNode node = emptyOctree.search(0d, 0d, 0d, 0);
		assertTrue("Occupied node does not retrieve occupied", emptyOctree.isNodeOccupied(node));
	}
	
	/**
	 * Inserts a "free node" update several times, checking that 
	 * the probability of occupation must decrease with each update.
	 */
	@Test
	public void test15_recursiveUpdateFreeNodeTest(){
		JOctreeNode node = emptyOctree.search(0d, 0d, 0d, 0);
		//continue updating the node
		for(int i = 0; i < 10; i++){
			double occ = node.getOccupancy();
			node = emptyOctree.updateNode(0d, 0d, 0d, false);
			double occNew = node.getOccupancy();
			assertTrue("Occupancy must decrease with each update", occNew <= occ);
		}
	}
	
	/**
	 * Queries if the node is not occupied.
	 */
	@Test
	public void test16_isNodeOccupiedTest(){
		JOctreeNode node = emptyOctree.search(0d, 0d, 0d, 0);
		assertTrue("Occupied node does not retrieve occupied", !emptyOctree.isNodeOccupied(node));
	}
}
