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
#include <iostream>
#include <jni.h>
#include <octomap/OcTree.h>
#include <octomap/OcTreeKey.h>
#include <octomap/OccupancyOcTreeBase.h>
#include <octomap/OcTreeNode.h>
#include <float.h>
#include <queue>
#include <unordered_set>
#include "definitions.h"
#include "collisions.h"
#include "nativeobject.h"
#include "joctreeutils.h"

using namespace std;
using namespace octomap;

int POINTS_CONSIDERED = 2;

/*
 *
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_getOccupiedKeysBetween
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpointmin, jobject jpointmax){
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    
    //get values of the Point3D objects
    float pointminX = env->GetFloatField(jpointmin, fieldX);
    float pointminY = env->GetFloatField(jpointmin, fieldY);
    float pointminZ = env->GetFloatField(jpointmin, fieldZ);
    float pointmaxX = env->GetFloatField(jpointmax, fieldX);
    float pointmaxY = env->GetFloatField(jpointmax, fieldY);
    float pointmaxZ = env->GetFloatField(jpointmax, fieldZ);
    
    //define min/max points to iterate
    point3d min = point3d(pointminX, pointminY, pointminZ);
    point3d max = point3d(pointmaxX, pointmaxY, pointmaxZ);
    
    //find class and constructor to instantiate JOCtreekey
    jclass clsKey = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
    //find ArrayList class
    jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
    //retrieve JOctreeKey constructor
    jmethodID constructorKey = env->GetMethodID(clsKey, "<init>", "(III)V");
    //retrieve ArrayList constructor
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    //retrieve ArrayList add method
    jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    	
    //new arraylist()
    jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
    //get octree object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    //iterate and store objects in a java arraylist
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(min, max, 0), end = octree->end_leafs_bbx(); it != end; ++it){
        //transform current leaf_bbx_iterator to node
        OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
        //check colliding obstacles only
        if(octree->isNodeOccupied(currentNode)){
            OcTreeKey key = it.getKey();
            jobject currentKey = env->NewObject(clsKey, constructorKey, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
            //add object to the ArrayList
            env->CallBooleanMethod(arrayListObject, addArrayList, currentKey);
        }
    }
    return arrayListObject;
}

/*
 *
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_getOccupiedCoordinatesBetween
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpointmin, jobject jpointmax){
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    
    //get values of the Point3D objects
    float pointminX = env->GetFloatField(jpointmin, fieldX);
    float pointminY = env->GetFloatField(jpointmin, fieldY);
    float pointminZ = env->GetFloatField(jpointmin, fieldZ);
    float pointmaxX = env->GetFloatField(jpointmax, fieldX);
    float pointmaxY = env->GetFloatField(jpointmax, fieldY);
    float pointmaxZ = env->GetFloatField(jpointmax, fieldZ);
    
    //define min/max points to iterate
    point3d min = point3d(pointminX, pointminY, pointminZ);
    point3d max = point3d(pointmaxX, pointmaxY, pointmaxZ);

    //find Pair class
    jclass clsPair = env->FindClass("es/usc/citius/lab/motionplanner/core/util/Pair");
    //find ArrayList class
    jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
    //find Float class
    jclass floatClass = env->FindClass("Ljava/lang/Float;");
    //retrieve Point3D constructor
    jmethodID constructorPoint3D = env->GetMethodID(clsPoint3D, "<init>", "(FFF)V");
    //retrieve Float constructor
    jmethodID floatConstructor = env->GetMethodID(floatClass, "<init>", "(F)V");
    //retrieve Pair constructor
    jmethodID constructorPair = env->GetMethodID(clsPair, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    //retrieve ArrayList constructor
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    //retrieve ArrayList add method
    jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    	
    //new arraylist()
    jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
    //get octree object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    //iterate and store objects in a java arraylist
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(min, max, 0), end = octree->end_leafs_bbx(); it != end; ++it){
        //transform current leaf_bbx_iterator to node
        OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
        //check colliding obstacles only
        if(octree->isNodeOccupied(currentNode)){
            point3d coordinate = it.getCoordinate();
            jobject currentPoint3D = env->NewObject(clsPoint3D, constructorPoint3D, static_cast<float>(coordinate.x()), static_cast<float>(coordinate.y()), static_cast<float>(coordinate.z()));
            jobject currentFloat = env->NewObject(floatClass, floatConstructor, static_cast<float>(it.getSize()));
            jobject currentPair = env->NewObject(clsPair, constructorPair, currentPoint3D, currentFloat);
            //add object to the ArrayList
            env->CallBooleanMethod(arrayListObject, addArrayList, currentPair);
        }
    }
    return arrayListObject;
}


/*
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    resolutionAt
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_resolutionAt
  (JNIEnv *env, jclass cls, jobject jtree, jobject jpoint){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    //get values of the Point3D objects
    float pointX = env->GetFloatField(jpoint, fieldX);
    float pointY = env->GetFloatField(jpoint, fieldY);
    float pointZ = env->GetFloatField(jpoint, fieldZ);
    //query resolution of the octree
    OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(point3d(pointX, pointY, pointZ), point3d(pointX, pointY, pointZ), 0);
    float resolution = FLT_MAX;
    if(it != octree->end_leafs_bbx()){
        resolution = static_cast<float>(it.getSize());
    }
    return resolution;
}

/*
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    resolutionAddedIn
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_resolutionAddedIn
  (JNIEnv *env, jclass cls, jobject jtree, jobject jpoint1, jobject jpoint2){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    //get values of the Point3D objects
    float point1X = env->GetFloatField(jpoint1, fieldX);
    float point1Y = env->GetFloatField(jpoint1, fieldY);
    float point1Z = env->GetFloatField(jpoint1, fieldZ);
    float point2X = env->GetFloatField(jpoint2, fieldX);
    float point2Y = env->GetFloatField(jpoint2, fieldY);
    float point2Z = env->GetFloatField(jpoint2, fieldZ);
    //query first resolution of the octree
    OcTree::leaf_bbx_iterator it1 = octree->begin_leafs_bbx(point3d(point1X, point1Y, point1Z), point3d(point1X, point1Y, point1Z), 0);
    float resolution1 = FLT_MAX;
    if(it1 != octree->end_leafs_bbx()){
        resolution1 = static_cast<float>(it1.getSize());
    }
    //query second resolution of the octree
    OcTree::leaf_bbx_iterator it2 = octree->begin_leafs_bbx(point3d(point2X, point2Y, point2Z), point3d(point2X, point2Y, point2Z), 0);
    float resolution2 = FLT_MAX;
    if(it2 != octree->end_leafs_bbx()){
        resolution2 = static_cast<float>(it2.getSize());
    }
    return resolution1 + resolution2;
}

/**
 * Custom defined type for Point2D.
 */
