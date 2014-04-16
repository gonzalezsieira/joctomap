package es.usc.citius.lab.joctomap;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.junit.Test;
import org.junit.runner.RunWith;

import ch.unibe.jexample.Given;
import ch.unibe.jexample.JExample;
import static org.junit.Assert.*;

/**
 * Unit test for octree objects. Ensures the dynamic library
 * to be loaded before the execution of the test cases.
 * 
 * Also, this class is run with {@link JExample} to define test dependencies.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@RunWith(JExample.class)
public class JOctreeTest{

	private static final String testFilesDir = "src/test/resources/";
	private static final String testFilesName = "fr_campus";
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
	public JOctree readFileOtTest() throws IOException {
		JOctree octree = JOctree.read(fileRead.getAbsolutePath());
		assertTrue("Octree direction of memory not assigned",
				octree.getPointer() != 0);
		return octree;
	}

	/**
	 * Unit test for writing a .ot file.
	 * 
	 * @throws IOException
	 */
	@Test
	@Given("#readFileOtTest")
	public void writeFileOtTest(JOctree octree) throws IOException {
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
	@Given("#readFileOtTest")
	public JOctreeKey obtainCellKeyAtPositionTest(JOctree octree) {
		JOctreeKey key = octree.cellKeyAt(0f, 0f, 0f);
		assertTrue("JOctreeKey must not have negative values", key.getX() >= 0 && key.getY() >= 0 && key.getZ() >= 0);
		return key;
	}

	/**
	 * Unit test for retrieving a {@link JOctreeKey} from a position and depth
	 * 
	 * @param octree {@link JOctree} instance
	 * @return key that identifies the node
	 */
	@Test
	@Given("#readFileOtTest")
	public JOctreeKey obtainCellKeyAtPositionWithDepthTest(JOctree octree) {
		JOctreeKey key = octree.cellKeyAt(0f, 0f, 0f, 1);
		assertTrue("JOctreeKey must not have negative values", key.getX() >= 0 && key.getY() >= 0 && key.getZ() >= 0);
		return key;
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
	@Given("#readFileOtTest, #obtainCellKeyAtPositionTest, #obtainCellKeyAtPositionWithDepthTest")
	public void adjustDepthOfKey(JOctree octree, JOctreeKey key, JOctreeKey keyLevel1){
		JOctreeKey adjustement = octree.adjustKeyAt(key, 1);
		assertTrue("JOctreeKey must not have negative values", adjustement.getX() >= 0 && adjustement.getY() >= 0 && adjustement.getZ() >= 0);
		assertTrue("Adjusted key are not equal to the retrieved one at the same depth", keyLevel1.equals(adjustement));
		assertTrue("Adjusted key has a different hash to the retrieved one at the same depth", keyLevel1.hashCode() == adjustement.hashCode());
	}
	
	/**
	 * Test case to recover the resolution of the octree.
	 * 
	 * @param octree {@link JOctree} instance
	 * @return octree resolution
	 */
	@Test
	@Given("#readFileOtTest")
	public double getResolutionTest(JOctree octree){
		double res = octree.getResolution();
		assertTrue("Resolution must be > 0", res > 0);
		return res;
	}
	
	/**
	 * Test case to recover the depth of the octree.
	 * 
	 * @param octree {@link JOctree} instance
	 * @return octree maximum depth
	 */
	@Test
	@Given("#readFileOtTest")
	public int getTreeDepthTest(JOctree octree){
		int depth = octree.getTreeDepth();
		assertTrue("Tree depth must be > 0", depth > 0);
		return depth;
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
	@Given("#readFileOtTest, #getResolutionTest, #getTreeDepthTest")
	public void getNodeSizeTest(JOctree octree, double res, int depth){
		double size = octree.getNodeSize(depth);
		assertTrue("Node size must be > 0", size > 0);
		assertTrue("Minimum node size must be equal to the octree resolution", Double.compare(res, size) == 0);
	}
}
