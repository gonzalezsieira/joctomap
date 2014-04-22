package es.usc.citius.lab.joctomap;

/**
 * Represents a node of the {@link JOctree}, which is identified
 * with a unique {@link JOctreeKey}. The node contains information
 * about the occupancy in a region of the map. Also, in unexplored
 * regions of the map, there is no nodes to represent that region.
 * Since this is a native object, it extends to {@link NativeObject}
 * to store the pointer properly.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class JOctreeNode extends NativeObject{

	/**
	 * Default constructor for the class, that initializes
	 * the pointer to the native object.
	 * 
	 * @param pointer to the native object
	 */
	public JOctreeNode(long pointer) {
		super(pointer);
	}
	
	/**
	 * Retrieves the probability of occupancy of the node.
	 * 
	 * @return [0, 1] probability of occupancy
	 */
	public native double getOccupancy();
	
	/**
	 * Retrieves the value stored in the node.
	 * 
	 * @return current node value
	 */
	public native float getValue();

}