class Point2D
{

	double coord_x, coord_y;

	public:
		Point2D(){
			coord_x = 0;
			coord_y = 0;
		}

		Point2D(point3d point_3d){
			this->coord_x = point_3d.x();
			this->coord_y = point_3d.y();
		}

		Point2D(double x, double y){
			this->coord_x = x; this->coord_y = y;
		}

		~Point2D() { }

		double x() const { return this->coord_x; }

		double y() const { return this->coord_y; }

		bool operator== (const Point2D &other) const
		{
			return (this->coord_x == other.coord_x)
					&& (this->coord_y == other.coord_y);
		}

};

/**
 * Hash function for Point2D.
 */
struct Point2D_Hash
{

	std::size_t operator()(const Point2D &point) const
	{
		//already defined hash function for doubles
		return std::hash<double>()(point.x()) + std::hash<double>()(point.y());
	}

};

Point2D jpoint2d_to_Point2D(jobject jpoint2d, JNIEnv* env, jfieldID field_x, jfieldID field_y){
    //retrieve argument-passed object field values
    jfloat point2d_x = env->GetFloatField(jpoint2d, field_x);
    jfloat point2d_y = env->GetFloatField(jpoint2d, field_y);
    //return value
    return Point2D(point2d_x, point2d_y);
}

struct NodeInfo {
    
    jobject jkey;
    OcTreeKey key;
    double size;
    Point2D coordinate;
    
