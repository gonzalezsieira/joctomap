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
#include <unordered_set>
#include <jni.h>
#include <octomap/OcTree.h>
#include <octomap/math/Vector3.h>
#include "nativeobject.h"
#include "definitions.h"
#include "adjacencymap.h"
#include "transitionfunctionutils.h"
#include "h3dmrtransitionfunction.h"

using namespace octomap;
using namespace octomath;

/**
 * Computes the shortest distance between the segment and a given point
 *
 * @param point given point
 * @param point1 segment - point A
 * @param point2 segment - point B
 * @return shortest distance
 */
float distance_to_segment(point3d point, point3d point1, point3d point2){
    Vector3 v = Vector3(point2.x() - point1.x(), point2.y() - point1.y(), point2.z() - point1.z());
    Vector3 w = Vector3(point.x() - point1.x(), point.y() - point1.y(), point.z() - point1.z());
    float c1 = w.dot(v);
    if(c1 <= 0){
        return point.distance(point1);
    } else {
        float c2 = v.dot(v);
        if(c2 <= c1){
            return point.distance(point2);
        }
        else{
            float b = c1 / c2;
            point3d result = point3d(point1.x() + b * v.x(), point1.y() + b * v.y(), point1.z() + b * v.z());
            return point.distance(result);
        }
    }
}

bool isInBounds(double octree_min_x, double octree_min_y, double octree_min_z, double octree_max_x, double octree_max_y, double octree_max_z, point3d point){
    return point.x() > octree_min_x
           && point.x() < octree_max_x
           && point.y() > octree_min_y
           && point.y() < octree_max_y
           && point.z() > octree_min_z
           && point.z() < octree_max_z;
}

/**
 * Returns pair of angles : yaw, pitch, between two points (origin, dest).
 * @param point1 origin point
 * @param point2 destination point
 * @return view to point1 to point2
 */
std::pair<float, float> angles_to(point3d point1, point3d point2){
    float dx = point2.x() - point1.x();
    float dy = point2.y() - point1.y();
    float dz = point2.z() - point1.z();
    return std::pair<float, float>(atan2(dy, dx), atan2(dz, sqrt(dx * dx + dy * dy)));
}

/**
 * Computes the frontier points in 3D.
 *
 * @param size_cell
 * @param center
 * @param queue
 */
void frontier_points(float size_cell, point3d center, point3d_priorityqueue &queue){
    //add four positions within each cell
    float diff = size_cell / 4;
    float centerXMinuDiff = center.x() - diff;
    float centerYMinuDiff = center.y() - diff;
    float centerZMinuDiff = center.z() - diff;
    float centerXPlusDiff = center.x() + diff;
    float centerYPlusDiff = center.y() + diff;
    float centerZPlusDiff = center.z() + diff;
    queue.push(point3d(centerXMinuDiff, centerYMinuDiff, centerZMinuDiff));
    queue.push(point3d(centerXMinuDiff, centerYPlusDiff, centerZMinuDiff));
    queue.push(point3d(centerXPlusDiff, centerYMinuDiff, centerZMinuDiff));
    queue.push(point3d(centerXPlusDiff, centerYPlusDiff, centerZMinuDiff));
    queue.push(point3d(centerXMinuDiff, centerYMinuDiff, centerZPlusDiff));
    queue.push(point3d(centerXMinuDiff, centerYPlusDiff, centerZPlusDiff));
    queue.push(point3d(centerXPlusDiff, centerYMinuDiff, centerZPlusDiff));
    queue.push(point3d(centerXPlusDiff, centerYPlusDiff, centerZPlusDiff));
}

struct StaticInformation3D : StaticInformation{

    //classes
    jclass cls_set;
    jclass cls_iterator;

    //field IDs
    jfieldID field_jpoint3d_x;
    jfieldID field_jpoint3d_y;
    jfieldID field_jpoint3d_z;

