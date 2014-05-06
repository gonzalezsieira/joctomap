package es.usc.citius.lab.joctomap.distance;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.util.NativeObject;

/**
 * Represents a map distance based in a {@link JOctree} structure,
 * allowing queries as "nearest obstacle to".
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class JOctreeDistanceMap extends NativeObject{
	
	/**
	 * Initializes this object with the pointer to the native object
	 * of the library.
	 * 
	 * @param pointer
	 */
	private JOctreeDistanceMap(long pointer) {
		super(pointer);
	}
	
	/**
	 * Executes the update operation of the distance map.
	 */
	public native void update();
	
	/**
	 * Obtains a new instance of {@link JOctreeDistanceMap} from a {@link JOctree}
	 * instance with all distances calculated.
	 * 
	 * @param octree {@link JOctree} instance
	 * @param maxDistance max distance to calculate the map
	 * @return distance map
	 */
	public static native JOctreeDistanceMap create(JOctree octree, double maxDistance);

}
