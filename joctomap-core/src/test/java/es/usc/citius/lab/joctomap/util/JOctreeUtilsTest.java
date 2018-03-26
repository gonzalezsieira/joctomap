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

import es.usc.citius.lab.joctomap.octree.Cell;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import java.util.Collection;
import java.util.List;

import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author adrian
 */
public class JOctreeUtilsTest{
    
    private JOctree octree;
    
    public JOctreeUtilsTest() {
        
    }
    
    @Before
    public void setUp() {
        //create octree
        octree = JOctree.create(0.2f);
        //mark nodes as occupied
        octree.updateNode(5, 5, 0, true);
        octree.updateNode(10, 10, 0, true);
    }

    @Test
    public void test01_Keys() {
        Collection<JOctreeKey> keysOccupied = JOctreeUtils.getOccupiedKeysBetween(octree, Point3D.ZERO, new Point3D(20, 20, 0));
        assertTrue("list of occupied cells must be size 2", keysOccupied.size() == 2);
        assertTrue("position (5,5,0) not in the list of occupied", keysOccupied.contains(octree.coordToKey(5, 5, 0)));
        assertTrue("position (10,10,0) not in the list of occupied", keysOccupied.contains(octree.coordToKey(10, 10, 0)));
    }
    
    @Test
    public void test02_Coordinates() {
        Collection<Cell> coordinatesOccupied = JOctreeUtils.getOccupiedCoordinatesBetween(octree, Point3D.ZERO, new Point3D(20, 20, 0));
        assertTrue("list of occupied cells must be size 2", coordinatesOccupied.size() == 2);
        //assertTrue("position (5,5,0) not in the list of occupied", coordinatesOccupied.contains(octree.coordToKey(5, 5, 0)));
        //assertTrue("position (10,10,0) not in the list of occupied", coordinatesOccupied.contains(octree.coordToKey(10, 10, 0)));
    }
}
