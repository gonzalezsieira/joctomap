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

import java.io.Serializable;


/**
 * Represents a key of the {@link JOctree}, which is the identifier
 * used to store the nodes in the data structure. This is not a native
 * object wrapper since it has not associated functions.
 * 
 * @author Adrián González Sieira {@literal <adrian.gonzalez@usc.es>}
 */
public class JOctreeKey implements Serializable{

	private int x;
	private int y;
	private int z;
	
	/**
	 * Default constructor that initializes the 3D coordinates
	 * of the key.
	 *
	 * @param x key, coordinate X
	 * @param y key, coordinate Y
	 * @param z key, coordinate Z
	 */
	public JOctreeKey(int x, int y, int z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + x;
		result = prime * result + y;
		result = prime * result + z;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		JOctreeKey other = (JOctreeKey) obj;
		if (x != other.x)
			return false;
		if (y != other.y)
			return false;
		if (z != other.z)
			return false;
		return true;
	}
	
	@Override
	public String toString() {
		return "{" + x + ", " + y + ", " + z + "}";
	}

	/**
	 * @return key, coordinate X
	 */
	public int getX() {
		return x;
	}

	/**
	 * @return key, coordinate Y
	 */
	public int getY() {
		return y;
	}

	/**
	 * @return key, coordinate Z
	 */
	public int getZ() {
		return z;
	}
	
}
