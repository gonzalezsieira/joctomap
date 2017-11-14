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
#include <iostream>
#include <jni.h>
#include <octomap/OcTree.h>
#include <octomap/OcTreeKey.h>
#include <octomap/OccupancyOcTreeBase.h>
#include <octomap/OcTreeNode.h>
#include <float.h>
#include <queue>
#include <math.h>
#include <limits>
#include <unordered_set>
#include "definitions.h"
#include "collisions.h"
#include "nativeobject.h"
#include "joctreeutils.h"

using namespace std;
using namespace octomap;

int POINTS_CONSIDERED = 2;

/*
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    getOccupiedKeysBetween
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point;Les/usc/citius/lab/motionplanner/core/spatial/Point;)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_getOccupiedKeysBetween
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpointmin, jobject jpointmax){
    //get fields of Point3D object
    jclass cls_point = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point");
    jmethodID methodGetX = env->GetMethodID(cls_point, "getX", "()F");
    jmethodID methodGetY = env->GetMethodID(cls_point, "getY", "()F");
    jmethodID methodGetZ = env->GetMethodID(cls_point, "getZ", "()F");
    
    //get values of the Point3D objects
    float pointminX = env->CallFloatMethod(jpointmin, methodGetX);
    float pointminY = env->CallFloatMethod(jpointmin, methodGetY);
    float pointminZ = env->CallFloatMethod(jpointmin, methodGetZ);
    float pointmaxX = env->CallFloatMethod(jpointmax, methodGetX);
    float pointmaxY = env->CallFloatMethod(jpointmax, methodGetY);
    float pointmaxZ = env->CallFloatMethod(jpointmax, methodGetZ);
    
    //define min/max points to iterate
    point3d min = point3d(pointminX, pointminY, pointminZ);
    point3d max = point3d(pointmaxX, pointmaxY, pointmaxZ);
    
    //find class and constructor to instantiate JOCtreekey
    jclass clsKey = env->FindClass("Les/usc/citius/lab/joctomap/octree/JOctreeKey;");
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
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    getOccupiedCoordinatesBetween
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point;Les/usc/citius/lab/motionplanner/core/spatial/Point;)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_getOccupiedCoordinatesBetween
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpointmin, jobject jpointmax){
    //get fields of Point3D object
    jclass cls_point3d = env->FindClass("Les/usc/citius/lab/motionplanner/core/spatial/Point3D;");
    jclass cls_point = env->FindClass("Les/usc/citius/lab/motionplanner/core/spatial/Point;");
    //find Pair class
    jclass clsCell = env->FindClass("Les/usc/citius/lab/joctomap/octree/Cell;");
    //find ArrayList class
    jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
    //retrieve Point3D constructor
    jmethodID constructorPoint3D = env->GetMethodID(cls_point3d, "<init>", "(FFF)V");
    //retrieve Pair constructor
    jmethodID constructorCell = env->GetMethodID(clsCell, "<init>", "(Les/usc/citius/lab/motionplanner/core/spatial/Point3D;F)V");
    //retrieve ArrayList constructor
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    //retrieve ArrayList add method
    jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    //get method ID
    jmethodID methodGetX = env->GetMethodID(cls_point, "getX", "()F");
    jmethodID methodGetY = env->GetMethodID(cls_point, "getY", "()F");
    jmethodID methodGetZ = env->GetMethodID(cls_point, "getZ", "()F");

    //get values of the Point3D objects
    float pointminX = env->CallFloatMethod(jpointmin, methodGetX);
    float pointminY = env->CallFloatMethod(jpointmin, methodGetY);
    float pointminZ = env->CallFloatMethod(jpointmin, methodGetZ);
    float pointmaxX = env->CallFloatMethod(jpointmax, methodGetX);
    float pointmaxY = env->CallFloatMethod(jpointmax, methodGetY);
    float pointmaxZ = env->CallFloatMethod(jpointmax, methodGetZ);

    //define min/max points to iterate
    point3d min = point3d(pointminX, pointminY, pointminZ);
    point3d max = point3d(pointmaxX, pointmaxY, pointmaxZ);


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
            jobject currentPoint3D = env->NewObject(cls_point3d, constructorPoint3D, static_cast<float>(coordinate.x()), static_cast<float>(coordinate.y()), static_cast<float>(coordinate.z()));
            jobject currentPair = env->NewObject(clsCell, constructorCell, currentPoint3D, static_cast<float>(it.getSize()));
            //add object to the ArrayList
            env->CallBooleanMethod(arrayListObject, addArrayList, currentPair);
        }
    }
    return arrayListObject;
}


/*
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    resolutionAt
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point;)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_resolutionAt
  (JNIEnv *env, jclass cls, jobject jtree, jobject jpoint){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get fields of Point3D object
    jclass cls_point = env->FindClass("Les/usc/citius/lab/motionplanner/core/spatial/Point;");
    //get method ID
    jmethodID methodGetX = env->GetMethodID(cls_point, "getX", "()F");
    jmethodID methodGetY = env->GetMethodID(cls_point, "getY", "()F");
    jmethodID methodGetZ = env->GetMethodID(cls_point, "getZ", "()F");
    //get values of the Point3D objects
    float pointX = env->CallFloatMethod(jpoint, methodGetX);
    float pointY = env->CallFloatMethod(jpoint, methodGetY);
    float pointZ = env->CallFloatMethod(jpoint, methodGetZ);
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
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point;Les/usc/citius/lab/motionplanner/core/spatial/Point;)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_resolutionAddedIn
  (JNIEnv *env, jclass cls, jobject jtree, jobject jpoint1, jobject jpoint2){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get fields of Point3D object
    jclass cls_point = env->FindClass("Les/usc/citius/lab/motionplanner/core/spatial/Point;");
    //get method ID
    jmethodID methodGetX = env->GetMethodID(cls_point, "getX", "()F");
    jmethodID methodGetY = env->GetMethodID(cls_point, "getY", "()F");
    jmethodID methodGetZ = env->GetMethodID(cls_point, "getZ", "()F");
    //get values of the Point3D objects
    float point1X = env->CallFloatMethod(jpoint1, methodGetX);
    float point1Y = env->CallFloatMethod(jpoint1, methodGetY);
    float point1Z = env->CallFloatMethod(jpoint1, methodGetZ);
    float point2X = env->CallFloatMethod(jpoint2, methodGetX);
    float point2Y = env->CallFloatMethod(jpoint2, methodGetY);
    float point2Z = env->CallFloatMethod(jpoint2, methodGetZ);
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