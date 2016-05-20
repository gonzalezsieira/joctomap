package es.usc.citius.lab.joctomap.util;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import es.usc.citius.lab.motionplanner.core.util.Pair;
import java.io.IOException;
import java.util.Collection;

/**
 * Contains utilities implemented for the {@link JOctree}, but not included
 * in the C/C++ implementation of Octomap.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class JOctreeUtils {
    
    //place to load the dynamic libraries used in the code
    static{
        try {
            NativeUtils.loadLibraryFromJar("/joctomap-natives/libjoctomap.so");
        } catch (IOException ex) {
            throw new RuntimeException(ex);
        }
    }
    
    /**
     * Retrieves a collection with the keys of the occupied cells within
     * a minimum and maximum 3D positions; the low level operations
     * of this method include generate an iterator and check the 
     * occupancy of the existing nodes between the given positions.
     * 
     * @param octree instance of {@link JOctree}
     * @param min minimum position of the range to check
     * @param max maximum position of the range to check
     * @return collection of keys of the occupied cells in the range
     */
    public native static Collection<JOctreeKey> getOccupiedKeysBetween(JOctree octree, Point3D min, Point3D max);
    
    /**
     * Retrieves a collection with the pairs: size of the occupied cells within
     * a minimum and maximum 3D positions; the low level operations
     * of this method include generate an iterator and check the 
     * occupancy of the existing nodes between the given positions.
     * 
     * @param octree instance of {@link JOctree}
     * @param min minimum position of the range to check
     * @param max maximum position of the range to check
     * @return collection of pair(coordinate, size) of the occupied cells in the range
     */
    public native static Collection<Pair<Point3D, Float>> getOccupiedCoordinatesBetween(JOctree octree, Point3D min, Point3D max);
    
    /**
     * Queries the resolution at a current point of the octree; retrieves Float.MAX_VALUE
     * if the point has not correspondence with a cell in the map.
     * 
     * @param octree instance of {@link JOctree}
     * @param point position to query
     * @return resolution of the cell at the given point, Float.MAX_VALUE if doesn't exist in the map
     */
    public native static float resolutionAt(JOctree octree, Point3D point);
    
    /**
     * Queries the resolution between the given points; this is, the resolution at the first
     * point added to the resolution in the second point.
     * 
     * @param octree instance of {@link JOctree}
     * @param point1 position to query
     * @param point2 position to query
     * @return resolution of the cell at the given point, Float.MAX_VALUE if any point don't exist in the map
     */
    public native static float resolutionAddedIn(JOctree octree, Point3D point1, Point3D point2);
    
}
