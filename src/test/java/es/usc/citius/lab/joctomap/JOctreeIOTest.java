package es.usc.citius.lab.joctomap;

import org.junit.Test;

/**
 * Unit test for I/O over octrees.
 */
public class JOctreeIOTest{
	
	/**
	 * Load dynamic library.
	 */
	static {
		System.loadLibrary("joctomap");
	}
	
	private JOctree joctree;
	
	/**
	 * Initialize data for I/O tests.
	 */
	public JOctreeIOTest() {
		this.joctree = new JOctree();
	}
	
	/**
	 * Unit test for reading a .bt file.
	 */
	@Test
	public void readFileBt(){
		joctree.read("bt file");
	}
	
	/**
	 * Unit test for reading a .ot file.
	 */
	@Test
	public void readFileOt(){
		joctree.read("ot file");
	}
	
	/**
	 * Unit test for writing a .bt file.
	 */
	@Test
	public void writeFileBt(){
		joctree.write(0, "bt file");
	}
	
	/**
	 * Unit test for writing a .ot file.
	 */
	@Test
	public void writeFileOt(){
		joctree.write(0, "ot file");
	}
	
}
