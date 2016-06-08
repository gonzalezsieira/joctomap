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
package es.usc.citius.lab.joctomap.util;

import es.usc.citius.lab.joctomap.iterators.OctreeIterator;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import es.usc.citius.lab.motionplanner.core.util.Pair;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * This class implements a builder for calculating the adjacency map of an Octomap.
 * Currenty ocotmap does not support querying for cells which are adjacent to others,
 * so it is needed to implement an external tool to calculate and query that. It is
 * important to recalculate the adjacency of the cells if the geometry of the map changes
 * (new cells added, or the size of the cells changes).
 *
 * @author Adrián González Sieira <<a href="mailto:adrian.gonzalez@usc.es">adrian.gonzalez@usc.es</a>>
 * @since 09/03/2016
 */
public class AdjacencyMap implements Serializable{

    private Map<JOctreeKey, List<JOctreeKey>> adjacencies;
    private JOctree octree;
    //contains the information of size and center of each leaf in the octree
    private Map<JOctreeKey, Pair<Float, Point3D>> nodesInfo;
    private double EPSILON = 1e-3;

    /**
     * Here the adjacency between the leafs of the octreee is calculated; two leafs are considered adjacent
     * if the distance between the paralell planes containing their centers is the sum of their resolutions;
     * here the adjacency map is filled using a java implementation.
     *
     * @param octree current octree instance
     */
    private AdjacencyMap(JOctree octree){
        this.adjacencies = new HashMap<JOctreeKey, List<JOctreeKey>>();
        this.octree = octree;
        this.nodesInfo = new HashMap<JOctreeKey, Pair<Float, Point3D>>();
        //create cache
        Cache cacheOfKeys = new Cache();

        //iterate over the cells of the octree twice, to generate the distances in pairs
        OctreeIterator it;
        for(it = octree.leafBBXIterator(octree.getMetricMin(), octree.getMetricMax(), 0); it.hasNext(); it.next()){
            Point3D coordinate1 = it.coordinate();
            float size1 = it.size();
            JOctreeKey key = cacheOfKeys.getInstance(it.key());
            nodesInfo.put(key, new Pair<Float, Point3D>(it.size(), it.coordinate()));
            adjacencies.put(key, new ArrayList<JOctreeKey>());
            float diff = octree.getResolution() + it.size() / 2f;
            OctreeIterator it2;
            for(it2 = octree.leafBBXIterator(new Point3D(coordinate1.getX() - diff, coordinate1.getY() - diff, coordinate1.getZ() - diff), new Point3D(coordinate1.getX() + diff, coordinate1.getY() + diff, coordinate1.getZ() + diff), 0); it2.hasNext(); it2.next()){
                //get coordinates and size
                Point3D coordinate2 = it2.coordinate();
                float sizeAdded = size1/2f + it2.size()/2f;
                JOctreeKey key2 = cacheOfKeys.getInstance(it2.key());
                //skip current keys
                //calculate adjacency using the AABB method (Axis Aligned Bounding Box)
                if (!key.equals(key2) &&
                        Math.abs(coordinate1.getX() - coordinate2.getX()) - sizeAdded <= EPSILON &&
                        Math.abs(coordinate1.getY() - coordinate2.getY()) - sizeAdded <= EPSILON &&
                        Math.abs(coordinate1.getZ() - coordinate2.getZ()) - sizeAdded <= EPSILON) {
                    //adjacent cells
                    adjacencies.get(key).add(key2);
                }
            }
            it2.dispose();
        }
        it.dispose();
    }
    
    /**
     * This initializes the adjacency map with the basic structures that are used to store information;
     * octree is initialized in JNI code.
     */
    private AdjacencyMap(){
        this.adjacencies = new HashMap<JOctreeKey, List<JOctreeKey>>();
        this.nodesInfo = new HashMap<JOctreeKey, Pair<Float, Point3D>>();
    }
    
    /**
     * Native method to obtain an instance of AdjacencyMap from an octree.
     * 
     * @param octree {@link JOctree} instance
     * @return instance of {@link AdjacencyMap}
     */
    public static AdjacencyMap create(JOctree octree){
        //initialize structures
        AdjacencyMap map = new AdjacencyMap();
        //assign octree
        map.octree = octree;
        //fill with JNI method
        map.initializeJNI();
        //retrieve result
        return map;
    }
    
    /**
     * Java method to obtain an instance of AdjacencyMap from an octree.
     * 
     * @param octree {@link JOctree} instance
     * @return instance of {@link AdjacencyMap}
     * @deprecated it is preferrable to use the native implementation provided in {@link #create(es.usc.citius.lab.joctomap.octree.JOctree) } rather than the java one
     */
    public static AdjacencyMap createJava(JOctree octree){
       return new AdjacencyMap(octree);
    }
    