    //constructor
    jmethodID method_constructor_point3d;
    jmethodID method_iterator_next;
    jmethodID method_iterator_hasnext;

    //other methods
    jmethodID method_map_keyset;
    jmethodID method_set_iterator;

    //auxiliary information
    std::list<float> directions_yaw;
    std::list<float> directions_pitch;
    std::unordered_map<std::pair<float, float>, Point3D, Pair_Hash> neighbors;

    //cache for collision check
    std::unordered_map<std::pair<Point3D, Point3D>, bool, Pair_Hash> cache_collisions;


    StaticInformation3D(JNIEnv *env, long octree_pointer, jobject jadjacencymap, float radius_optimistic, float min_resolution_trajectories, jobject h3dmr_neighbors_information) : StaticInformation(env, octree_pointer, jadjacencymap, radius_optimistic, min_resolution_trajectories){

        //classes
        jclass cls_set_local = env->FindClass(CLS_SET);
        jclass cls_iterator_local = env->FindClass(CLS_ITERATOR);

        //convert to global references
        this->cls_set = (jclass) env->NewGlobalRef(cls_set_local);
        this->cls_iterator = (jclass) env->NewGlobalRef(cls_iterator_local);

        //DELETE LOCAL REFERENCES (no longer used)
        env->DeleteLocalRef(cls_set_local);
        env->DeleteGlobalRef(cls_iterator_local);

        //constructor methods
        this->method_constructor_point3d = env->GetMethodID(cls_point3d, METHOD_CONSTRUCTOR, "(FFF)V");

        //other methods
        this->method_map_keyset = env->GetMethodID(cls_map, "keySet", "()Ljava/util/Set;");
        this->method_set_iterator = env->GetMethodID(cls_set, "iterator", "()Ljava/util/Iterator;");
        this->method_iterator_next = env->GetMethodID(cls_iterator, "next", "()Ljava/lang/Object;");
        this->method_iterator_hasnext = env->GetMethodID(cls_iterator, "hasNext", "()Z");

        //retrieve argument-passed object fields
        this->field_jpoint3d_x = env->GetFieldID(cls_point3d, FIELD_X, SIGNATURE_FLOAT);
        this->field_jpoint3d_y = env->GetFieldID(cls_point3d, FIELD_Y, SIGNATURE_FLOAT);
        this->field_jpoint3d_z = env->GetFieldID(cls_point3d, FIELD_Z, SIGNATURE_FLOAT);

        //get neighbor information and put in C++ structs
        jclass cls_h3dmr_neighbors_information = env->FindClass("Les/usc/citius/lab/motionplanner/planner/tridimensional/hipster/heuristic/H3DMR$Neighbors;");
        jfieldID field_neighbors_information = env->GetFieldID(cls_h3dmr_neighbors_information, "neighborsByDirection", "Ljava/util/Map;");
        jobject map_neighbors = env->GetObjectField(h3dmr_neighbors_information, field_neighbors_information);
        jobject map_neighbors_set = env->CallObjectMethod(map_neighbors, method_map_keyset);
        jobject map_neighbors_set_iterator = env->CallObjectMethod(map_neighbors_set, method_set_iterator);
        std::unordered_set<float> set_yaw;
        std::unordered_set<float> set_pitch;
        //get elements of the key
        //hasNext()
        while(env->CallBooleanMethod(map_neighbors_set_iterator, method_iterator_hasnext)){
            //next()
            jobject current_key = env->CallObjectMethod(map_neighbors_set_iterator, method_iterator_next);
            //get first/second values
            jobject current_key_first = env->GetObjectField(current_key, field_pair_first);
            jobject current_key_second = env->GetObjectField(current_key, field_pair_second);
            //get float values
            float current_key_first_floatvalue = env->GetFloatField(current_key_first, field_float_value);
            float current_key_second_floatvalue = env->GetFloatField(current_key_second, field_float_value);
            //get jpoint3d
            jobject current_value = env->CallObjectMethod(map_neighbors, method_map_get, current_key);
            float current_value_x = env->GetFloatField(current_value, field_point3d_x);
            float current_value_y = env->GetFloatField(current_value, field_point3d_y);
            float current_value_z = env->GetFloatField(current_value, field_point3d_z);
            //put information in C++ map
            this->neighbors[std::pair<float, float>(current_key_first_floatvalue, current_key_second_floatvalue)] = Point3D(current_value_x, current_value_y, current_value_z);
            //put values in the arrays of values (yaw/pitch)
            set_yaw.insert(current_key_first_floatvalue);
            set_pitch.insert(current_key_second_floatvalue);
        }
        //populate arrays with values (from sets)
        for(std::unordered_set<float>::const_iterator iterator_yaw = set_yaw.begin(); iterator_yaw != set_yaw.end(); ++iterator_yaw){
            this->directions_yaw.push_back(*iterator_yaw);
        }
        for(std::unordered_set<float>::const_iterator iterator_pitch = set_pitch.begin(); iterator_pitch != set_pitch.end(); ++iterator_pitch){
            this->directions_pitch.push_back(*iterator_pitch);
        }

    }

