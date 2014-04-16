package es.usc.citius.lab.joctomap;

public class JOctree extends NativeObject{
	
	//used by native methods
	private long pointer;
	
	/**
	 * Initializes the JOctree with a pointer to the native OcTree object of
	 * the Octomap library. Used by native methods.
	 * 
	 * @param pointer pointer to the native OcTree object.
	 */
	public JOctree(long pointer) {
		super(pointer);
	}
	
	
	/*
	 * *******************************************************************************
	 * *					Position query functions	                             *
	 * *******************************************************************************
	 */
	/**
	 * Retrieves the position of the cell at a current 
	 * location.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 * @return key of the node at the given position
	 */
	public native JOctreeKey cellKeyAt(float x, float y, float z);
	
	/**
	 * Retrieves the position of the cell at a current 
	 * location, at a given depth.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 * @param depth
	 * @return key of the node at the given position and depth
	 */
	public native JOctreeKey cellKeyAt(float x, float y, float z, int depth);
	
	/**
	 * Given an {@link JOctreeKey}, adjusts it to a given depth in the octree.
	 * 
	 * @param key to be adjusted
	 * @param depth to obtain the adjusted {@link JOctreeKey}
	 * @return adjusted {@link JOctreeKey} to the specified depth
	 */
	public native JOctreeKey adjustKeyAt(JOctreeKey key, int depth);
	
	
	/*
	 * *******************************************************************************
	 * *				Octree information query functions                           *
	 * *******************************************************************************
	 */
	/**
	 * Retrieves the maximum depth of the octree.
	 * 
	 * @return maximum depth value
	 */
	public native int getTreeDepth();
	
	/**
	 * Retrieves the minimum resolution of the octree nodes.
	 * 
	 * @return minimum node resolution
	 */
	public native double getResolution();
	
	/**
	 * Retrieves the size of the nodes at a given depth (aka. resolution at a given depth).
	 * 
	 * @param depth object of the query
	 * @return node resolution at the depth level
	 */
	public native double getNodeSize(int depth);
	
	
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