    NodeInfo(OcTreeKey key, double size, jobject jkey, Point2D coordinate) { this->key = key; this->size = size; this->jkey = jkey; this->coordinate = coordinate; }
    
};

struct NodeInfo_Adjacencies : NodeInfo{
    
    jobject jadjacencies;
    
    NodeInfo_Adjacencies(OcTreeKey key, double size, jobject jkey, Point2D coordinate, jobject jadjacencies) : NodeInfo (key, size, jkey, coordinate) { this->jadjacencies = jadjacencies; }
    
};

/* 
 *  Comparison returns true when p1 > p2, doing this the priority queue
 *  will return the lowest element at first
 */
struct ComparePoint3D {
    
    //comparator argument
    point3d reference;
    
    //constructor
    ComparePoint3D(point3d reference) { this->reference = reference; }
    
    //comparison function
    bool operator()(point3d const & p1, point3d const & p2){
        return p1.distance(reference) > p2.distance(reference);
    }
};

//define priority queue
typedef std::priority_queue<point3d, std::vector<point3d>, ComparePoint3D> priorityqueue;

void frontier_points(double octree_resolution, float size_cell, Point2D center, priorityqueue &queue){
    if(size_cell > octree_resolution){
        float diff = size_cell / 4.0;
        queue.push(point3d(center.x() - diff, center.y() - diff, 0));
        queue.push(point3d(center.x() - diff, center.y() + diff, 0));
        queue.push(point3d(center.x() + diff, center.y() + diff, 0));
        queue.push(point3d(center.x() + diff, center.y() - diff, 0));
    }
}

NodeInfo node_info(OcTreeKey key, jobject jkey, JNIEnv* env, jmethodID method_nodesinfo,
		jfieldID field_float_value, jfieldID field_first, jfieldID field_second,
		jfieldID field_jpoin2d_x, jfieldID field_jpoin2d_y, jobject jadjacencymap){
    //get node info from java object
    jobject jnode_info = env->CallObjectMethod(jadjacencymap, method_nodesinfo, jkey);
    //get key value
    jobject size_float = env->GetObjectField(jnode_info, field_first);
    //get Float value
    jfloat size = env->GetFloatField(size_float, field_float_value);
    //get coordinate value
    jobject jpoint2d = env->GetObjectField(jnode_info, field_second);
    //transform point3d from java to C++
    Point2D point2d = jpoint2d_to_Point2D(jpoint2d, env, field_jpoin2d_x, field_jpoin2d_y);
    //convert float to 
    return NodeInfo(key, size, jkey, point2d);
    
}

NodeInfo_Adjacencies search_node(OcTree* octree, point3d point, JNIEnv* env, jobject jadjacencymap, jmethodID method_adjacency, jclass cls_joctreekey, jmethodID method_constructor_joctreekey){
    //get max depth
    int depth = octree->getTreeDepth() + 1;
    OcTreeKey key;
    jobject jkey, jadjacencies;
    //this loop breaks when a key in the adjacency map is found (means that it exists in the tree)
    do{
        //find 
        depth--;
        key = octree->coordToKey(point, depth);
        //instantiate key in java
        jkey = env->NewObject(cls_joctreekey, method_constructor_joctreekey, key.k[0], key.k[1], key.k[2]);
        jadjacencies = env->CallObjectMethod(jadjacencymap, method_adjacency, jkey);
    } while(depth > 0 && jadjacencies == NULL);
    return NodeInfo_Adjacencies( key, octree->getNodeSize(depth), jkey, Point2D(octree->keyToCoord(key)), jadjacencies );
    
}

JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_availableH2DMRTransitions
  (JNIEnv *env, jclass cls_joctree_utils, jobject joctree, jobject jadjacencymap, jobject jpoint2d, jfloat radius){
    //retrieve native objects
    OcTree* octree = (OcTree*) getPointer(env, joctree);
    //retrieve argument-passed types
    jclass cls_adjacencymap = env->FindClass(CLS_JADJACENCYMAP);
    jclass cls_point2d = env->FindClass(CLS_POINT2D);
    jclass cls_float = env->FindClass(CLS_FLOAT);
    //retrieve classes used in this method
    jclass cls_joctreekey = env->FindClass(CLS_JOCTREEKEY);
    jclass cls_arraylist = env->FindClass(CLS_ARRAYLIST);
    //jclass cls_transition = env->FindClass(CLS_TRANSITION);
    jclass cls_pair = env->FindClass(CLS_PAIR);
    //retrieve argument-passed object methods
    jmethodID method_adjacency = env->GetMethodID(cls_adjacencymap, "adjacency", "(Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Ljava/util/List;");
    jmethodID method_nodeinfo = env->GetMethodID(cls_adjacencymap, "nodeInfo", "(Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Les/usc/citius/lab/motionplanner/core/util/Pair;");
    //retrieve constructors of classes used in this method
    jmethodID method_constructor_arraylist = env->GetMethodID(cls_arraylist, METHOD_CONSTRUCTOR, "()V");
    jmethodID method_constructor_joctreekey = env->GetMethodID(cls_joctreekey, METHOD_CONSTRUCTOR, "(III)V");
    //jmethodID method_constructor_transition = env->GetMethodID(cls_joctreekey, METHOD_CONSTRUCTOR, "(Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)V");
    jmethodID method_constructor_point2d = env->GetMethodID(cls_point2d, METHOD_CONSTRUCTOR, "(FF)V");
    //retrieve fields used by classes in this method
    jfieldID field_joctreekey_x = env->GetFieldID(cls_joctreekey, FIELD_X, SIGNATURE_INT);
    jfieldID field_joctreekey_y = env->GetFieldID(cls_joctreekey, FIELD_Y, SIGNATURE_INT);
    jfieldID field_joctreekey_z = env->GetFieldID(cls_joctreekey, FIELD_Z, SIGNATURE_INT);
    jfieldID field_pair_first = env->GetFieldID(cls_pair, FIELD_PAIR_KEY, CLS_OBJECT);
    jfieldID field_pair_second = env->GetFieldID(cls_pair, FIELD_PAIR_CONTENT, CLS_OBJECT);
    jfieldID field_float_value = env->GetFieldID(cls_float, FIELD_VALUE, SIGNATURE_FLOAT);
    //retrieve methods of classes used in this code
    jmethodID method_get_arraylist = env->GetMethodID(cls_arraylist, "get", "(I)Ljava/lang/Object;");
    jmethodID method_size_arraylist = env->GetMethodID(cls_arraylist, "size", "()I");
    jmethodID method_add_arraylist = env->GetMethodID(cls_arraylist, "add", "(Ljava/lang/Object;)Z");
    //retrieve argument-passed object fields
    jfieldID field_jpoint2d_x = env->GetFieldID(cls_point2d, FIELD_X, SIGNATURE_FLOAT);
    jfieldID field_jpoint2d_y = env->GetFieldID(cls_point2d, FIELD_Y, SIGNATURE_FLOAT);
    //retrieve argument-passed object field values
    jfloat point2d_x = env->GetFloatField(jpoint2d, field_jpoint2d_x);
    jfloat point2d_y = env->GetFloatField(jpoint2d, field_jpoint2d_y);

    //create array of neighbors
    jobject jarraylistneighbors = env->NewObject(cls_arraylist, method_constructor_arraylist);
    //current state
    point3d state = point3d(point2d_x, point2d_y, 0);
    //define priority queue with custom comparator
    ComparePoint3D comparator = ComparePoint3D(state);
    priorityqueue queue_frontier_points(comparator);
    double resolution = octree->getResolution();
    //get info for current node
    NodeInfo_Adjacencies info = search_node(octree, state, env, jadjacencymap, method_adjacency, cls_joctreekey, method_constructor_joctreekey);
    //know current adjacencies for this point
    jobject jcells = info.jadjacencies;
    //add current cell to the array
    jint jcells_size = env->CallIntMethod(jcells, method_size_arraylist);
    //variable to store the explored cells
    std::unordered_set<Point2D, Point2D_Hash> points_considered;
    for(int i = 0; i < jcells_size; i++){
        //current java method
        jobject jcurrentkey = env->CallObjectMethod(jcells, method_get_arraylist, i);
        //retrieve field values
        jint currentkey_x = env->GetIntField(jcurrentkey, field_joctreekey_x);
        jint currentkey_y = env->GetIntField(jcurrentkey, field_joctreekey_y);
        jint currentkey_z = env->GetIntField(jcurrentkey, field_joctreekey_z);
        //transform current method to native one
        OcTreeKey currentkey = OcTreeKey(currentkey_x, currentkey_y, currentkey_z);
        //node info
        NodeInfo current_node_info = node_info(currentkey, jcurrentkey, env, method_nodeinfo, field_float_value, field_pair_first, field_pair_second, field_jpoint2d_x, field_jpoint2d_y, jadjacencymap);
        //CENTER OF THE CELL CHECKING
        point3d center = octree->keyToCoord(currentkey);
        Point2D center_2d = Point2D(center);
        //Check if the key was already explored. If it was, then skip
        if(points_considered.find(center_2d) == points_considered.end()){
        	//add to the list of explored keys
        	points_considered.insert(center_2d);
			if(!checkCollision(center, radius, octree)){
				//create instance of Point2D
				jobject jpoint2dneighbor = env->NewObject(cls_point2d, method_constructor_point2d, center.x(), center.y());
				//create transition object
				//jobject transition = env->NewObject(cls_transition, method_constructor_transition, jpoint2d, jpoint2dneighbor);
				//add to the arraylist
				env->CallBooleanMethod(jarraylistneighbors, method_add_arraylist, jpoint2dneighbor);
				//delete local references used by instantiated objects
				//env->DeleteLocalRef(transition);
				env->DeleteLocalRef(jpoint2dneighbor);
			}
			//RELEVANT FRONTIER POINTS CHECKING
			frontier_points(resolution, current_node_info.size, current_node_info.coordinate, queue_frontier_points);
			int generated = 0;
			while(queue_frontier_points.size() > 0 && generated < POINTS_CONSIDERED){
				//retrieve first
				point3d current = queue_frontier_points.top();
				//remove first
				queue_frontier_points.pop();
				if(current == state) continue;
				if(!checkCollision(current, radius, octree)){
					//create instance of Point2D
					jobject jpoint2dneighbor = env->NewObject(cls_point2d, method_constructor_point2d, current.x(), current.y());
					//create transition object
					//jobject transition = env->NewObject(cls_transition, method_constructor_transition, jpoint2d, jpoint2dneighbor);
					//add to the arraylist
					env->CallBooleanMethod(jarraylistneighbors, method_add_arraylist, jpoint2dneighbor);
					generated++;
					//delete local references used by instantiated objects
					//env->DeleteLocalRef(transition);
					env->DeleteLocalRef(jpoint2dneighbor);
				}
			}
			//clear content of the queue
			priorityqueue empty(comparator);
			queue_frontier_points.swap(empty);
        }
    }
    
	//add transition to go to the center of the current cell
    point3d center = point3d(info.coordinate.x(), info.coordinate.y(), 0);
    if(center.distance(state) > 0 && !checkCollision(center, radius, octree)){
        //create instance of Point2D
        jobject jpoint2dneighbor = env->NewObject(cls_point2d, method_constructor_point2d, center.x(), center.y());
        //create transition object
        //jobject transition = env->NewObject(cls_transition, method_constructor_transition, jpoint2d, jpoint2dneighbor);
        //add to the arraylist
        env->CallBooleanMethod(jarraylistneighbors, method_add_arraylist, jpoint2dneighbor);
        //delete local references used by instantiated objects
        //env->DeleteLocalRef(transition);
        env->DeleteLocalRef(jpoint2dneighbor);
    }
    
    //delete local references used by classes
    env->DeleteLocalRef(cls_adjacencymap);
    env->DeleteLocalRef(cls_arraylist);
    env->DeleteLocalRef(cls_joctree_utils);
    env->DeleteLocalRef(cls_joctreekey);
    env->DeleteLocalRef(cls_pair);
    env->DeleteLocalRef(cls_point2d);
    //env->DeleteLocalRef(cls_transition);
    
    //delete local references used by instantiated objects
    //env->DeleteLocalRef(jarraylistneighbors);
    
    //return result
    return jarraylistneighbors;
}