    Point3D closestNeighborTo(float yaw, float pitch){
        float yaw_adapted = closestOrientationTo(this->directions_yaw, yaw);
        //TODO: REVIEW HERE, does not enter the loop
        float_priorityqueue pitch_ordered = closestOrientations(this->directions_pitch, pitch);
        while(!pitch_ordered.empty()){
            //get first
            float pitch_adapted = pitch_ordered.top();
            //query neighbors
            std::unordered_map<std::pair<float, float>, Point3D, Pair_Hash>::const_iterator query_result = this->neighbors.find(std::pair<float, float>(yaw_adapted, pitch_adapted));
            //found, so return
            if(query_result != this->neighbors.end()){
                return query_result->second;
            }
            //remove from the queue
            pitch_ordered.pop();
        }
        //not found - throw exception
        throw;
    }

    bool checkCollision3D_Line(const point3d point1, const point3d point2){
        float resolutionPlusRadius = this->octree->getResolution() + this->radius_optimistic;
        float min_x = std::min(point1.x(), point2.x()) - resolutionPlusRadius;
        float min_y = std::min(point1.y(), point2.y()) - resolutionPlusRadius;
        float min_z = std::min(point1.z(), point2.z()) - resolutionPlusRadius;
        float max_x = std::max(point1.x(), point2.x()) + resolutionPlusRadius;
        float max_y = std::max(point1.y(), point2.y()) + resolutionPlusRadius;
        float max_z = std::max(point1.z(), point2.z()) + resolutionPlusRadius;
        //distance = positive infinity
        float distance = std::numeric_limits<float>::max();
        //iterate over occupied cells
        for(OcTree::leaf_bbx_iterator it=this->octree->begin_leafs_bbx(point3d(min_x, min_y, min_z), point3d(max_x, max_y, max_z)), end=this->octree->end_leafs_bbx(); it != end; ++it){
            //transform current leaf_bbx_iterator to node
            OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
            if( this->octree->isNodeOccupied(currentNode)){
                float new_distance = distance_to_segment(it.getCoordinate(), point1, point2);
                distance = std::min(distance, new_distance);
                //collision detected
                if(distance <= this->radius_optimistic + it.getSize()){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkCollision3D_Cached(point3d point1, point3d point2){
        Point3D point1_query = Point3D(point1);
        Point3D point2_query = Point3D(point2);
        //create query
        std::pair<Point3D, Point3D> query1 = std::pair<Point3D, Point3D>(point1_query, point2_query);
        bool result;
        //not found in cache
        if(this->cache_collisions.find(query1) == this->cache_collisions.end()){
            //try reversed query
            std::pair<Point3D, Point3D> query2 = std::pair<Point3D, Point3D>(point2_query, point1_query);
            if(this->cache_collisions.find(query2) == this->cache_collisions.end()) {
                result = checkCollision3D_Line(point1, point2);
                //update cache with both entries
                this->cache_collisions[query1] = result;
                this->cache_collisions[query2] = result;
            }
            else{
                //retrieve result from query2
                result = this->cache_collisions[query2];
                //update cache with first entry
                this->cache_collisions[query1] = result;
            }
            //retrieve result from query1
            this->cache_collisions[query1] = result;
        }
        else{
            result = this->cache_collisions[query1];
        }
        return result;
    }

    ~StaticInformation3D(){
        env->DeleteGlobalRef(this->cls_set);
        env->DeleteGlobalRef(this->cls_iterator);
    }

};


/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    initialize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_initialize
        (JNIEnv *env, jobject h3dmrtransitionfunction, jlong octree_pointer, jobject jadjacency_map, jfloat radius_optimistic, jfloat min_resolution_trajectories, jobject h3dmr_neighbor_information){
        StaticInformation3D *information = new StaticInformation3D(env, octree_pointer, jadjacency_map, radius_optimistic, min_resolution_trajectories, h3dmr_neighbor_information);
        return (long) information;
}

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    transitionsFrom
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)Ljava/lang/Iterable;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_transitionsFrom
        (JNIEnv *env, jobject jh3dmrtransitiongenerator, jobject jpoint3d){

    StaticInformation3D* information = (StaticInformation3D*) getPointer(env, jh3dmrtransitiongenerator);

    //retrieve argument-passed object field values
    jfloat point3d_x = env->GetFloatField(jpoint3d, information->field_jpoint3d_x);
    jfloat point3d_y = env->GetFloatField(jpoint3d, information->field_jpoint3d_y);
    jfloat point3d_z = env->GetFloatField(jpoint3d, information->field_jpoint3d_z);

    //create array of neighbors
    jobject jarraylistneighbors = env->NewObject(information->cls_arraylist, information->method_constructor_arraylist);
    //current state
    point3d state = point3d(point3d_x, point3d_y, point3d_z);
    //define priority queue with custom comparator
    ComparePoint3D comparator = ComparePoint3D(state);
    point3d_priorityqueue queue_frontier_points(comparator);
    //get min/max positions in octree
    double octree_min_x, octree_min_y, octree_min_z, octree_max_x, octree_max_y, octree_max_z;
    information->octree->getMetricMin(octree_min_x, octree_min_y, octree_min_z);
    information->octree->getMetricMax(octree_max_x, octree_max_y, octree_max_z);
    //get info for current node
    NodeInfo_Adjacencies info = search_node(
            information,
            state
    );
    //set of tested points
    std::unordered_set<point3d> tested;
    point3d center_of_current_cell = info.coordinate;
    point3d state_3D_compare = information->octree->keyToCoord(info.key, information->maxdepth);
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
        point3d center = current_node_info.coordinate;
        //compare this way to avoid precision problems
        if(current_node_info.size - information->maxdepthsize > 0.001){
            frontier_points(current_node_info.size, current_node_info.coordinate, queue_frontier_points);
        }
        else {
            point3d upCenter = information->octree->keyToCoord(currentkey, information->maxdepth);
            //avoid testing the same upscaled cell more than once
            if (tested.find(upCenter) == tested.end()) {
                tested.insert(upCenter);
                if (fabs(upCenter.x() - state_3D_compare.x()) < 0.001 &&
                    fabs(upCenter.y() - state_3D_compare.y()) < 0.001 &&
                    fabs(upCenter.z() - state_3D_compare.z()) < 0.001) {
                    std::pair<float, float> angles = angles_to(center_of_current_cell, center);
                    //get closest neighbor (yaw, pitch)
                    Point3D neighbor = information->closestNeighborTo(angles.first, angles.second);
                    upCenter = point3d(upCenter.x() + neighbor.x(), upCenter.y() + neighbor.y(),
                                       upCenter.z() + neighbor.z());
                }
                if (information->checkCollision3D_Cached(state, upCenter)) {
                    frontier_points(information->maxdepthsize, upCenter, queue_frontier_points);
                }
                //only add center
                else {
                    queue_frontier_points.push(upCenter);
                }
            }
        }
        //Generate the transition to the couple of nearest frontier points of the adjacent cell
        int generated = 0;
        while(queue_frontier_points.size() > 0 /*&& generated < information->POINTS_CONSIDERED*/){
            //retrieve first
            point3d current = queue_frontier_points.top();
            //remove first
            queue_frontier_points.pop();
            if(current == state || !isInBounds(octree_min_x, octree_min_y, octree_min_z, octree_max_x, octree_max_y, octree_max_z, current)) continue;
            if(!information->checkCollision3D_Cached(state, current)){
                //create instance of Point2D
                jobject jpoint3dneighbor = env->NewObject(information->cls_point3d, information->method_constructor_point3d, current.x(), current.y(), current.z());
                //create transition object
                jobject transition = env->CallObjectMethod(information->cls_transition, information->method_create_transition, jpoint3d, jpoint3dneighbor);
                //add to the arraylist
                env->CallBooleanMethod(jarraylistneighbors, information->method_add_arraylist, transition);
                generated++;
                //delete local references used by instantiated objects
                env->DeleteLocalRef(transition);
                env->DeleteLocalRef(jpoint3dneighbor);
            }
        }
        //clear content of the queue
        point3d_priorityqueue empty(comparator);
        queue_frontier_points.swap(empty);
    }

    //return result
    return jarraylistneighbors;
}

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    innerSamplesOfCell
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;FLjava/util/Queue;)V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_innerSamplesOfCell
  (JNIEnv *env, jclass cls_h3dmrtransitionfunction, jobject jpoint3d, jfloat jsize, jobject jqueue){

        jclass cls_queue = env->FindClass(CLS_QUEUE);
        jmethodID method_add = env->GetMethodID(cls_queue, "add", "(Ljava/lang/Object;)Z");

        jclass cls_point3d = env->FindClass(CLS_POINT3D);
        jfieldID field_point3d_x = env->GetFieldID(cls_point3d, FIELD_X, SIGNATURE_FLOAT);
        jfieldID field_point3d_y = env->GetFieldID(cls_point3d, FIELD_Y, SIGNATURE_FLOAT);
        jfieldID field_point3d_z = env->GetFieldID(cls_point3d, FIELD_Z, SIGNATURE_FLOAT);
        jmethodID constructor_point3d = env->GetMethodID(cls_point3d, METHOD_CONSTRUCTOR, "(FFF)V");

        //retrieve argument-passed object field values
        jfloat point3d_x = env->GetFloatField(jpoint3d, field_point3d_x);
        jfloat point3d_y = env->GetFloatField(jpoint3d, field_point3d_y);
        jfloat point3d_z = env->GetFloatField(jpoint3d, field_point3d_z);

        //origin point
        point3d origin(point3d_x, point3d_y, point3d_z);
        //comparator
        ComparePoint3D comparator(origin);
        point3d_priorityqueue queue_frontier_points(comparator);
        //create points
        frontier_points(jsize, origin, queue_frontier_points);
        while(queue_frontier_points.size() > 0){
            //retrieve point
            point3d top = queue_frontier_points.top();
            //remove from queue
            queue_frontier_points.pop();
            //create jpoint3d
            jobject jpoint3d = env->NewObject(cls_point3d, constructor_point3d, top.x(), top.y(), top.z());
            //insert in queue
            env->CallBooleanMethod(jqueue, method_add, jpoint3d);
        }
        env->DeleteLocalRef(cls_point3d);
        env->DeleteLocalRef(cls_queue);
  }

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_dispose
    (JNIEnv *env, jobject j3dmrtransitiongenerator){
    StaticInformation3D* information = (StaticInformation3D*) getPointer(env, j3dmrtransitiongenerator);
    delete information;
}