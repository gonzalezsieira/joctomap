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
#include "transitionfunctionutils.h"

point3d jpoint3d_to_point3D(StaticInformation* information, jobject jpoint3d){
    //retrieve argument-passed object field values
    jfloat point3d_x = information->env->GetFloatField(jpoint3d, information->field_point3d_x);
    jfloat point3d_y = information->env->GetFloatField(jpoint3d, information->field_point3d_y);
    jfloat point3d_z = information->env->GetFloatField(jpoint3d, information->field_point3d_z);
    //return value
    return point3d(point3d_x, point3d_y, point3d_z);
}

NodeInfo node_info(StaticInformation* information, OcTreeKey key, jobject jkey){
    //get node info from java object
    jobject jnode_info = information->env->CallObjectMethod(information->jadjacencymap, information->method_nodeinfo, jkey);
    //get key value
    jobject size_float = information->env->GetObjectField(jnode_info, information->field_pair_first);
    //get Float value
    jfloat size = information->env->GetFloatField(size_float, information->field_float_value);
    //get coordinate value
    jobject jpoint3d = information->env->GetObjectField(jnode_info, information->field_pair_second);
    //transform point3d from java to C++
    point3d point2d = jpoint3d_to_point3D(information, jpoint3d);
    //convert float to
    return NodeInfo(key, size, jkey, point2d);
}

NodeInfo_Adjacencies search_node(StaticInformation* information, point3d point){
    //get max depth
    int depth = information->octree->getTreeDepth() + 1;
    OcTreeKey key;
    jobject jkey, jadjacencies;
    //this loop breaks when a key in the adjacency map is found (means that it exists in the tree)
    do{
        //find
        depth--;
        key = information->octree->coordToKey(point, depth);
        //instantiate key in java
        jkey = information->env->NewObject(information->cls_joctreekey, information->method_constructor_joctreekey, key.k[0], key.k[1], key.k[2]);
        jadjacencies = information->env->CallObjectMethod(information->jadjacencymap, information->method_adjacency, jkey);
    } while(depth > 0 && jadjacencies == NULL);
    return NodeInfo_Adjacencies( key, information->octree->getNodeSize(depth), jkey, information->octree->keyToCoord(key, depth), jadjacencies);

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

float_priorityqueue closestOrientations(std::list<float> neighbors_orientations, float value){

    //create priority queue (float)
    float_priorityqueue ordered_values = float_priorityqueue(CompareValues(value));

    for(std::list<float>::iterator list_iterator = neighbors_orientations.begin(); list_iterator != neighbors_orientations.end(); list_iterator++){
        ordered_values.push(*list_iterator);
    }
    return ordered_values;

}