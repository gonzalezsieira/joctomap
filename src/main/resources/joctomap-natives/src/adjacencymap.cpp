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
#include <cmath>
#include <jni.h>
#include <octomap/OcTree.h>
#include "definitions.h"
#include "nativeobject.h"
#include "adjacencymap.h"

using namespace octomap;

JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_util_AdjacencyMap_initializeJNI
  (JNIEnv *env, jobject jadjacencymap){
    //error margin
    float EPSILON = 0.001f;
    //retrieve fields from adjacencymap
    jclass cls_jadjacencymap = env->FindClass(CLS_JADJACENCYMAP);
    jfieldID field_joctomap = env->GetFieldID(cls_jadjacencymap, FIELD_ADJACENCYMAP_OCTREE, CLS_JOCTREE);
    jfieldID field_nodes_info = env->GetFieldID(cls_jadjacencymap, FIELD_ADJACENCYMAP_NODESINFO, CLS_MAP);
    jfieldID field_adjacencies = env->GetFieldID(cls_jadjacencymap, FIELD_ADJACENCYMAP_ADJACENCIES, CLS_MAP);
    //retrieve objects from fields
    jobject joctree = env->GetObjectField(jadjacencymap, field_joctomap);
    jobject jnodes_info = env->GetObjectField(jadjacencymap, field_nodes_info);
    jobject jadjacencies = env->GetObjectField(jadjacencymap, field_adjacencies);
    //retrieve HashMap, JOctreeKey, Pair, Point3D class
    jclass cls_hashmap = env->FindClass(CLS_MAP);
    jclass cls_joctreekey = env->FindClass(CLS_JOCTREEKEY);
    jclass cls_pair = env->FindClass(CLS_PAIR);
    jclass cls_point3d = env->FindClass(CLS_POINT3D);
    jclass cls_float = env->FindClass(CLS_FLOAT);
    jclass cls_arraylist = env->FindClass(CLS_ARRAYLIST);
    //retrieve "put" method of HashMap, constructors of JOctreekey, Pair, Point3D
    jmethodID method_hashmap_put = env->GetMethodID(cls_hashmap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    jmethodID method_arraylist_add = env->GetMethodID(cls_arraylist, "add", "(Ljava/lang/Object;)Z");
    jmethodID constructor_pair = env->GetMethodID(cls_pair, METHOD_CONSTRUCTOR, "(Ljava/lang/Object;Ljava/lang/Object;)V");
    jmethodID constructor_point3d = env->GetMethodID(cls_point3d, METHOD_CONSTRUCTOR, "(FFF)V");
    jmethodID constructor_joctreekey = env->GetMethodID(cls_joctreekey, METHOD_CONSTRUCTOR, "(III)V");
    jmethodID constructor_float = env->GetMethodID(cls_float, METHOD_CONSTRUCTOR, "(F)V");
    jmethodID constructor_arraylist = env->GetMethodID(cls_arraylist, METHOD_CONSTRUCTOR, "()V");
    
    //retrieve octree from object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    //iterate over the cells of the octree twice to generate distances in pairs
    double minx, miny, minz, maxx, maxy, maxz;
    octree->getMetricMin(minx, miny, minz);
    octree->getMetricMax(maxx, maxy, maxz);
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(point3d(minx, miny, minz), point3d(maxx, maxy, maxz)), end=octree->end_leafs_bbx(); it!= end; ++it){
        //obtain coordinate, size and ke of the node
        float size = static_cast<float>(it.getSize());
        OcTreeKey key = it.getKey();
        point3d coordinate = it.getCoordinate();
        //build object "joctreekey"
        jobject joctreekey = env->NewObject(cls_joctreekey, constructor_joctreekey, key.k[0], key.k[1], key.k[2]);
        //build object "float"
        jobject jfloat = env->NewObject(cls_float, constructor_float, size);
        //build object "point3d"
        jobject jpoint3d = env->NewObject(cls_point3d, constructor_point3d, coordinate.x(), coordinate.y(), coordinate.z());
        //build object "pair<float, point3d>
        jobject jpair = env->NewObject(cls_pair, constructor_pair, jfloat, jpoint3d);
        //put into "nodesinfo" field the variables above
        env->CallObjectMethod(jnodes_info, method_hashmap_put, joctreekey, jpair);
        //create new arraylist
        jobject jarraylist = env->NewObject(cls_arraylist, constructor_arraylist);
        //calculate size of voxel
        float diff = static_cast<float>(octree->getResolution()) + size;
        //iterate again the cells
        for(OcTree::leaf_bbx_iterator it2 = octree->begin_leafs_bbx(point3d(coordinate.x() - diff, coordinate.y() - diff, coordinate.z() - diff), point3d(coordinate.x() + diff, coordinate.y() + diff, coordinate.z() + diff)), end2=octree->end_leafs_bbx(); it2!= end2; ++it2){
            //get coordinate, key and size of the second cell
            point3d coordinate2 = it2.getCoordinate();
            OcTreeKey key2 = it2.getKey();
            //calculate (sum of sizes of both cells / 2.0f)
            float size_added = size/2.0f + static_cast<float>(it2.getSize())/2.0f;
            //calculate if cells are adjacent
            if(key != key2 
                    && std::abs(coordinate.x() - coordinate2.x()) - size_added <= EPSILON
                    && std::abs(coordinate.y() - coordinate2.y()) - size_added <= EPSILON
                    && std::abs(coordinate.z() - coordinate2.z()) - size_added <= EPSILON
            ){
                //build object "joctreekey"
                jobject joctreekey2 = env->NewObject(cls_joctreekey, constructor_joctreekey, key2.k[0], key2.k[1], key2.k[2]);
                //add cell as adjacent
                env->CallBooleanMethod(jarraylist, method_arraylist_add, joctreekey2);
                //release local ref
                env->DeleteLocalRef(joctreekey2);
            }
        }
        //put into "adjacencies" a new instance of arraylist
        env->CallObjectMethod(jadjacencies, method_hashmap_put, joctreekey, jarraylist);
        //delete local references used in this loop
        env->DeleteLocalRef(jarraylist);
        env->DeleteLocalRef(jpair);
        env->DeleteLocalRef(jpoint3d);
        env->DeleteLocalRef(jfloat);
        env->DeleteLocalRef(joctreekey);
    }
    //delete local reference used by jclass
    env->DeleteLocalRef(cls_arraylist);
    env->DeleteLocalRef(cls_float);
    env->DeleteLocalRef(cls_point3d);
    env->DeleteLocalRef(cls_pair);
    env->DeleteLocalRef(cls_joctreekey);
    env->DeleteLocalRef(cls_hashmap);
    env->DeleteLocalRef(cls_jadjacencymap);
    //delete local references used by jobjects from fields
    env->DeleteLocalRef(jadjacencies);
    env->DeleteLocalRef(jnodes_info);
    env->DeleteLocalRef(jadjacencies);
}