package es.usc.citius.lab.joctomap;

import static org.junit.Assert.*;

import java.util.List;

import org.junit.BeforeClass;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

/**
 * Test case over the methods in {@link JOctreeNode}. The tests are executing in
 * ascending order to guarantee dependencies between them, if exist.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class JOctreeNodeTest {

	private static JOctreeNode octreeNode; //used in tests
	private static boolean hasChildren; //used in tests
	private static double occupancy; //used in tests
	private static int numChildren; //used in tests
	private static List<JOctreeNode> children; //used in tests
	
	
	
	/**
	 * Initializes 
	 */
	@BeforeClass
	public static void initialize(){
		//store node of the readed octree
		octreeNode = JOctreeTest.getOctree().search(0d, 0d, 0d, 0);
	}
	
	/**
	 * Test if the occupancy of the node is between 0 and 1
	 */
	@Test
	public void test01_getOccupancyTest() {
		occupancy = octreeNode.getOccupancy();
		assertTrue("Occupancy be between 0 and 1 > 1", occupancy >=0 && occupancy <= 1);
	}
	
	/**
	 * Test if the current node has children.
	 */
	@Test
	public void test02_hasChildrenTest(){
		hasChildren = octreeNode.hasChildren();
	}
	
	/**
	 * Test the query of number of children.
	 */
	@Test
	public void test03_numChildrenTest(){
		numChildren = octreeNode.getNumChildren();
		assertTrue("hasChildren = " + hasChildren + " but numchildren = " + numChildren, (!hasChildren && numChildren == 0) || (hasChildren && numChildren > 0));
	}
	
	/**
	 * Test the method to retrieve the childrens of a node.
	 */
	@Test
	public void test04_getChildrenTest(){
		children = octreeNode.getChildren();
		assertTrue("numchildren and children array size differ", children.size() == numChildren);
		assertTrue("haschildren and numchildren with different information", (hasChildren && numChildren > 0) || (!hasChildren && numChildren == 0));
	}
	
	/**
	 * Test if the children exist.
	 */
	@Test
	public void test05_childExistsTest(){
		for(int i = 0; i < numChildren; i++){
			boolean exists = octreeNode.childExists(i);
			assertTrue("child " + i + " does not exist but numchildren=" + numChildren, exists);
		}
	}
	
	/**
	 * Test if the current node is collapsible (not has children, or 
	 * has children and all of them exist, and they have all the same occupancy information).
	 */
	@Test
	public void test06_collapsibleTest(){
		//retrieve collapsibility
		boolean collapsible = octreeNode.collapsible();
		//check equality of the nodes
		boolean equals = true;
		//check occupancy of the children
		for(JOctreeNode child : children){
			if(Double.compare(child.getOccupancy(), occupancy) != 0){
				equals = false;
				break;
			}
		}
		//assertion conditions
		if(!collapsible){
			assertTrue("Not collapsibility conditions are not matched", !hasChildren || (hasChildren && (numChildren < 8 || (numChildren == 8 && !equals))));
		}
		else{
			assertTrue("Collapsibility conditions are not matched", hasChildren && numChildren == 8 && equals);
		}
	}
	


}
