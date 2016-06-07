package es.usc.citius.lab.joctomap.util;

import com.google.common.base.Stopwatch;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import es.usc.citius.lab.motionplanner.core.util.Pair;
import java.io.File;
import java.net.URISyntaxException;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author adrian.gonzalez
 */
public class AdjacencyMapTest {
    
    private final JOctree octree;
    private final AdjacencyMap adjacencyMap;
    private final File resourceOutput;
    
    public AdjacencyMapTest() throws URISyntaxException{
        String resourceInput = new File(getClass().getClassLoader().getResource("ecmrv4_withfree.ot").toURI()).getAbsolutePath();
        //retrieve octree instance
        octree = JOctree.read(resourceInput);
        //calculate adjacency map
        Stopwatch timer = Stopwatch.createStarted();
        adjacencyMap = AdjacencyMap.createJava(octree);
        timer = timer.stop();
        System.out.println("Created AdjacencyMap (java version): " + timer.elapsed(TimeUnit.SECONDS) + " s");
        resourceOutput = new File("tempFile.tmp");
    }
    
    @Test
    public void test01_calculateAdjacencyMap(){
        
        assertEquals("Nodes info size do not match with number of leaves.", adjacencyMap.getOctree().size(), adjacencyMap.getNodesInfo().size());
        assertEquals("Adjacencies info size do not match with number of leaves.", adjacencyMap.getOctree().size(), adjacencyMap.getAdjacencies().size());
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
                assertTrue("Distance between centers is greater than sum of half size of cells", sizeCell1 / 2f + sizeCell2 / 2f <= distanceMaxAxis);
            }
        }
    }
    
    @Test
    public void test02_saveAndLoad(){
        //write map to the output path
        adjacencyMap.write(resourceOutput.getAbsolutePath());
        //load a new instance of the saved map
        Stopwatch timer = Stopwatch.createStarted();
        AdjacencyMap loadedMap = AdjacencyMap.read(resourceOutput.getAbsolutePath());
        System.out.println("Created AdjacencyMap (loaded from file): " + timer.elapsed(TimeUnit.SECONDS) + " s");
        //remove file
        resourceOutput.delete();
        compareMaps(loadedMap);
    }
    
    public void test03_nativeImplementation(){
        Stopwatch timer = Stopwatch.createStarted();
        AdjacencyMap adjacencyMapJNI = AdjacencyMap.create(octree);
        System.out.println("Created AdjacencyMap (JNI version): " + timer.elapsed(TimeUnit.SECONDS) + " s");
        compareMaps(adjacencyMapJNI);
    }
    
    
    private void compareMaps(AdjacencyMap otherMap){
        //compare maps
        assertEquals("Number of adjacencies does not match.", adjacencyMap.getAdjacencies().size(), otherMap.getAdjacencies().size());
        assertEquals("Number of information nodes do not match.", adjacencyMap.getNodesInfo().size(), otherMap.getNodesInfo().size());
        //compare content element-by-element
        //compare nodes info
        for(Map.Entry<JOctreeKey, Pair<Float, Point3D>> current : adjacencyMap.getNodesInfo().entrySet()){
            assertTrue("Key not present in nodes info of the loaded map", otherMap.getNodesInfo().containsKey(current.getKey()));
            Pair<Float, Point3D> loadedInfo = otherMap.getNodesInfo().get(current.getKey());
            assertEquals("Size of node do not match", current.getValue().getKey(), loadedInfo.getKey());
            assertEquals("Center of cell do not match", current.getValue().getContent(), loadedInfo.getContent());
        }
        //compare adjacencies
        for(Map.Entry<JOctreeKey, List<JOctreeKey>> current : adjacencyMap.getAdjacencies().entrySet()){
            assertTrue("Key not present in adjacencies of the loaded map", otherMap.getAdjacencies().containsKey(current.getKey()));
            assertEquals("Size of adjacencies array do not match.", current.getValue().size(), otherMap.getAdjacencies().get(current.getKey()).size());
            //compare the adjacent keys
            List<JOctreeKey> loadedAdjacencies = otherMap.getAdjacencies().get(current.getKey());
            for(int i = 0; i < current.getValue().size(); i++){
                assertEquals("Adjacent key do not match", current.getValue().get(i), loadedAdjacencies.get(i));
            }
        }
    }
}
