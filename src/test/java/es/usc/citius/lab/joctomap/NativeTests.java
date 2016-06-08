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
package es.usc.citius.lab.joctomap;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

import es.usc.citius.lab.joctomap.iterators.LeafBBXIteratorTest;
import es.usc.citius.lab.joctomap.util.JOctreeUtilsTest;

/**
 * Order the execution of all native tests in the library.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@RunWith(Suite.class)
@SuiteClasses({ 
	JOctreeTest.class, 
	JOctreeNodeTest.class,
	LeafBBXIteratorTest.class,
        JOctreeUtilsTest.class
	//disabled due to high memory consumption
	//JOctreeDistanceMapTest.class 
})
public class NativeTests {
	
}
