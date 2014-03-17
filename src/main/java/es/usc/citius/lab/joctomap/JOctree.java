package es.usc.citius.lab.joctomap;

public class JOctree {
	
	/**
	 * 
	 * @param octreePointer
	 * @param filename
	 * @return
	 */
	public native boolean write(long octreePointer, String filename);
	
	/**
	 * 
	 * @param filename
	 * @return
	 */
	public native long read(String filename);
	
}
