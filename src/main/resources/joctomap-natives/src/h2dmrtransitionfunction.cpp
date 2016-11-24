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
#include <jni.h>
#include <map>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <limits>
#include <octomap/OcTree.h>
#include <octomap/OcTreeKey.h>
#include <math.h>
#include "collisions.h"
#include "h2dmrtransitionfunction.h"
#include "nativeobject.h"
#include "definitions.h"

using namespace octomap;


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


struct StaticInformation{
    //environment (to release references later)
    JNIEnv *env;
    
    //other information used by the transition function
    jobject jadjacencymap;
    float radius_optimistic;
    float min_resolution_trajectories;
    
    //static information
    OcTree* octree;
    
    //Java classes
    jclass cls_adjacencymap;
    jclass cls_point2d;
    jclass cls_float;
    jclass cls_joctreekey;
    jclass cls_arraylist;
    jclass cls_transition;
    jclass cls_pair;
    jclass cls_map;
    
    //java method
    jmethodID method_adjacency;
    jmethodID method_nodeinfo;
    jmethodID method_constructor_arraylist;
    jmethodID method_constructor_joctreekey;
    jmethodID method_create_transition;
    jmethodID method_constructor_point2d;
    jmethodID method_constructor_float;
    jmethodID method_get;
    jmethodID method_get_arraylist;
    jmethodID method_size_arraylist;
    jmethodID method_add_arraylist;
    
    //java fields ID
    jfieldID field_joctreekey_x;
    jfieldID field_joctreekey_y;
    jfieldID field_joctreekey_z;
    jfieldID field_pair_first;
    jfieldID field_pair_second;
    jfieldID field_float_value;
    jfieldID field_jpoint2d_x;
    jfieldID field_jpoint2d_y;
    
