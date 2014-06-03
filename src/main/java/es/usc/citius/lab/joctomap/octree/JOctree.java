package es.usc.citius.lab.joctomap.octree;

import es.usc.citius.lab.joctomap.iterators.OctreeIterator;
import es.usc.citius.lab.joctomap.util.NativeObject;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;

public class JOctree extends NativeObject{
	
	//used by native methods
	private long pointer;
	
	/**
	 * Initializes the JOctree with a pointer to the native OcTree object of
	 * the Octomap library. Used by native methods.
	 * 
	 * @param pointer pointer to the native OcTree object.
	 */
	private JOctree(long pointer) {
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
	public native JOctreeKey coordToKey(float x, float y, float z);
	
	/**
	 * Retrieves the position of the cell at a current 
	 * location, at a given depth.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 * @param depth level to search (depth=0 means search in the full octree)
	 * @return key of the node at the given position and depth
	 */
	public native JOctreeKey coordToKey(float x, float y, float z, int depth);
	
	/**
	 * Given an {@link JOctreeKey}, adjusts it to a given depth in the octree.
	 * 
	 * @param key to be adjusted
	 * @param depth to obtain the adjusted {@link JOctreeKey}
	 * @return adjusted {@link JOctreeKey} to the specified depth
	 */
	public native JOctreeKey adjustKeyAtDepth(JOctreeKey key, int depth);
	
	/**
	 * Given a {@link Point3D}, retrieves the {@link JOctreeKey} of the node
	 * that occupies that position.
	 * 
	 * @param point coordinates to retrieve the corresponding key
	 * @return {@link JOctreeKey} of the node that occupies the queried position
	 */
	public native JOctreeKey coordToKey(Point3D point);
	
	/**
	 * Given a {@link Point3D}, retrieves the {@link JOctreeKey} of the node
	 * that occupies that position at a current depth level.
	 * 
	 * @param point coordinate to query the key of the node at that position
	 * @param depth of the {@link JOctreeKey} in the octree
	 * @return
	 */
	public native JOctreeKey coordToKey(Point3D point, int depth);
	
	/**
	 * Retrieves the {@link Point3D} that corresponds to the coordinates of the
	 * center of the node identified by the given its {@link JOctreeKey}.
	 * 
	 * @param key identifier of the node
	 * @return coordinates of the center of the node
	 */
	public native Point3D keyToCoord(JOctreeKey key);
	
	/**
	 * Retrieves the {@link Point3D} that corresponds to the coordinates of the
	 * center of the node identified by the given its {@link JOctreeKey} and the
	 * depth of the node.
	 * 
	 * @param key identifier of the node
	 * @param depth of the {@link JOctreeKey} in the octree
	 * @return coordinates of the center of the node
	 */
	public native Point3D keyToCoord(JOctreeKey key, int depth);
	
	/*
	 * *******************************************************************************
	 * *					Node query functions	                             	 *
	 * *******************************************************************************
	 */
	/**
	 * Retrieves a node of the octree given the identifier of the node ({@link JOctreeKey}) and
	 * the depth to search in the {@link JOctree}.
	 * 
	 * @param key identifier of the node
	 * @param depth level to search (depth=0 means search in the full octree)
	 * @return node of the octree, if found, null otherwhise
	 */
	public native JOctreeNode search(JOctreeKey key, int depth);
	
	/**
	 * Calls the method {@link #search(JOctreeKey, int)} with the 
	 * default value depth=0. Default values in argument methods are 
	 * not supported by Java, so this method overload tries to emulate 
	 * the call with the default value depth = 0;
	 * 
	 * @param key identifier of the node
	 * @return node of the octree, if found, null otherwhise
	 */
	public JOctreeNode search(JOctreeKey key){
		return search(key, 0);
	}
	
	/**
	 * Retrieves a node of the {@link JOctree} in a given position of the 3D space and a depth level
	 * of the octree.
	 * 
	 * @param x 
	 * @param y
	 * @param z
	 * @param depth level to search (depth=0 means search in the full octree)
	 * @return node of the octree, if found, null otherwhise
	 */
	public native JOctreeNode search(float x, float y, float z, int depth);
	
	/**
	 * Calls the method {@link #search(float, float, float, int)} with the default value of 
	 * depth=0. Default values in argument methods are not supported by Java, so this method overload
	 * tries to emulate the call with the default value depth = 0;
	 * @param x
	 * @param y
	 * @param z
	 * @return node of the octree, if found, null otherwise
	 */
	public JOctreeNode search(float x, float y, float z){
		return search(x, y, z, 0);
	}
	
	/**
	 * Queries if a node is occupied, according to its occupancy probability.
	 * 
	 * @param node node to query if is occupied
	 * @return result of the query
	 */
	public native boolean isNodeOccupied(JOctreeNode node);
	
	
	/*
	 * *******************************************************************************
	 * *					Update functions		                             	 *
	 * *******************************************************************************
	 */
	/**
	 * Updates the occupancy information of a {@link JOctreeNode}, retrieving the node 
	 * instance affected.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 * @param occupied
	 * @return {@link JOctreeNode} affected
	 */
	public native JOctreeNode updateNode(float x, float y, float z, boolean occupied);
	
	/**
	 * Enables or disables the usage of the bounding box to limit the octree 
	 * update region.
	 * 
	 * @param value true/false
	 */
	public native void useBBXLimit(boolean value);
	
	/**
	 * Updates the bounding box limits.
	 * 
	 * @param min minimum {@link Point3D} of the bounding box
	 * @param max maximum {@link Point3D} of the bounding box
	 */
	public native void setBBX(Point3D min, Point3D max);
	
	/**
	 * Updates the minimum bounding box limit.
	 * 
	 * @param min {@link Point3D} of the bounding box
	 */
	public native void setBBXMin(Point3D min);
	
	/**
	 * Updates the maximum bounding box limit.
	 * 
	 * @param max {@link Point3D} of the bounding box
	 */
	public native void setBBXMax(Point3D max);
	
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
	public native float getResolution();
	
	/**
	 * Retrieves the size of the nodes at a given depth (aka. resolution at a given depth).
	 * 
	 * @param depth object of the query
	 * @return node resolution at the depth level
	 */
	public native float getNodeSize(int depth);
	
	/**
	 * Returns the central point of the BBX queries, if set.
	 * 
	 * @return {@link Point3D} instance with the center of the BBX, null if not set
	 */
	public native Point3D getBBXCenter();
	
	/**
	 * Returns the min point of the BBX queries, if set.
	 * 
	 * @return {@link Point3D} instance with the min of the BBX, null if not set
	 */
	public native Point3D getBBXMin();
	
	/**
	 * Returns the max point of the BBX queries, if set.
	 * 
	 * @return {@link Point3D} instance with the max of the BBX, null if not set
	 */
	public native Point3D getBBXMax();
	
	/**
	 * Minimum point of the bounding box of the known space in the octree.
	 * 
	 * @return the minimum {@link Point3D} of the known space
	 */
	public native Point3D getMetricMin();
	
	/**
	 * Maximum point of the bounding box of the known space in the octree.
	 * 
	 * @return the maximum {@link Point3D} of the known space
	 */
	public native Point3D getMetricMax();
	
	/**
	 * Dimensions of the bounding box that contains the known space of the octree.
	 * 
	 * @return a float[3] array with the dimensions x, y and z of the known space
	 */
	public native float[] getMetricSize();
	
	/**
	 * Queries if the bounding box is set. Returns true if {@link #getBBXMin()} and {@link #getBBXMax()}
	 * return the same value, as the BBX with the same beginning and ending point does not exist.
	 * 
	 * @return true if {@link #getBBXMin()} != {@link #getBBXMax()}
	 * @see #setBBX(Point3D, Point3D)
	 */
	public native boolean isBBXSet();
	
	/**
	 * Queries if the bounding box is being applied to the octree. When it is, 
	 * the octree only applies updates for the region within the bounding box.
	 * 
	 * @return true if a BBX being applied in the octree, false otherwise
	 * @see #useBBXLimit(boolean)
	 */
	public native boolean isBBXApplied();
	
	/*
	 * *******************************************************************************
	 * *				Iterator retrieval functions	                             *
	 * *******************************************************************************
	 */
	/**
	 * Retrieve a bounding box iterator over the {@link JOctreeKey} of the leaf nodes 
	 * of an octree. It is possible to define a maximum depth of the elements returned by
	 * the iterator.
	 * 
	 * @param min min position of the bbx
	 * @param max max position of the bbx
	 * @param maxDepth max depth of the leaf nodes
	 * @return {@link JOctreeKey} iterator
	 */
	public native OctreeIterator leafBBXIterator(Point3D min, Point3D max, int maxDepth);
	
	/**
	 * Retrieve bounding box iterator over {@link JOctreeKey} of the leaf nodes of an octree.
	 * It is possible to define a maximum depth of the elements returned by
	 * the iterator.
	 * 
	 * @param min min node key of the bbx
	 * @param max max node key of the bbx
	 * @param maxDepth max depth of the leaf nodes
	 * @return {@link JOctreeKey} iterator
	 */
	public native OctreeIterator leafBBXIterator(JOctreeKey min, JOctreeKey max, int maxDepth);
	
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
	
	/**
	 * Creates a new {@link JOctree} with a minimum resolution. An empty octree is not fully
	 * usable (you cannot query information about the bounds of the octree, or query the 
	 * {@link JOctreeKey} corresponding to a posiition) until the first occupancy information 
	 * is added using the modification methods like {@link #updateNode(float, float, float, boolean)}.
	 * 
	 * @param res minimum node resolution
	 * @return octree with the given resolution
	 */
	public static native JOctree create(float res);
	
}
