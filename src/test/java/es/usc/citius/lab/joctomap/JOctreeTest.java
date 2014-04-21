package es.usc.citius.lab.joctomap;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

import static org.junit.Assert.*;

/**
 * Unit test for octree objects. Ensures the dynamic library
 * to be loaded before the execution of the test cases.
 * 
 * Also, this class is run with {@link JExample} to define test dependencies.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeTest{

	private static final String testFilesDir = "src/test/resources/";
	private static final String testFilesName = "fr_campus";
	private static JOctree octree; //used in tests
	private static JOctreeKey key1; //used in tests
	private static JOctreeKey key2; //used in tests
	private static Double res; //used in tests
	private static Integer depth; //used in tests
	private File fileRead;
	private File fileWrite;

	/**
	 * Load dynamic library to access the native methods.
	 */
	static {
		System.loadLibrary("joctomap");
	}
	
	/**
	 * Default constructor for this class
	 */
	public JOctreeTest() throws IOException{
		this.fileRead = new File(testFilesDir.concat(testFilesName).concat(
				".ot")).getCanonicalFile();
		this.fileWrite = new File(testFilesName.concat("_test").concat(".ot"))
		.getCanonicalFile();
	}
	
	/**
	 * Unit test for reading a .ot file.
	 * @throws IOException 
	 */
	@Test
	public void test01_readFileOt() throws IOException {
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
	public void test02_writeFileOtTest() throws IOException {
		// write file to compare sizes
		octree.write(fileWrite.getAbsolutePath());
		// compare by content both files
		assertTrue("Input and output files with the same content",
				FileUtils.contentEquals(fileRead, fileWrite));
		// delete the generated file in this test case
		fileWrite.delete();
	}
	
	/**
	 * Unit test for retrieving a {@link JOctreeKey} from a position.
	 * 
	 * @param octree {@link JOctree} instance
	 * @return key that identifies the node
	 */
	@Test
	public void test03_obtainCellKeyAtPositionTest() {
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
	public void test04_obtainCellKeyAtPositionWithDepthTest() {
		key2 = octree.cellKeyAt(0f, 0f, 0f, 1);
		assertTrue("JOctreeKey must not have negative values", key2.getX() >= 0 && key2.getY() >= 0 && key2.getZ() >= 0);
	}
	
	/**
	 * Unit test for testing the adjustement of {@link JOctreeKey} between different
	 * depth levels.
	 * 
	 * @param octree {@link JOctree} instance
	 * @param key {@link JOctreeKey} at the minimum depth
	 * @param keyLevel1 {@link JOctreeKey} at depth 1
	 */
	@Test
	public void test05_adjustDepthOfKey(){
		JOctreeKey adjustement = octree.adjustKeyAt(key2, 1);
		assertTrue("JOctreeKey must not have negative values", adjustement.getX() >= 0 && adjustement.getY() >= 0 && adjustement.getZ() >= 0);
		assertTrue("Adjusted key are not equal to the retrieved one at the same depth", key2.equals(adjustement));
		assertTrue("Adjusted key has a different hash to the retrieved one at the same depth", key2.hashCode() == adjustement.hashCode());
	}
	
	/**
	 * Test case to recover the resolution of the octree.
	 * 
	 * @param octree {@link JOctree} instance
	 * @return octree resolution
	 */
	@Test
	public void test06_getResolutionTest(){
		res = octree.getResolution();
		assertTrue("Resolution must be > 0", res > 0);
	}
	
	/**
	 * Test case to recover the depth of the octree.
	 * 
	 * @param octree {@link JOctree} instance
	 * @return octree maximum depth
	 */
	@Test
	public void test07_getTreeDepthTest(){
		depth = octree.getTreeDepth();
		assertTrue("Tree depth must be > 0", depth > 0);
	}
	
	/**
	 * Test case to recover the node size of the octree. 
	 * Checks if the size at the maximum depth is equal to the
	 * minimum resolution of the octree.
	 * 
	 * @param octree {@link JOctree} instance
	 * @param res octree minimum resolution
	 * @param depth octree maximum depth
	 */
	@Test
	public void test08_getNodeSizeTest(){
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
	public void test09_searchWithKeyTest(){
		JOctreeNode node = octree.search(key1, 0);
		assertTrue("Native pointer of node is not assigned", node.getPointer() != 0);
	}
	
	/**
	 * Test case to search a node in the octree, given a 3D position (search
	 * with depth = 0)
	 * 
	 * @param octree {@link JOctree} instance
	 * @param key {@link JOctreeKey} instance
	 */
	@Test
	public void test10_searchWith3DPositionTest(){
		JOctreeNode node = octree.search(0f, 0f, 0f, 0);
		assertTrue("Native pointer of node is not assigned", node.getPointer() != 0);
	}
}
