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
package es.usc.citius.lab.joctomap.util;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.motionplanner.core.spatial.Point;
import es.usc.citius.lab.motionplanner.core.spatial.Pose;
import java.io.IOException;

/**
 * This class implements a collision checker which uses native methods to determine
 * the collision in a 2D octree using 2D shapes (circular and rectangular).
 *
 * @author Adrián González Sieira {@literal <adrian.gonzalez@usc.es>}
 */
public class CollisionChecker2D {
    
    //place to load the dynamic libraries used in the code
    static{
        try {
            NativeUtils.loadLibraryFromJar("/libjoctomap_natives.so");
        } catch (IOException ex) {
            throw new RuntimeException(ex);
        }
    }
    
    /**
     * Checks if there is a collision given an octree and a curren pose, 
     * considering a circular shape of the robot centered in the pose
     * given as parameter.
     * 
     * @param octree instance of octree
     * @param pose pose where the shape is rotated
     * @param radius radius of the circular shape
     * @return true if there is a collision with the obstacles in the map
     */
    public static native boolean collides(JOctree octree, Point pose, float radius);
    
    /**
     * Checks if there is a collision given an octree and a curren pose, 
     * considering a rectangular shape, defined by a distance in x and y from
     * the robot center of rotation, which is centered in the pose given as 
     * parameter.
     * 
     * @param octree instance of octree
     * @param pose pose where the shape is rotated
     * @param dx distance in x from the rotation center to the borders
     * @param dy distance in y from the rotation center to the borders
     * @return true if there is a collision with the obstacles in the map
     */
    public static native boolean collides(JOctree octree, Pose pose, float dx, float dy);
    
}
