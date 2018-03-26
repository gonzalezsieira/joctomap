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
package es.usc.citius.lab.joctomap;

import es.usc.citius.lab.joctomap.iterators.OctreeIterator;
import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.octree.JOctreeNode;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import static org.junit.Assert.*;

/**
 * Unit test for octree objects. Ensures the dynamic library
 * to be loaded before the execution of the test cases.
 * 
 * The methods in this class are executed in an ascending ordered way to ensure
 * that the static parameters of the class (that store values used in 
 * later tests) are filled properly.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeTest{

	private static JOctree emptyOctree; //used in tests
	private static JOctree octree; //used in tests
	private static JOctreeKey key1; //used in tests
	private static JOctreeKey key2; //used in tests
	private static Float res; //used in tests
	private static Integer depth; //used in tests
	private static Point3D metricMin; //used in tests
	private static Point3D metricMax; //used in tests
	private static File fileRead;
	private static File fileWrite;

	private static int POINTS_TESTCHANGEDETECTION = 100;
	
	public JOctreeTest() throws URISyntaxException, IOException{
		fileRead = new File(getClass().getClassLoader().getResource("fr_campus.ot").toURI());
		fileWrite = new File("fr_campus".concat("_test").concat(".ot"))
				.getCanonicalFile();
	}
	
	@AfterClass
	public static void down() throws IOException{
		// delete the generated file in this test case
		fileWrite.delete();
	}
	
	/**
	 * Accessing {@link JOctree} element of this class after tests are executed.
	 * 
	 * @return readed {@link JOctree}
	 */
	public static JOctree getOctree(){
		return octree;
	}
	
	/**
	 * Instantiate new empty octree with 0.5 resolution
	 */
	@Test
	public void test01_createOctree(){
		emptyOctree = JOctree.create(0.5f);
	}
	
	/**
	 * Unit test for reading a .ot file.
	 * @throws IOException when an I/O error occurs 
	 */
	@Test
	public void test02_readFileOt() throws IOException {
		String inputPath = fileRead.getAbsolutePath();
		octree = JOctree.read(inputPath);
		assertTrue("Octree direction of memory not assigned",
				octree.getPointer() != 0);
	}

	/**
	 * Unit test for writing a .ot file.
	 * 
	 * @throws IOException
	 */
	@Test
	public void test03_writeFileOtTest() throws IOException {
		// write file to compare sizes
		octree.write(fileWrite.getAbsolutePath());
		// compare by content both files
		assertTrue("Input and output files with the same content",
                    FileUtils.contentEquals(fileRead, fileWrite));
	}
	
	/**
	 * Unit test for retrieving a {@link JOctreeKey} from a position.
	 */
	@Test
	public void test04_obtainCellKeyAtPositionTest() {
		key1 = octree.coordToKey(0f, 0f, 0f);
		assertTrue("JOctreeKey must not have negative values", key1.getX() >= 0 && key1.getY() >= 0 && key1.getZ() >= 0);
	}

	/**
	 * Unit test for retrieving a {@link JOctreeKey} from a position and depth
	 *
	 * @return key that identifies the node
	 */
	@Test
	public void test05_obtainCellKeyAtPositionWithDepthTest() {
		key2 = octree.coordToKey(0f, 0f, 0f, 1);
		assertTrue("JOctreeKey must not have negative values", key2.getX() >= 0 && key2.getY() >= 0 && key2.getZ() >= 0);
	}
	
	/**
	 * Unit test for testing the adjustement of {@link JOctreeKey} between different
	 * depth levels.
	 */
	@Test
	public void test06_adjustDepthOfKey(){
		JOctreeKey adjustement = octree.adjustKeyAtDepth(key1, 1);
		assertTrue("JOctreeKey must not have negative values", adjustement.getX() >= 0 && adjustement.getY() >= 0 && adjustement.getZ() >= 0);
		assertTrue("Adjusted key are not equal to the retrieved one at the same depth", key2.equals(adjustement));
		assertTrue("Adjusted key has a different hash to the retrieved one at the same depth", key2.hashCode() == adjustement.hashCode());	
	}
	
	/**
	 * Test case to recover the resolution of the octree.
	 */
	@Test
	public void test07_getResolutionTest(){
		res = octree.getResolution();
		double res2 = emptyOctree.getResolution();
		assertTrue("Resolution must be > 0", res > 0);
		assertTrue("Resolution different to the created with the empty octree", Double.compare(0.5, res2) == 0); 
	}
	
	/**
	 * Test case to recover the depth of the octree.
	 *
	 * @return octree maximum depth
	 */
	@Test
	public void test08_getTreeDepthTest(){
		depth = octree.getTreeDepth();
		assertTrue("Tree depth must be > 0", depth > 0);
	}
	
	/**
	 * Test case to recover the node size of the octree. 
	 * Checks if the size at the maximum depth is equal to the
	 * minimum resolution of the octree.
	 */
	@Test
	public void test09_getNodeSizeTest(){
		double size = octree.getNodeSize(depth);
		assertTrue("Node size must be > 0", size > 0);
		assertTrue("Minimum node size must be equal to the octree resolution", Double.compare(res, size) == 0);
	}
	
	/**
	 * Test case to search a node in the octree, given its {@link JOctreeKey} (search
	 * with depth = 0)
	 */
	@Test
	public void test10_searchWithKeyTest(){
		JOctreeNode node = octree.search(key1, 0);
		assertTrue("Native pointer of node is not assigned", node.getPointer() != 0);
	}
	
	/**
	 * Test case to search a node in the octree, given a 3D position (search
	 * with depth = 0)
	 */
	@Test
	public void test11_searchWith3DPositionTest(){
		JOctreeNode node = octree.search(0f, 0f, 0f, 0);
		assertTrue("Native pointer of node is not assigned", node.getPointer() != 0);
	}
	
	/**
	 * Tests if the node retrieved is the same in the update step and
	 * the search step.
	 */
	@Test
	public void test12_updateNodeTest(){
		JOctreeNode node1 = emptyOctree.updateNode(0, 0, 0, true);
		JOctreeNode node2 = emptyOctree.search(0, 0, 0, 0);
		assertEquals("Updated and retrieved node are not equals ", node1.getPointer(), node2.getPointer());
		assertTrue("Updated node must be occupancy probability > 0", node1.getOccupancy() > 0);
	}
	
	/**
	 * Inserts an "occupied node" update several times, checking
	 * that the probability of occupation must increase with each update.
	 */
	@Test
	public void test13_recursiveUpdateOccupiedNodeTest(){
		JOctreeNode node = emptyOctree.search(0, 0, 0, 0);
		//continue updating the node
		for(int i = 0; i < 10; i++){
			double occ = node.getOccupancy();
			node = emptyOctree.updateNode(0, 0, 0, true);
			double occNew = node.getOccupancy();
			assertTrue("Occupancy must increase with each update", occNew >= occ);
		}
	}
	
	/**
	 * Queries if the node is occupied.
	 */
	@Test
	public void test14_isNodeOccupiedTest(){
		JOctreeNode node = emptyOctree.search(0, 0, 0, 0);
		assertTrue("Occupied node does not retrieve occupied", emptyOctree.isNodeOccupied(node));
	}
	
	/**
	 * Inserts a "free node" update several times, checking that 
	 * the probability of occupation must decrease with each update.
	 */
	@Test
	public void test15_recursiveUpdateFreeNodeTest(){
		JOctreeNode node = emptyOctree.search(0, 0, 0, 0);
		//continue updating the node
		for(int i = 0; i < 10; i++){
			double occ = node.getOccupancy();
			node = emptyOctree.updateNode(0, 0, 0, false);
			double occNew = node.getOccupancy();
			assertTrue("Occupancy must decrease with each update", occNew <= occ);
		}
	}
	
	/**
	 * Queries if the node is not occupied.
	 */
	@Test
	public void test16_isNodeOccupiedTest(){
		JOctreeNode node = emptyOctree.search(0, 0, 0, 0);
		assertTrue("Occupied node does not retrieve occupied", !emptyOctree.isNodeOccupied(node));
	}
	
	/**
	 * Queries the position of a {@link JOctreeKey}.
	 */
	@Test
	public void test17_keyToCoord(){
		Point3D coord = octree.keyToCoord(key1);
		double res = octree.getResolution();
		assertTrue("Coord out of bounds", Math.abs(coord.getX()) < res && Math.abs(coord.getY()) < res && Math.abs(coord.getZ()) < res);
	}
	
	/**
	 * Queries the position of a {@link JOctreeKey} at a depth.
	 */
	@Test
	public void test18_keyToCoordAtDepth(){
		Point3D original = new Point3D(100f, -50f, 0.05f);
		JOctreeKey key = octree.coordToKey(original);
		for(int i = 1; i <= depth; i++){
			Point3D coord = octree.keyToCoord(key, i);
			double size = octree.getNodeSize(i);
			assertTrue("Coord out of bounds", Math.abs(coord.getX() - original.getX()) < size && Math.abs(coord.getY() - original.getY()) < size && Math.abs(coord.getZ() - original.getZ()) < size);
		}
	}
	
	/**
	 * Tests if the min and max metric coordinates are equals to the metric size
	 */
	@Test
	public void test19_metricCoordinates(){
		metricMin = octree.getMetricMin();
		metricMax = octree.getMetricMax();
		float[] metricSize = octree.getMetricSize();
		assertTrue("Metric size array must be 3", metricSize.length == 3);
		assertTrue("X size does not match min.x - max.x", Float.compare(Math.abs(metricMin.getX() - metricMax.getX()), metricSize[0]) == 0);
		assertTrue("Y size does not match min.y - max.y", Float.compare(Math.abs(metricMin.getY() - metricMax.getY()), metricSize[1]) == 0);
		assertTrue("Z size does not match min.z - max.z", Float.compare(Math.abs(metricMin.getZ() - metricMax.getZ()), metricSize[2]) == 0);
	}
	
	/**
	 * Tests the return value of the BBX retrieval methods.
	 */
	@Test
	public void test20_applyBBX(){
		//checks behavior of the bbx set method
		assertTrue("bbx is disabled by default", !octree.isBBXApplied());
		//change value
		octree.useBBXLimit(true);
		//checks if the value is changed
		assertTrue("bbx changed to enabled, but returns disabled", octree.isBBXApplied());
		//restore the value to its original
		octree.useBBXLimit(false);
	}
	
	/**
	 * Tests the values of the bbx retrieval methods
	 */
	@Test
	public void test21_coordinatesBBX(){
		//check default values
		assertTrue("BBX min must be (0, 0, 0) by default", octree.getBBXMin().equals(new Point3D(0, 0, 0)));
		assertTrue("BBX max must be (0, 0, 0) by default", octree.getBBXMax().equals(new Point3D(0, 0, 0)));
		assertTrue("BBX center must be (0, 0, 0) by default", octree.getBBXCenter().equals(new Point3D(0, 0, 0)));
		assertTrue("BBX is not set by default", !octree.isBBXSet());
		//set a bbx
		Point3D min = new Point3D(-1, -2, -3);
		Point3D max = new Point3D(1, 2, 3);
		octree.setBBX(min, max);
		//check new bbx
		assertTrue("BBX min updated to " + min + " but returned " + octree.getBBXMin(), octree.getBBXMin().equals(min));
		assertTrue("BBX max updated to " + max + " but returned " + octree.getBBXMax(), octree.getBBXMax().equals(max));
		assertTrue("BBX center not updated properly", octree.getBBXCenter().equals(new Point3D((max.getX() - min.getX())/2 + min.getX(), (max.getY() - min.getY())/2 + min.getY(), (max.getZ() - min.getZ())/2 + min.getZ())));
		assertTrue("BBX must be set after specifying min or max point", octree.isBBXSet());
	}
	
	/**
	 * Tests the values retrieved by the metric coordinates and size.
	 */
	@Test
	public void test22_metricCoordinates(){
		//retrieve metric coordinates
		Point3D min = octree.getMetricMin();
		Point3D max = octree.getMetricMax();
		float[] size = octree.getMetricSize();
		assertTrue("size X does not match with min and max metric points", Float.compare(size[0], Math.abs(max.getX() - min.getX())) == 0);
		assertTrue("size Y does not match with min and max metric points", Float.compare(size[1], Math.abs(max.getY() - min.getY())) == 0);
		assertTrue("size Z does not match with min and max metric points", Float.compare(size[2], Math.abs(max.getZ() - min.getZ())) == 0);
	}
	
	/**
	 * Manipulates the bounds of the BBX to test the queries of that elements.
	 */
	@Test
	public void test23_manipulateBBX(){
		//manipulate max bound
		Point3D currentMax = octree.getBBXMax();
		Point3D currentMin = octree.getBBXMin();
		//manipulate max
		octree.setBBXMax(currentMin);
		assertTrue("BBX max == BBX min but method appears to be set", !octree.isBBXSet());
		assertTrue("BBX max == BBX min but methods to retrieve them offer bad information", octree.getBBXMax().equals(octree.getBBXMin()));
		//manipulate min
		octree.setBBXMin(new Point3D(0, 0, 0));
		octree.setBBXMax(currentMax);
		assertTrue("BBX max != BBX min but method appears to be not set", octree.isBBXSet());
		assertTrue("BBX max offer bad information", octree.getBBXMax().equals(currentMax));
		assertTrue("BBX min offer bad information", octree.getBBXMin().equals(new Point3D(0, 0, 0)));
	}
        
        /**
         * Prunes the elements of the octree.
         */
        @Test
        public void test24_prune(){
            octree.prune();
        }
        
        /**
         * Expands the nodes of the ocree to the minimum resolution.
         */
        @Test
        public void test25_expand(){
            octree.expand();
        }
        
        /**
         * Updates the inner occupancy of the nodes of the occtree (to match the children's ones).
         */
        @Test
        public void test26_innerOcupancy(){
            octree.updateInnerOccupancy();
        }
        
        @Test 
        public void test27_size(){
            int sizeNative = octree.size();
            int sizeJava = 0;
            for(OctreeIterator it = octree.leafBBXIterator(octree.getMetricMin(), octree.getMetricMax(), 0); it.hasNext(); it.next()){
                sizeJava++;
            }
            assertEquals("Native and java methods to retrieve the size do not match", sizeJava, sizeNative);
        }

        @Test
		public void test28_changeDetection(){
        	Point3D min = octree.getMetricMin();
        	Point3D max = octree.getMetricMax();
			Point3D endChanges = new Point3D(min.x + (max.x - min.x) / 10f, min.y + (max.y - min.y) / 10f, min.z + (max.z - min.z) / 10f);

        	//enable change control
			octree.enableChangeDetection(true);

			//create list of positions to change
			List<Point3D> listPositionsToChange = new ArrayList<Point3D>();
			List<JOctreeKey> listKeysToChange = new ArrayList<JOctreeKey>();

			Random random = new Random(System.nanoTime());
			for(int i = 0; i < POINTS_TESTCHANGEDETECTION; i++){
				float x = random.nextFloat() * (max.x - min.x) + min.x;
				float y = random.nextFloat() * (max.y - min.y) + min.y;
				float z = random.nextFloat() * (max.z - min.z) + min.z;
				listPositionsToChange.add(new Point3D(x, y, z));
			}

			//introduce changes in the map
			for (Point3D current : listPositionsToChange){
				octree.updateNode(current.x, current.y, current.z, true);
			}

			//get list of changes
			List<JOctreeKey> keysChanged = octree.keysChanged();

			//check not empty
			assertTrue("List of changed keys cannot be empty",keysChanged.size() > 0);

			//all changes must belong to the first quarter of the map (which was the changed region)

			for(JOctreeKey key : keysChanged){
				Point3D coordinate = octree.keyToCoord(key);
				boolean isInChanges = false;
				for(Point3D currentItemInList : listPositionsToChange){
					if (currentItemInList.distance(coordinate) < octree.getResolution()){
						isInChanges = true;
						break;
					}
				}
				assertTrue("Changed coordinate must be in the list of changes introduced", isInChanges);
			}

			//reset changes detection
			octree.resetChangeDetection();

			//check empty
			assertTrue("List of changed keys must be empty after reset", octree.keysChanged().isEmpty());

			//disable changes detection
			octree.enableChangeDetection(false);

			//re-introduce changes in the octree (first quarter of the map)
			for(Point3D current : listPositionsToChange){
				octree.updateNode(current.x, current.y, current.z, false);
			}

			//check empty
			assertTrue("List of changed keys must be empty when disabled", octree.keysChanged().isEmpty());
		}

		@Test
		public void test29_getTreeType(){
        	String type = JOctree.create(0.1f).getTreeType();
        	assertTrue(!type.isEmpty());
		}
}