    /**
     * This fills the structures of the adjacency map using full-native
     * implementation method for efficiency.
     */
    private native void initializeJNI();

    /**
     * Retrieves the adjacencies for a current key.
     *
     * @param key instance of {@link JOctreeKey}
     * @return
     */
    public List<JOctreeKey> adjacency(JOctreeKey key){
        return adjacencies.get(key);
    }

    public Map<JOctreeKey, List<JOctreeKey>> getAdjacencies(){
        return adjacencies;
    }

    public Map<JOctreeKey, Pair<Float, Point3D>> getNodesInfo() {
        return nodesInfo;
    }

    public JOctree getOctree() {
        return octree;
    }

    /**
     * Retrieves the information of a node stored after exploring the adjacencies.
     *
     * @param key instance of {@link JOctreeKey}
     * @return center of the node and
     */
    public Pair<Float, Point3D> nodeInfo(JOctreeKey key){
        return nodesInfo.get(key);
    }
    
    /**************************************************************************
     *                      I/O OPERATIONS
     **************************************************************************/
    
    /**
     * Saves into a file the current instance of AdjacencyMap.
     * 
     * @param filename path to output file
     * @return true if write operation was successful
     */
    public boolean write(String filename){
        //create new file
        File outputFile = new File(filename);
        //if file exists, delete
        if (outputFile.exists()){
            JOctomapLogger.warning(filename + " already exists. Content will be replaced.");
            outputFile.delete();
        }
        //open output stream
        try{
            ObjectOutputStream outputStream = new ObjectOutputStream(new FileOutputStream(outputFile));
            //write this instance
            outputStream.writeObject(this);
            outputStream.close();
        } catch (IOException ex) {
            //I/O error
            JOctomapLogger.severe("I/O error when writing adjacency map to " + filename);
            throw new RuntimeException(ex);
        }
        return true;
    }
    
    /**
     * Reads an instance of AdjacencyMap from file.
     * 
     * @param filename path to input file
     * @return stored instance of AdjacencyMap
     */
    public static AdjacencyMap read(String filename){
        //create new file
        File inputFile = new File(filename);
        //error if input file does not exist
        if(!inputFile.exists()){
            JOctomapLogger.severe("Could not open " + filename + ". File does not exist.");
            throw new RuntimeException("Specified file " + filename + " does not exist");
        }
        AdjacencyMap map;
        //open input stream (will be closed after this statement)
        try{
            ObjectInputStream inputStream = new ObjectInputStream(new FileInputStream(inputFile));
            //read map instance
            map = (AdjacencyMap) inputStream.readObject();
            inputStream.close();
        } catch (IOException ex) {
            //I/O error
            JOctomapLogger.severe("I/O error when reading adjacency map from " + filename);
            throw new RuntimeException(ex);
        } catch (ClassNotFoundException ex) {
            //error if object cannot be casted
            JOctomapLogger.severe("Cannot cast object to AdjacencyMap");
            throw new RuntimeException(ex);
        }
        return map;
    }
    
    /**
     * Routine to write an object of this class to an output stream.
     * 
     * @param out output stream
     * @throws IOException if some I/O error occurs
     */
    private void writeObject(ObjectOutputStream out) throws IOException{
        out.writeObject(this.adjacencies);
        out.writeObject(this.nodesInfo);
        out.writeObject(this.octree.getPath());
    }
    
    /**
     * Routine to read an object of this class from an input stream.
     * 
     * @param in input stream
     * @throws IOException if some I/O error occurs
     * @throws ClassNotFoundException if object cannot be casted to target class
     */
    private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException{
        this.adjacencies = (Map<JOctreeKey, List<JOctreeKey>>) in.readObject();
        this.nodesInfo = (Map<JOctreeKey, Pair<Float, Point3D>>) in.readObject();
        String path = (String) in.readObject();
        this.octree = JOctree.read(path);
    }
    
    /**************************************************************************
     *                      CACHE OF STORED KEYS
     **************************************************************************/
    

    /**
     * Cache of objects "Joctreekey" that will be used in this class. The cache will allow to obtain maps where
     * the objects stored will be less, minimizing the size of the generated maps and the garbage collection.
     */
    private class Cache {

        private HashMap<JOctreeKey, JOctreeKey> cache;

        public Cache(){
            this.cache = new HashMap<JOctreeKey, JOctreeKey>();
        }

        /**
         * Given a key object, retrieves the one stored in the cache to avoid storing instances
         * with the same content but different object reference. This is more memory-efficient than
         * storing all the created objects.
         *
         * @param key query object
         * @return cached object
         */
        public JOctreeKey getInstance(JOctreeKey key){
            //retrieve object from the cache
            JOctreeKey object = cache.get(key);
            //introduce value in the cache
            if(object == null){
                cache.put(key, key);
                object = key;
            }
            //return value
            return object;
        }

    }
}
