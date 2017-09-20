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
#include <algorithm>
#include "nativeobject.h"
#include "h2dmrtransitionfunction.h"
#include "transitionfunctionutils.h"

using namespace octomap;


struct StaticInformation2D : StaticInformation {

    jclass cls_point2d;

    jmethodID method_constructor_point2d;

    jfieldID field_jpoint2d_x;
    jfieldID field_jpoint2d_y;

    std::list<float> neighbors_directions;
    std::unordered_map<float, Point2D> neighbors2d;
    //cache for collision check
    std::unordered_map<Point2D, bool> cache_collisions;
    int POINTS_CONSIDERED = 2;

    //constructor
    StaticInformation2D(JNIEnv *env, long octree_pointer, jobject jadjacencymap, float radius_optimistic, float min_resolution_trajectories)
    : StaticInformation(env, octree_pointer, jadjacencymap, radius_optimistic, min_resolution_trajectories) {

        //LOCAL REFERENCES
        jclass cls_point2d_local = env->FindClass(CLS_POINT2D);

        //CONVERT TO GLOBAL REFERENCES
        this->cls_point2d = (jclass) env->NewGlobalRef(cls_point2d_local);

        //DELETE LOCAL REFERENCES (no longer used)
        env->DeleteLocalRef(cls_point2d_local);

        //retrieve constructors of classes used in this method
        this->method_constructor_point2d = env->GetMethodID(cls_point2d, METHOD_CONSTRUCTOR, "(FF)V");

        //retrieve argument-passed object fields
        this->field_jpoint2d_x = env->GetFieldID(cls_point2d, FIELD_X, SIGNATURE_FLOAT);
        this->field_jpoint2d_y = env->GetFieldID(cls_point2d, FIELD_Y, SIGNATURE_FLOAT);

        //insert neighbors
        int level = 1;
        for (int i = -level; i <= level; i++) {
            for (int j = -level; j <= level; j++) {
                if (i == -level || i == level || j == -level || j == level) {
                    Point2D point = Point2D(maxdepthsize * i, maxdepthsize * j);
                    float direction = atan2(point.y(), point.x());
                    this->neighbors_directions.push_back(direction);
                    this->neighbors2d[direction] = point;
                }
            }
        }
    }

    Point2D closestNeighborTo(float yaw){
        float yaw_adapted = closestOrientationTo(neighbors_directions, yaw);
        return neighbors2d[yaw_adapted];
    }

    bool checkCollision2D_Line(Point2D point1, Point2D point2){
        float resolution = this->octree->getResolution();
        float min_x = std::min(point1.x(), point2.x());
        float min_y = std::min(point1.y(), point2.y());
        float max_x = std::max(point1.x(), point2.x());
        float max_y = std::max(point1.y(), point2.y());
        //distance = positive infinity
        float distance = std::numeric_limits<float>::max();
        float diff_x = point2.x() - point1.x();
        float diff_y = point2.y() - point1.y();
        float hypot = hypotf(diff_y, diff_x);
        //iterate over occupied cells
        for(OcTree::leaf_bbx_iterator it=this->octree->begin_leafs_bbx(point3d(min_x, min_y, 0), point3d(max_x, max_y, 0)), end=this->octree->end_leafs_bbx(); it != end; ++it){
            //transform current leaf_bbx_iterator to node
            OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
            if( this->octree->isNodeOccupied(currentNode)){
                float new_distance = fabs(diff_y * it.getCoordinate().x() - diff_x * it.getCoordinate().y() + point2.x() * point1.y() - point2.y() * point1.x()) / hypot;
                distance = std::min(distance, new_distance);
                //collision detected
                if(distance <= this->radius_optimistic + it.getSize()){
                    return true;
                }
            }
        }
        return false;
    }

    bool checkCollision2d_Cached(point3d point){
        Point2D point_2d = Point2D(point);
        //not found, calculate and cache
        bool result;
        if(this->cache_collisions.find(point_2d) == this->cache_collisions.end()){
            result = checkCollision(point, this->radius_optimistic, this->octree);
            this->cache_collisions[point_2d] = result;
        }
        else{
            result = this->cache_collisions[point_2d];
        }
        return result;
    }

    ~StaticInformation2D() {

        env->DeleteGlobalRef(cls_point2d);

    }

};

bool isInBounds(double octree_min_x, double octree_min_y, double octree_max_x, double octree_max_y, point3d point){
    return point.x() > octree_min_x
           && point.x() < octree_max_x
           && point.y() > octree_min_y
           && point.y() < octree_max_y;
}

//other methods
void frontier_points2d(float size_cell, point3d center, point3d_priorityqueue &queue){
    float diff = size_cell / 4.0;
    queue.push(point3d(center.x() - diff, center.y() - diff, 0));
    queue.push(point3d(center.x() - diff, center.y() + diff, 0));
    queue.push(point3d(center.x() + diff, center.y() + diff, 0));
    queue.push(point3d(center.x() + diff, center.y() - diff, 0));
}


JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_hipster_H2DMRTransitionFunction_initialize
  (JNIEnv *env , jobject jh2dmrtransitiongenerator, jlong octree_pointer, jobject jadjacency_map, jfloat radius_optimistic, jfloat min_resolution_trajectories){
        StaticInformation2D *information = new StaticInformation2D(env, octree_pointer, jadjacency_map, radius_optimistic, min_resolution_trajectories);
        return (long) information;
}

JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_hipster_H2DMRTransitionFunction_transitionsFrom
  (JNIEnv *env, jobject jh2dmrtransitiongenerator, jobject jpoint2d){
    StaticInformation2D* information = (StaticInformation2D*) getPointer(env, jh2dmrtransitiongenerator);

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
    point3d center_of_current_cell = info.coordinate;
    point3d state_2D_compare = information->octree->keyToCoord(info.key, information->maxdepth);
    //know current adjacencies for this point
    jobject jcells = info.jadjacencies;
    jint jcells_size = env->CallIntMethod(jcells, information->method_size_arraylist);
    //variable to store the explored cells
    std::unordered_set<Point2D> points_considered;
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
        Point2D center_2d = Point2D(current_node_info.coordinate.x(), current_node_info.coordinate.y());
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
                    float yaw_neighbor = atan2(center.y() - center_of_current_cell.y(), center.x() - center_of_current_cell.x());
                    Point2D neighbor = information->closestNeighborTo(yaw_neighbor);
                    upCenter = Point2D(upCenter.x() + neighbor.x(), upCenter.y() + neighbor.y());
                }
                //if current cell is occupied, try subsampling
                point3d upCenter3d = point3d(upCenter.x(), upCenter.y(), 0);
                if(information->checkCollision2D_Line(state, upCenter3d)){
                    frontier_points2d(information->maxdepthsize, upCenter3d, queue_frontier_points);
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
                if(current == state || !isInBounds(octree_min_x, octree_min_y, octree_max_x, octree_max_y, current)) continue;
                if(!information->checkCollision2D_Line(state, current)){
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
            point3d_priorityqueue empty(comparator);
            queue_frontier_points.swap(empty);
        }
    }

    //return result
    return jarraylistneighbors;
}


JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_hipster_H2DMRTransitionFunction_dispose
  (JNIEnv *env, jobject jh2dmrtransitiongenerator){
    StaticInformation2D* information = (StaticInformation2D*) getPointer(env, jh2dmrtransitiongenerator);
    delete information;
}

