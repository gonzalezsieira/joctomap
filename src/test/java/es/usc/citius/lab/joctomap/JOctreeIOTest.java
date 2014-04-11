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
				octree.getOctreePointer() != 0);
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
	
	@Test
	@Given("#readFileOtTest")
	public void obtainCellKeyAtPosition(JOctree octree) {
		octree.cellKeyAt(0f, 0f, 0f);
	}

	@Test
	@Given("#readFileOtTest")
	public void obtainCellKeyAtPositionWithDepth(JOctree octree) {
		octree.cellKeyAt(0f, 0f, 0f, 1);
	}
}
