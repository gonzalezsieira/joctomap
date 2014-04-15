package es.usc.citius.lab.joctomap;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runner.RunWith;

import ch.unibe.jexample.Given;
import ch.unibe.jexample.JExample;
import static org.junit.Assert.*;

/**
 * Unit test for I/O over octree objects. The {@link FixMethodOrder} policy
 * ensures that the execution of the methods will be: read first, write last,
 * avoiding errors due to the lack of ordering of the execution of the test
 * cases in previous JUnit versions.
 */
@RunWith(JExample.class)
public class JOctreeIOTest {

	/**
	 * Load dynamic library to access the native I/O methods.
	 */
	static {
		System.loadLibrary("joctomap");
	}

	private static final String testFilesDir = "src/test/resources/";
	private static final String testFilesName = "fr_campus";
	private File fileRead;
	private File fileWrite;

	/**
	 * Default constructor for this class
	 */
	public JOctreeIOTest() throws IOException{
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
}
