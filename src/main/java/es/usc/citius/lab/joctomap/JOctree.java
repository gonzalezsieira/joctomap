package es.usc.citius.lab.joctomap;

public class JOctree {
	
	//used by native methods
	private long pointer;
	
	/**
	 * Initializes the JOctree with a pointer to the native OcTree object of
	 * the Octomap library. Used by native methods.
	 * 
	 * @param pointer pointer to the native OcTree object.
	 */
	public JOctree(long pointer){
		this.pointer = pointer;
	}
	
	/**
	 * Retrieves the assigned octree pointer
	 * 
	 * @return pointer to the native OcTree
	 */
	public long getOctreePointer() {
		return pointer;
	}
	
	/**
	 * Retrieves the position of the cell at a current 
	 * location.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 */
	public native void cellKeyAt(float x, float y, float z);
	
	/**
	 * Retrieves the position of the cell at a current 
	 * location, at a given depth.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 * @param depth
	 */
	public native void cellKeyAt(float x, float y, float z, int depth);
	
	
	/*
	 * *******************************************************************************
	 * *							I/O functions                                    *
	 * *******************************************************************************
	 */
	/**
	 * Native implementation of the native function "write to file", that stores the 
	 * information contained in the pointer of the direction of memory of the native 
	 * OcTree object in the given path.
	 * 
	 * @param filename path to store the file
	 * @return true if the file has correctly been stored
	 */
	public native boolean write(String filename);
	
	/**
	 * Native implementation of the native function "read from file", that loads
	 * the information using the native implementation of Octomap and returns a new instance
	 * of {@link JOctree}.
	 * 
	 * @param filename name of file to read
	 * @return pointer of the direction of memory of the loaded native OcTree object
	 */
	public static native JOctree read(String filename);
	
}
