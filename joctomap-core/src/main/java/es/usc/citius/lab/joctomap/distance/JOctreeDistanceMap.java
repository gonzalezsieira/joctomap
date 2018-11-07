/**
 * Copyright (C) 2014-2017 Adrián González Sieira (adrian.gonzalez@usc.es)
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
package es.usc.citius.lab.joctomap.distance;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.util.NativeObject;
import es.usc.citius.lab.joctomap.util.Obstacle;

/**
 * Represents a map distance based in a {@link JOctree} structure,
 * allowing queries as "nearest obstacle to".
 *
 * @author Adrián González Sieira {@literal <adrian.gonzalez@usc.es>}
 */
public class JOctreeDistanceMap extends NativeObject{
	
	/**
	 * Initializes this object with the pointer to the native object
	 * of the library.
	 *
	 * @param pointer pointer to the native object (JNI global reference)
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
	 *
	 * @param x query point, coordinate X
	 * @param y query point, coordinate Y
	 * @param z query point, coordinate Z
	 * @return distance to the nearest obstacle
	 */
	public native double getDistance(double x, double y, double z);
	
	/**
	 * Retrieves the distance to the nearest obstacle given the
	 * {@link JOctreeKey} of a node as origin.
	 * 
	 * @param key {@link JOctreeKey} of the cell to query
	 * @return distance to the nearest obstacle
	 */
	public native double getDistance(JOctreeKey key);
	
	/**
	 * Retrieves the distance and location of the nearest obstacle to the given
	 * 3D point. If this method does not find any obstacle in the maximum
	 * distance (specified at the creation of the distance map), this method returns null.
	 * @param x query point, coordinate X
	 * @param y query point, coordinate Y
	 * @param z query point, coordinate Z
	 * @return {@literal <distance, location>} of the nearest obstacle, null if no obstacle found at the maximum distance of this map
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

        @Override
        public native void dispose();
        
        @Override
        protected void finalize() throws Throwable {
            super.finalize(); //To change body of generated methods, choose Tools | Templates.
            dispose();
        }
}
