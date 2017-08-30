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
#include <jni.h>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <limits>
#include <octomap/OcTree.h>
#include "h2dmrtransitionfunction.h"
#include "nativeobject.h"
#include "transitionfunctionutils.h"

using namespace octomap;


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
            information,
            state
    );
    Point2D center_of_current_cell = info.coordinate;
    Point2D state_2D_compare = Point2D(information->octree->keyToCoord(info.key, information->maxdepth));
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
        //node info
        NodeInfo current_node_info = node_info(
                information,
                currentkey,
                jcurrentkey
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
                frontier_points2d(current_node_info.size, current_node_info.coordinate, queue_frontier_points);
            }
            else{
                Point2D upCenter = Point2D(information->octree->keyToCoord(currentkey, information->maxdepth));
                if(fabs(upCenter.x() - state_2D_compare.x()) < 0.001 && fabs(upCenter.y() - state_2D_compare.y()) < 0.001){
                    float directionNeighbor = atan2(center.y() - center_of_current_cell.y(), center.x() - center_of_current_cell.x());
                    float orientation_adapted = closestOrientationTo(information->neighbors_directions, directionNeighbor);
                    Point2D neighbor = information->neighbors[orientation_adapted];
                    upCenter = Point2D(upCenter.x() + neighbor.x(), upCenter.y() + neighbor.y());
                }
                //if current cell is occupied, try subsampling
                point3d upCenter3d = point3d(upCenter.x(), upCenter.y(), 0);
                if(checkCollision2d_Cached(information, upCenter3d)){
                    frontier_points2d(information->maxdepthsize, upCenter, queue_frontier_points);
                }
                //only add center
                else{
                    queue_frontier_points.push(upCenter3d);
                }
            }
            //Generate the transition to the couple of nearest frontier points of the adjacent cell
            int generated = 0;
            while(queue_frontier_points.size() > 0 && generated < information->POINTS_CONSIDERED){
                //retrieve first
                point3d current = queue_frontier_points.top();
                //remove first
                queue_frontier_points.pop();
                if(current == state || !isInBounds(octree_min_x, octree_min_y, octree_min_z, octree_max_x, octree_max_y, octree_max_z, current)) continue;
                if(!checkCollision2d_Cached(information, current)){
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


JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_hipster_H2DMRTransitionFunction_dispose
  (JNIEnv *env, jobject jh2dmrtransitiongenerator){
    StaticInformation* information = (StaticInformation*) getPointer(env, jh2dmrtransitiongenerator);
    delete information;
}