    //auxiliary information
    int maxdepth;
    float maxdepthsize;
    std::list<float> neighbors_directions;
    std::map<float, Point2D> neighbors;
    int POINTS_CONSIDERED = 2;
    
    
    StaticInformation(JNIEnv *env, long octree_pointer, jobject jadjacencymap, float radius_optimistic, float min_resolution_trajectories){
        //java java excution environment
        this->env = env;
        //variables
        this->jadjacencymap = (jobject) env->NewGlobalRef(jadjacencymap);
        this->radius_optimistic = radius_optimistic;
        this->min_resolution_trajectories = min_resolution_trajectories;
        
        //retrieve native objects
        this->octree = (OcTree*) octree_pointer;
        //retrieve orientations
        //jsize len_jorientationneighbors = env->GetArrayLength(jorientationsneighbors);
        //jfloat* orientationneighbors = env->GetFloatArrayElements(jorientationsneighbors, 0);
        //retrieve argument-passed types and retrieve classes used in this method
        //LOCAL REFERENCES
        jclass cls_adjacencymap_local = env->FindClass(CLS_JADJACENCYMAP);
        jclass cls_point2d_local = env->FindClass(CLS_POINT2D);
        jclass cls_float_local = env->FindClass(CLS_FLOAT);
        jclass cls_joctreekey_local = env->FindClass(CLS_JOCTREEKEY);
        jclass cls_arraylist_local = env->FindClass(CLS_ARRAYLIST);
        jclass cls_transition_local = env->FindClass(CLS_TRANSITION);
        jclass cls_pair_local = env->FindClass(CLS_PAIR);
        jclass cls_map_local = env->FindClass(CLS_MAP);
        //CONVERT TO GLOBAL REFERENCES
        this->cls_adjacencymap = (jclass) env->NewGlobalRef(cls_adjacencymap_local);
        this->cls_point2d = (jclass) env->NewGlobalRef(cls_point2d_local);
        this->cls_float = (jclass) env->NewGlobalRef(cls_float_local);
        this->cls_joctreekey = (jclass) env->NewGlobalRef(cls_joctreekey_local);
        this->cls_arraylist = (jclass) env->NewGlobalRef(cls_arraylist_local);
        this->cls_transition = (jclass) env->NewGlobalRef(cls_transition_local);
        this->cls_pair = (jclass) env->NewGlobalRef(cls_pair_local);
        this->cls_map = (jclass) env->NewGlobalRef(cls_map_local);
        //DELETE LOCAL REFERENCES (no longer used)
        env->DeleteLocalRef(cls_adjacencymap_local);
        env->DeleteLocalRef(cls_arraylist_local);
        env->DeleteLocalRef(cls_joctreekey_local);
        env->DeleteLocalRef(cls_pair_local);
        env->DeleteLocalRef(cls_point2d_local);
        env->DeleteLocalRef(cls_transition_local);
        env->DeleteLocalRef(cls_map_local);
        env->DeleteLocalRef(cls_float_local);
        
        //retrieve argument-passed object methods
        this->method_adjacency = env->GetMethodID(cls_adjacencymap, "adjacency", "(Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Ljava/util/List;");
        this->method_nodeinfo = env->GetMethodID(cls_adjacencymap, "nodeInfo", "(Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Les/usc/citius/lab/motionplanner/core/util/Pair;");
        //retrieve constructors of classes used in this method
        this->method_constructor_arraylist = env->GetMethodID(cls_arraylist, METHOD_CONSTRUCTOR, "()V");
        this->method_constructor_joctreekey = env->GetMethodID(cls_joctreekey, METHOD_CONSTRUCTOR, "(III)V");
        this->method_create_transition = env->GetStaticMethodID(cls_transition, "create", "(Ljava/lang/Object;Ljava/lang/Object;)Les/usc/citius/hipster/model/Transition;");
        this->method_constructor_point2d = env->GetMethodID(cls_point2d, METHOD_CONSTRUCTOR, "(FF)V");
        this->method_constructor_float = env->GetMethodID(cls_float, METHOD_CONSTRUCTOR, "(F)V");
        this->method_get = env->GetMethodID(cls_map, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
        //retrieve fields used by classes in this method
        this->field_joctreekey_x = env->GetFieldID(cls_joctreekey, FIELD_X, SIGNATURE_INT);
        this->field_joctreekey_y = env->GetFieldID(cls_joctreekey, FIELD_Y, SIGNATURE_INT);
        this->field_joctreekey_z = env->GetFieldID(cls_joctreekey, FIELD_Z, SIGNATURE_INT);
        this->field_pair_first = env->GetFieldID(cls_pair, FIELD_PAIR_KEY, CLS_OBJECT);
        this->field_pair_second = env->GetFieldID(cls_pair, FIELD_PAIR_CONTENT, CLS_OBJECT);
        this->field_float_value = env->GetFieldID(cls_float, FIELD_VALUE, SIGNATURE_FLOAT);
        //retrieve methods of classes used in this code
        this->method_get_arraylist = env->GetMethodID(cls_arraylist, "get", "(I)Ljava/lang/Object;");
        this->method_size_arraylist = env->GetMethodID(cls_arraylist, "size", "()I");
        this->method_add_arraylist = env->GetMethodID(cls_arraylist, "add", "(Ljava/lang/Object;)Z");
        //retrieve argument-passed object fields
        this->field_jpoint2d_x = env->GetFieldID(cls_point2d, FIELD_X, SIGNATURE_FLOAT);
        this->field_jpoint2d_y = env->GetFieldID(cls_point2d, FIELD_Y, SIGNATURE_FLOAT);
        
        //calculate max depth
        this->maxdepth = octree->getTreeDepth();
        this->maxdepthsize = this->octree->getNodeSize(maxdepth);
        while(maxdepthsize < min_resolution_trajectories){
            maxdepth--;
            maxdepthsize = octree->getNodeSize(maxdepth);
        }
        
        //insert neighbors
        int level = 1;
        for (int i = -level; i <= level; i++) {
            for (int j = -level; j <= level; j++) {
                if (i == -level || i == level || j == -level || j == level) {
                    Point2D point = Point2D(maxdepthsize * i, maxdepthsize * j);
                    float direction = atan2(point.y(), point.x());
                    this->neighbors_directions.push_back(direction);
                    this->neighbors[direction] = point;
                }
            }
        }
    }
    
    ~StaticInformation(){
        env->DeleteGlobalRef(cls_adjacencymap);
        env->DeleteGlobalRef(cls_arraylist);
        env->DeleteGlobalRef(cls_joctreekey);
        env->DeleteGlobalRef(cls_pair);
        env->DeleteGlobalRef(cls_point2d);
        env->DeleteGlobalRef(cls_transition);
        env->DeleteGlobalRef(cls_map);
        env->DeleteGlobalRef(cls_float);
        env->DeleteGlobalRef(jadjacencymap);
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

void frontier_points(float size_cell, Point2D center, priorityqueue &queue){
    float diff = size_cell / 4.0;
    queue.push(point3d(center.x() - diff, center.y() - diff, 0));
    queue.push(point3d(center.x() - diff, center.y() + diff, 0));
    queue.push(point3d(center.x() + diff, center.y() + diff, 0));
    queue.push(point3d(center.x() + diff, center.y() - diff, 0));
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
    return NodeInfo_Adjacencies( key, octree->getNodeSize(depth), jkey, Point2D(octree->keyToCoord(key, depth)), jadjacencies );
    
}

bool isInBounds(double octree_min_x, double octree_min_y, double octree_max_x, double octree_max_y, point3d point){
    return point.x() > octree_min_x
            && point.x() < octree_max_x
            && point.y() > octree_min_y
            && point.y() < octree_max_y;
}

float closestOrientationTo(std::list<float> neighbors_orientations, float value){
    float closestori = -1;
    float diff = std::numeric_limits<float>::infinity();
    for (std::list<float>::iterator list_iterator = neighbors_orientations.begin(); list_iterator != neighbors_orientations.end(); list_iterator++){
        float actualdiff = fabs(value - *list_iterator);
        if(actualdiff < diff){
            closestori = *list_iterator;
            diff = actualdiff;
        }
    }
    //switch to PI instead of -PI if needed
    if(fabs(-M_PI - closestori) < 0.0001){
        closestori = M_PI;
    }
    return closestori;
}


JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_hipster_H2DMRTransitionFunction_initialize
  (JNIEnv *env , jobject jh2dmrtransitiongenerator, jlong octree_pointer, jobject jadjacency_map, jfloat radius_optimistic, jfloat min_resolution_trajectories){
        StaticInformation *information = new StaticInformation(env, octree_pointer, jadjacency_map, radius_optimistic, min_resolution_trajectories);
        return (long) information;
}

JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_hipster_H2DMRTransitionFunction_transitionsFrom
  (JNIEnv *env, jobject jh2dmrtransitiongenerator, jobject jpoint2d){
    StaticInformation* information = (StaticInformation*) getPointer(env, jh2dmrtransitiongenerator);
    
    //retrieve argument-passed object field values
    jfloat point2d_x = env->GetFloatField(jpoint2d, information->field_jpoint2d_x);
    jfloat point2d_y = env->GetFloatField(jpoint2d, information->field_jpoint2d_y);

    //create array of neighbors
    jobject jarraylistneighbors = env->NewObject(information->cls_arraylist, information->method_constructor_arraylist);
    //current state
    point3d state = point3d(point2d_x, point2d_y, 0);
    Point2D state_2D = Point2D(state);
    //define priority queue with custom comparator
    ComparePoint3D comparator = ComparePoint3D(state);
    priorityqueue queue_frontier_points(comparator);
    //get min/max positions in octree
    double octree_min_x, octree_min_y, octree_min_z, octree_max_x, octree_max_y, octree_max_z;
    information->octree->getMetricMin(octree_min_x, octree_min_y, octree_min_z);
    information->octree->getMetricMax(octree_max_x, octree_max_y, octree_max_z);
    //get info for current node
    NodeInfo_Adjacencies info = search_node(
            information->octree, 
            state, 
            env, 
            information->jadjacencymap,
            information->method_adjacency,
            information->cls_joctreekey,
            information->method_constructor_joctreekey
    );
    //know current adjacencies for this point
    jobject jcells = info.jadjacencies;
    jint jcells_size = env->CallIntMethod(jcells, information->method_size_arraylist);
    //variable to store the explored cells
    std::unordered_set<Point2D, Point2D_Hash> points_considered;
    for(int i = 0; i < jcells_size; i++){
        //TODO: try converstion to jobjectarray outside loop?
        //current java method
        jobject jcurrentkey = env->CallObjectMethod(jcells, information->method_get_arraylist, i);
        //retrieve field values
        jint currentkey_x = env->GetIntField(jcurrentkey, information->field_joctreekey_x);
        jint currentkey_y = env->GetIntField(jcurrentkey, information->field_joctreekey_y);
        jint currentkey_z = env->GetIntField(jcurrentkey, information->field_joctreekey_z);
        //transform current method to native one
        OcTreeKey currentkey = OcTreeKey(currentkey_x, currentkey_y, currentkey_z);
        //TODO: pass information instead of all this params?
        //node info
        NodeInfo current_node_info = node_info(
                currentkey, 
                jcurrentkey, 
                env, 
                information->method_nodeinfo, 
                information->field_float_value, 
                information->field_pair_first, 
                information->field_pair_second,
                information->field_jpoint2d_x,
                information->field_jpoint2d_y,
                information->jadjacencymap
        );
        //CENTER OF THE CELL CHECKING
        point3d center = point3d(current_node_info.coordinate.x(), current_node_info.coordinate.y(), 0);
        Point2D center_2d = current_node_info.coordinate;
        //Check if the key was already explored. If it was, then skip
        if(points_considered.find(center_2d) == points_considered.end()){
            //add to the list of explored keys
            points_considered.insert(center_2d);
            //compare this way to avoid precision problems
            if(current_node_info.size - information->maxdepthsize > 0.001){
                frontier_points(current_node_info.size, current_node_info.coordinate, queue_frontier_points);
            }
            else{
                Point2D upCenter = Point2D(information->octree->keyToCoord(currentkey, information->maxdepth));
                if(upCenter == state_2D){
                    float directionNeighbor = atan2(center.y() - state_2D.y(), center.x() - state_2D.x());
                    float orientation_adapted = closestOrientationTo(information->neighbors_directions, directionNeighbor);
                    Point2D neighbor = information->neighbors[orientation_adapted];
                    upCenter = Point2D(upCenter.x() + neighbor.x(), upCenter.y() + neighbor.y());
                }
                //if current cell is occupied, try subsampling
                if(information->octree->isNodeOccupied(information->octree->search(upCenter.x(), upCenter.y(), 0, information->maxdepth))){
                    frontier_points(information->maxdepthsize, upCenter, queue_frontier_points);
                }
                //only add center
                else{
                    queue_frontier_points.push(point3d(upCenter.x(), upCenter.y(), 0));
                }
            }
            //Generate the transition to the couple of nearest frontier points of the adjacent cell
            int generated = 0;
            while(queue_frontier_points.size() > 0 && generated < information->POINTS_CONSIDERED){
                //retrieve first
                point3d current = queue_frontier_points.top();
                //remove first
                queue_frontier_points.pop();
                if(current == state || !isInBounds(octree_min_x, octree_min_y, octree_max_x, octree_max_y, current)) continue;
                if(!checkCollision(current, information->radius_optimistic, information->octree)){
                    //create instance of Point2D
                    jobject jpoint2dneighbor = env->NewObject(information->cls_point2d, information->method_constructor_point2d, current.x(), current.y());
                    //create transition object
                    jobject transition = env->CallObjectMethod(information->cls_transition, information->method_create_transition, jpoint2d, jpoint2dneighbor);
                    //add to the arraylist
                    env->CallBooleanMethod(jarraylistneighbors, information->method_add_arraylist, transition);
                    generated++;
                    //delete local references used by instantiated objects
                    env->DeleteLocalRef(transition);
                    env->DeleteLocalRef(jpoint2dneighbor);
                }
            }
            //clear content of the queue
            priorityqueue empty(comparator);
            queue_frontier_points.swap(empty);
        }
    }
    
    //return result
    return jarraylistneighbors;
}

