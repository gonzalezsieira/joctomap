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
package es.usc.citius.lab.joctomap.distance;

import org.junit.BeforeClass;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

import es.usc.citius.lab.joctomap.JOctreeTest;
import es.usc.citius.lab.joctomap.octree.JOctree;

/**
 * Executes tests over the {@link JOctreeDistanceMap} class. Tests
 * are executed in ascending order given their name.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeDistanceMapTest {

	private static JOctree octree;
	private static JOctreeDistanceMap distanceMap;
	
	/**
	 * Retrieves the empty octree initialized in {@link JOctreeTest}.
	 */
	@BeforeClass
	public static void initialize(){
		octree = JOctreeTest.getOctree();
	}
	
	/**
	 * Executes the creation of the distance map.
	 */
	@Test
	public void test01_Create() {
		distanceMap = JOctreeDistanceMap.create(octree, 10);
	}
	
	/**
	 * Executes the update of the distance map. Consumes too much memory (about 4.7gb)
	 */
	@Test
	public void test01_Update() {
		distanceMap.update();
	}


}
