package es.usc.citius.lab.joctomap;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.junit.Before;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

import static org.junit.Assert.*;

/**
 * Unit test for I/O over octree objects. The {@link FixMethodOrder} policy
 * ensures that the execution of the methods will be: read first, write last,
 * avoiding errors due to the lack of ordering of the execution of the test
 * cases in previous JUnit versions.
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeIOTest {

	/**
	 * Load dynamic library to access the native I/O methods.
	 */
	static {
		System.loadLibrary("joctomap");
	}

	private File fileRead;
	private File fileWrite;
	private static final String testFilesDir = "src/test/resources/";
	private static final String testFilesName = "fr_campus";

	/**
	 * Initialize data for I/O tests.
	 * 
	 * @throws IOException 
	 */
	@Before
	public void initializeParams() throws IOException {
		// get canonical files
		this.fileRead = new File(testFilesDir.concat(testFilesName)
				.concat(".ot")).getCanonicalFile();
		this.fileWrite = new File(testFilesName.concat("_test").concat(".ot"))
				.getCanonicalFile();
	}

	/**
	 * Unit test for reading a .ot file.
	 */
	@Test
	public void readFileOt() {
		long octreeOt = JOctree.read(fileRead.getAbsolutePath());
		assertTrue("Octree direction of memory is assigned: " + octreeOt,
				octreeOt != 0);
	}

	/**
	 * Unit test for writing a .ot file.
	 * 
	 * @throws IOException 
	 */
	@Test
	public void writeFileOt() throws IOException {
		//read ot
		long octreeOt = JOctree.read(fileRead.getAbsolutePath());
		//write file to compare sizes
		JOctree.write(octreeOt, fileWrite.getAbsolutePath());
		//compare by content both files
		assertTrue("Input and output files with the same content",
				FileUtils.contentEquals(fileRead, fileWrite));
		//delete the generated file in this test case
		fileWrite.delete();
	}
}
