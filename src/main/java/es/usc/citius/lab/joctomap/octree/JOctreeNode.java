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
package es.usc.citius.lab.joctomap.octree;

import java.util.List;

import es.usc.citius.lab.joctomap.util.NativeObject;


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

        @Override
        public native void dispose();
        
        @Override
        protected void finalize() throws Throwable {
            super.finalize(); //To change body of generated methods, choose Tools | Templates.
            dispose();
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
