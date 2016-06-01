package es.usc.citius.lab.joctomap.util;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import java.io.File;
import java.net.URISyntaxException;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author adrian.gonzalez
 */
public class AdjacencyMapTest {
    
    public String resourceInput;
    
    public AdjacencyMapTest() throws URISyntaxException{
        resourceInput = new File(getClass().getClassLoader().getResource("ecmrv4_withfree.ot").toURI()).getAbsolutePath();
    }
    
    @Test
    public void test01_calculateAdjacencyMap(){
        //retrieve octree instance
        JOctree octree = JOctree.read(resourceInput);
        //calculate adjacency map
        AdjacencyMap adjacencyMap = new AdjacencyMap(octree);
        //iterate over the entries of the adjacency map
        for(Map.Entry<JOctreeKey, List<JOctreeKey>> entry : adjacencyMap.getAdjacencies().entrySet()){
            //center of first cell
            Point3D centerCell1 = adjacencyMap.nodeInfo(entry.getKey()).getContent();
            float sizeCell1 = adjacencyMap.nodeInfo(entry.getKey()).getKey();
            //iterate over the adjacencies of the current cell
            for(JOctreeKey current : entry.getValue()){
                Point3D centerCell2 = adjacencyMap.nodeInfo(current).getContent();
                float sizeCell2 = adjacencyMap.nodeInfo(current).getKey();
                float distanceMaxAxis = Collections.max(
                        Arrays.asList(
                                Math.abs(centerCell1.getX() - centerCell2.getX()),
                                Math.abs(centerCell1.getY() - centerCell2.getY()),
                                Math.abs(centerCell1.getZ() - centerCell2.getZ())
                        )
                );
                assertTrue("Distance between centers is greater than sum of half size of cells", sizeCell1 / 2f + sizeCell2 / 2f < distanceMaxAxis);
            }
        }
    }
    
}
