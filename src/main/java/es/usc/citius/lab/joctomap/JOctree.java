package es.usc.citius.lab.joctomap;

public class JOctree {
	
	/**
	 * 
	 * @param octreePointer
	 * @param filename
	 * @return
	 */
	public static native boolean write(long octreePointer, String filename);
	
	/**
	 * 
	 * @param filename
	 * @return
	 */
	public static native long read(String filename);
	
}
