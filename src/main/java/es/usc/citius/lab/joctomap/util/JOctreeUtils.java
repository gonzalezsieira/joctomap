/**
 * Copyright (C) 2014-2016 Adrián González Sieira (adrian.gonzalez@usc.es)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package es.usc.citius.lab.joctomap.util;

import es.usc.citius.hipster.model.Transition;
import es.usc.citius.hipster.model.impl.WeightedNode;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.motionplanner.core.spatial.Point2D;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import es.usc.citius.lab.motionplanner.core.util.Pair;
import java.io.IOException;
import java.util.Collection;
import java.util.Map;

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
    
    /**
     * Calculates and returns the available transitions from a given state, acting as transition function of the 
     * Multiresolution H2D heuristic (H2DMR). The native implementation provides a better efficiency of managing the
     * calls to the API of Octomap.
     *  
     * @param octree instance of {@link JOctree}
     * @param adjacencyMap instance of {@link AdjacencyMap}
     * @param neighborsByDirection angles/points of neighbors at minimum resolution
     * @param state position to calculate the neighbors from
     * @param minimumResolutionTrajectories lowest resolution between states in the trajectory set
     * @param radius optimistic size of the robot to check collisions on the neighbor positions
     * @param maxDepth level of resolution of the map corresponding with the minimum distance of trajectories
     * @return
     */
    //TODO: This method should be in a subproject of motionplanner
    public native static Iterable<Transition<Void, Point2D>> availableH2DMRTransitions(JOctree octree, AdjacencyMap adjacencyMap, float[] directionsNeighbors, Map<Float, Point2D> neighborsByDirection, float radius, float minimumResolutionTrajectories, int maxDepth, Point2D state);
    
    //TODO: This method should be in a subproject of motionplanner
    public native static Pair<Double, WeightedNode<Void, Point2D, Double>> queryClosed(JOctree octree, AdjacencyMap adjacencyMap, Point2D point, Map<Point2D, WeightedNode<Void, Point2D, Double>> closedList, float speed);
}
