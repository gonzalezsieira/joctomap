package es.usc.citius.lab.joctomap.distance;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.util.NativeObject;
import es.usc.citius.lab.joctomap.util.Obstacle;

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
	 * Retrieves the distance to the nearest obstacle, if there is one
	 * in the radius of the "maxDistance" of this distance max.
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	public native double getDistance(double x, double y, double z);
	
	/**
	 * Retrieves the distance to the nearest obstacle given the
	 * {@link JOctreeKey} of a node as origin.
	 * 
	 * @param key
	 * @return
	 */
	public native double getDistance(JOctreeKey key);
	
	/**
	 * Retrieves the distance and location of the nearest obstacle to the given
	 * 3D point. If this method does not find any obstacle in the maximum
	 * distance (specified at the creation of the distance map), this method returns null.
	 * @param x
	 * @param y
	 * @param z
	 * @return <distance, location> of the nearest obstacle, null if no obstacle found at the maximum distance of this map
	 */
	public native Obstacle getDistanceAndClosestObstacle(double x, double y, double z);
	
	/**
	 * Obtains a new instance of {@link JOctreeDistanceMap} from a {@link JOctree}
	 * instance. Note that this method only creates the distance map, but not really calculates
	 * the distances in all points of the space. To do this, execute the method {@link #update()}.
	 * 
	 * @param octree {@link JOctree} instance
	 * @param maxDistance max distance to calculate the map
	 * @return distance map
	 */
	public static native JOctreeDistanceMap create(JOctree octree, double maxDistance);

}
