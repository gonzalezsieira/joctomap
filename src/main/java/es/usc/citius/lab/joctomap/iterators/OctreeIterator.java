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
package es.usc.citius.lab.joctomap.iterators;

import java.util.Iterator;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.octree.JOctreeNode;
import es.usc.citius.lab.joctomap.util.NativeObject;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;

/**
 * Class that manages iterators based in {@link JOctree} objects. These iterators manage
 * {@link JOctreeKey} elements, but they allow to retrieve additional information of the 
 * current element using the methods {@link #key()}, {@link #coordinate()}, {@link #depth()},
 * {@link #node()}, {@link #x()} and {@link #y()}.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public abstract class OctreeIterator extends NativeObject implements Iterator<JOctreeKey>{

	/**
	 * Default constructor for this class.
	 * 	
	 * @param pointer pointer to a native object that manages iterators
	 */
	public OctreeIterator(long pointer) {
		super(pointer);
	}

	/**
	 * @return the {@link JOctreeKey} at the current position of the iterator.
	 */
	public abstract JOctreeKey key();
	
	/**
	 * @return depth of the current node.
	 */
	public abstract int depth();
	
	/**
	 * @return {@link Point3D} that gives the 3D coordinates of the center of the current node
	 */
	public abstract Point3D coordinate();
	
	/**
	 * @return x coordinate of the current node
	 */
	public abstract float x();
	
	/**
	 * @return y coordinate of the current node
	 */
	public abstract float y();
	
	/**
	 * @return z coordinate of the current node
	 */
	public abstract float z();
	
	/**
	 * @return size of the current node
	 */
	public abstract float size();
	
	/**
	 * @return current {@link JOctreeNode}
	 */
	public abstract JOctreeNode node();
	
	@Override
	public void remove() {
		throw new UnsupportedOperationException("OctreeIterator does not support remove operation");
	}
	
}
