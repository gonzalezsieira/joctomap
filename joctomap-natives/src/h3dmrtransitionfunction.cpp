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
//
// Created by adrian.gonzalez on 24/08/17.
//
#include <map>
#include <queue>
#include <jni.h>
#include <octomap/OcTree.h>
#include "nativeobject.h"
#include "definitions.h"
#include "adjacencymap.h"
#include "transitionfunctionutils.h"
#include "h3dmrtransitionfunction.h

using namespace octomap;

struct StaticInformation3D : StaticInformation{
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
    jclass cls_point3d;
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
    jmethodID method_constructor_point3d;
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
    jfieldID field_jpoint3d_x;
    jfieldID field_jpoint3d_y;
    jfieldID field_jpoint3d_z;

    //auxiliary information
    int maxdepth;
    float maxdepthsize;
    std::list<float> neighbors_directions;
    std::unordered_map<float, Point3D> neighbors;
    int POINTS_CONSIDERED = 2;

    //cache for collision check
    std::unordered_map<Point2D, bool, Point3D_Hash> cache_collisions;


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
        jclass cls_point3d_local = env->FindClass(CLS_POINT3D);
        jclass cls_float_local = env->FindClass(CLS_FLOAT);
        jclass cls_joctreekey_local = env->FindClass(CLS_JOCTREEKEY);
        jclass cls_arraylist_local = env->FindClass(CLS_ARRAYLIST);
        jclass cls_transition_local = env->FindClass(CLS_TRANSITION);
        jclass cls_pair_local = env->FindClass(CLS_PAIR);
        jclass cls_map_local = env->FindClass(CLS_MAP);
        //CONVERT TO GLOBAL REFERENCES
        this->cls_adjacencymap = (jclass) env->NewGlobalRef(cls_adjacencymap_local);
        this->cls_point3d = (jclass) env->NewGlobalRef(cls_point3d_local);
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
        env->DeleteLocalRef(cls_point3d_local);
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
        this->method_constructor_point3d = env->GetMethodID(cls_point3d, METHOD_CONSTRUCTOR, "(FFF)V");
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
        this->field_jpoint3d_x = env->GetFieldID(cls_point3d, FIELD_X, SIGNATURE_FLOAT);
        this->field_jpoint3d_y = env->GetFieldID(cls_point3d, FIELD_Y, SIGNATURE_FLOAT);
        this->field_jpoint3d_z = env->GetFieldID(cls_point3d, FIELD_Z, SIGNATURE_FLOAT);

        //calculate max depth
        this->maxdepth = octree->getTreeDepth();
        this->maxdepthsize = this->octree->getNodeSize(maxdepth);
        while(maxdepthsize < min_resolution_trajectories){
            maxdepth--;
            maxdepthsize = octree->getNodeSize(maxdepth);
        }

        //TODO: REVIEW FROM HERE
        //insert neighbors
        int level = 1;
        for (int i = -level; i <= level; i++) {
            for (int j = -level; j <= level; j++) {
                if (i == -level || i == level || j == -level || j == level) {
                    Point3D point = Point3D(maxdepthsize * i, maxdepthsize * j);
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
        env->DeleteGlobalRef(cls_point3d);
        env->DeleteGlobalRef(cls_transition);
        env->DeleteGlobalRef(cls_map);
        env->DeleteGlobalRef(cls_float);
        env->DeleteGlobalRef(jadjacencymap);
    }

};


/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    initialize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_initialize
        (JNIEnv *env, jobject){

}

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    transitionsFrom
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)Ljava/lang/Iterable;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_transitionsFrom
        (JNIEnv *env, jobject jh3dmrtransitiongenerator, jobject jpoint3d){

    StaticInformation* information = (StaticInformation*) getPointer(env, jh3dmrtransitiongenerator);

    //retrieve argument-passed object field values
    jfloat point3d_x = env->GetFloatField(jpoint3d, information->field_jpoint3d_x);
    jfloat point3d_y = env->GetFloatField(jpoint3d, information->field_jpoint3d_y);
    jfloat point3d_z = env->GetFloatField(jpoint3d, information->field_jpoint3d_z);

    //create array of neighbors
    jobject jarraylistneighbors = env->NewObject(information->cls_arraylist, information->method_constructor_arraylist);
    //current state
    point3d state = point3d(point3d_x, point3d_y, point3d_z);
    Point3D state_3D = Point3D(state);
    //define priority queue with custom comparator
    ComparePoint3D comparator = ComparePoint3D(state);
    priorityqueue queue_frontier_points(comparator);
    //get min/max positions in octree
    double octree_min_x, octree_min_y, octree_min_z, octree_max_x, octree_max_y, octree_max_z;
    information->octree->getMetricMin(octree_min_x, octree_min_y, octree_min_z);
    information->octree->getMetricMax(octree_max_x, octree_max_y, octree_max_z);
    //get info for current node
    NodeInfo_Adjacencies info = search_node(
            information,
            state
    );
    Point3D center_of_current_cell = info.coordinate;
    Point3D state_3D_compare = Point3D(information->octree->keyToCoord(info.key, information->maxdepth));
    //know current adjacencies for this point
    jobject jcells = info.jadjacencies;
    jint jcells_size = env->CallIntMethod(jcells, information->method_size_arraylist);
    //variable to store the explored cells
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
        //node info
        NodeInfo current_node_info = node_info(
                information,
                currentkey,
                jcurrentkey
        );
        //CENTER OF THE CELL CHECKING
        point3d center = point3d(current_node_info.coordinate.x(), current_node_info.coordinate.y(), current_node_info.coordinate.z());
        //compare this way to avoid precision problems
        if(current_node_info.size - information->maxdepthsize > 0.001){
            frontier_points(current_node_info.size, current_node_info.coordinate, queue_frontier_points);
        }
        else{
            Point3D upCenter = Point3D(information->octree->keyToCoord(currentkey, information->maxdepth));
            if(fabs(upCenter.x() - state_3D_compare.x()) < 0.001 && fabs(upCenter.y() - state_3D_compare.y()) < 0.001 && fabs(upCenter.z() - state_3D_compare.z()) < 0.001){
                float yawNeighbor = atan2(center.y() - center_of_current_cell.y(), center.x() - center_of_current_cell.x());
                float pitchNeighbor;
                //adapt to available orientations
                yawNeighbor = closestOrientationTo(information->neighbors_directions, yawNeighbor);
                Point3D neighbor = information->neighbors[orientation_adapted];
                upCenter = Point3D(upCenter.x() + neighbor.x(), upCenter.y() + neighbor.y(), upCenter.z() + neighbor.z());
            }
            if(!checkCollision_Cached){
                frontier_points(information->maxdepthsize, upCenter, queue_frontier_points);
            }
            //only add center
            else{
                queue_frontier_points.push(point3d(upCenter.x(), upCenter.y(), 0));
            }
        }
        //Generate the transition to the couple of nearest frontier points of the adjacent cell
        int generated = 0;
        while(queue_frontier_points.size() > 0 /*&& generated < information->POINTS_CONSIDERED*/){
            //retrieve first
            point3d current = queue_frontier_points.top();
            //remove first
            queue_frontier_points.pop();
            if(current == state || !isInBounds(octree_min_x, octree_min_y, octree_max_x, octree_max_y, current)) continue;
            if(!checkCollision_Cached(information, current)){
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

    //return result
    return jarraylistneighbors;
}

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_dispose
(JNIEnv *env, jobject){

}