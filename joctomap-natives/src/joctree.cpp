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
#include <fstream>
#include <sstream>
#include <jni.h>
#include <octomap/OcTree.h>
#include <octomap/AbstractOcTree.h>
#include <octomap/OccupancyOcTreeBase.h>
#include "joctree.h"
#include "nativeobject.h"
#include "leafbbxiterator.h"
#include "iteratorinformation.h"
#include "definitions.h"

using namespace std;
using namespace octomap;

JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_dispose
  (JNIEnv *env, jobject jtree){
        //get pointer
        long pointer = getPointer(env, jtree);
        //if not released yet
        if(pointer != INVALID_POINTER){
            //recover octree
            OcTree *object = (OcTree*) pointer;
            //delete object
            delete object;
            //delete pointer
            deletePointer(env, jtree);
        }
}

/*
 * This method finds the position of a cell given a real position in the map (x, y, z)
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__FFF
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	OcTreeKey key = octree->coordToKey(x, y, z);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/*
 * This method finds the position of a cell given a real position in the map (x, y, z) at
 * a given depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__FFFI
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	OcTreeKey key = octree->coordToKey(x, y, z, depth);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(III)V");
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/**
 * This method adjusts the key of a node at a givn depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_adjustKeyAtDepth
  (JNIEnv *env, jobject jtree, jobject jkey, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field IDs
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jfieldID fieldX = env->GetFieldID(cls, FIELD_X, SIGNATURE_INT);
	jfieldID fieldY = env->GetFieldID(cls, FIELD_Y, SIGNATURE_INT);
	jfieldID fieldZ = env->GetFieldID(cls, FIELD_Z, SIGNATURE_INT);
	//recover field values
	int x = env->GetIntField(jkey, fieldX);
	int y = env->GetIntField(jkey, fieldY);
	int z = env->GetIntField(jkey, fieldZ);
	//adjust key to the depth specified
	OcTreeKey key = octree->adjustKeyAtDepth(OcTreeKey(x, y, z), depth);
	//find constructor to instantiate JOCtreekey
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/**
 * Retrieves the key of a given coordinate.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2
  (JNIEnv *env, jobject jtree, jobject jcoord){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(III)V");
	//find class and fields of point3d
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jfieldID fieldX = env->GetFieldID(clspoint, FIELD_X, SIGNATURE_FLOAT);
	jfieldID fieldY = env->GetFieldID(clspoint, FIELD_Y, SIGNATURE_FLOAT);
	jfieldID fieldZ = env->GetFieldID(clspoint, FIELD_Z, SIGNATURE_FLOAT);
	//retrieve values of the fields
	jfloat x = env->GetFloatField(jcoord, fieldX);
	jfloat y = env->GetFloatField(jcoord, fieldY);
	jfloat z = env->GetFloatField(jcoord, fieldZ);
	OcTreeKey key = octree->coordToKey(point3d(x, y, z));
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/**
 * Retrieves the key at a given depth, given a coordinate.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2I
  (JNIEnv *env, jobject jtree, jobject jcoord, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(III)V");
	//find class and fields of point3d
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jfieldID fieldX = env->GetFieldID(clspoint, FIELD_X, SIGNATURE_FLOAT);
	jfieldID fieldY = env->GetFieldID(clspoint, FIELD_Y, SIGNATURE_FLOAT);
	jfieldID fieldZ = env->GetFieldID(clspoint, FIELD_Z, SIGNATURE_FLOAT);
	//retrieve values of the fields
	jfloat x = env->GetFloatField(jcoord, fieldX);
	jfloat y = env->GetFloatField(jcoord, fieldY);
	jfloat z = env->GetFloatField(jcoord, fieldZ);
	OcTreeKey key = octree->coordToKey(point3d(x, y, z), depth);
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/**
 * Retrieves the coordinate of a given key.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_keyToCoord__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2
  (JNIEnv *env, jobject jtree, jobject jkey){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover key field IDs
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jfieldID fieldX = env->GetFieldID(cls, FIELD_X, SIGNATURE_INT);
	jfieldID fieldY = env->GetFieldID(cls, FIELD_Y, SIGNATURE_INT);
	jfieldID fieldZ = env->GetFieldID(cls, FIELD_Z, SIGNATURE_INT);
	//recover key field values
	int kx = env->GetIntField(jkey, fieldX);
	int ky = env->GetIntField(jkey, fieldY);
	int kz = env->GetIntField(jkey, fieldZ);
	//recover class of the point3d and its constructor, to return the instance
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//recover coordinate of the key
	point3d coord = octree->keyToCoord(OcTreeKey(kx, ky, kz));
	//return new object
	return env->NewObject(clspoint, constructor, static_cast<float>(coord.x()), static_cast<float>(coord.y()), static_cast<float>(coord.z()));
}

/**
 * Retrieves the coordinate of a given key and depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_keyToCoord__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *env, jobject jtree, jobject jkey, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover key field IDs
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jfieldID fieldX = env->GetFieldID(cls, FIELD_X, SIGNATURE_INT);
	jfieldID fieldY = env->GetFieldID(cls, FIELD_Y, SIGNATURE_INT);
	jfieldID fieldZ = env->GetFieldID(cls, FIELD_Z, SIGNATURE_INT);
	//recover key field values
	int kx = env->GetIntField(jkey, fieldX);
	int ky = env->GetIntField(jkey, fieldY);
	int kz = env->GetIntField(jkey, fieldZ);
	//recover class of the point3d and its constructor, to return the instance
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//recover coordinate of the key
	point3d coord = octree->keyToCoord(OcTreeKey(kx, ky, kz), depth);
	//return new object
	return env->NewObject(clspoint, constructor, static_cast<float>(coord.x()), static_cast<float>(coord.y()), static_cast<float>(coord.z()));
}

/**
 * This method searches in the octree a node given by a key and a depth (depth = 0 means search
 * in the whole octree).
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_search__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *env, jobject jtree, jobject jkey, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover key field IDs
	jclass cls = env->FindClass(CLS_JOCTREEKEY);
	jfieldID fieldX = env->GetFieldID(cls, FIELD_X, SIGNATURE_INT);
	jfieldID fieldY = env->GetFieldID(cls, FIELD_Y, SIGNATURE_INT);
	jfieldID fieldZ = env->GetFieldID(cls, FIELD_Z, SIGNATURE_INT);
	//recover field values
	int x = env->GetIntField(jkey, fieldX);
	int y = env->GetIntField(jkey, fieldY);
	int z = env->GetIntField(jkey, fieldZ);
	//adjust key to the depth specified
	OcTreeKey key = OcTreeKey(x, y, z);
	//find node in the octree
	OcTreeNode *node = octree->search(key, depth);
	//return new node instance if the node is found in the octree
	if(node != NULL){
		//find class and constructor to instantiate joctreenode
		cls = env->FindClass(CLS_JOCTREENODE);
		jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(J)V");
		//return new instance
		return env->NewObject(cls, constructor, node);
	}
	//return null if the node is not found
	else{
		return NULL;
	}
}

/**
 * This method searches in the octree a node given by a 3D position and a depth ()
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_search__FFFI
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//search node in the octree
	OcTreeNode *node = octree->search(x, y, z, depth);
	//return new node instance if the node is found in the octree
	if(node != NULL){
		//find class and constructor to instantiate joctreenode
		jclass cls = env->FindClass(CLS_JOCTREENODE);
		jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(J)V");
		//return new instance
		return env->NewObject(cls, constructor, node);
	}
	//return null if the node is not found
	else{
		return NULL;
	}
}

/**
 * Retrieves if a node is occupied according to its probability of occupancy.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_isNodeOccupied
  (JNIEnv *env, jobject jtree, jobject jnode){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover node
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//return query
	return octree->isNodeOccupied(node);
}

/**
 * This method updates the occupancy information of a node in the position (x, y, z).
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_updateNode
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z, jboolean occupied){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//update information of the node
	OcTreeNode *node = octree->updateNode(x, y, z, (bool) occupied, false);
	//find class and constructor to instantiate joctreenode
	jclass cls = env->FindClass(CLS_JOCTREENODE);
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(J)V");
	//return new instance
	return env->NewObject(cls, constructor, node);
}

/**
 * Enables/disables the usage of the BBX to limit the octree updates.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_useBBXLimit
  (JNIEnv *env, jobject jtree, jboolean value){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//set value
	octree->useBBXLimit(value);
}

/**
 * Updates the min/max points of the BBX.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_setBBX
  (JNIEnv *env, jobject jtree, jobject minPoint, jobject maxPoint){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field information of the class
	jclass clsPoint = env->FindClass(CLS_POINT3D);
	jfieldID fieldx = env->GetFieldID(clsPoint, FIELD_X, SIGNATURE_FLOAT);
	jfieldID fieldy = env->GetFieldID(clsPoint, FIELD_Y, SIGNATURE_FLOAT);
	jfieldID fieldz = env->GetFieldID(clsPoint, FIELD_Z, SIGNATURE_FLOAT);
	//recover information of point3d: min
	jfloat minx = env->GetFloatField(minPoint, fieldx);
	jfloat miny = env->GetFloatField(minPoint, fieldy);
	jfloat minz = env->GetFloatField(minPoint, fieldz);
	//recover information of point3d: max
	jfloat maxx = env->GetFloatField(maxPoint, fieldx);
	jfloat maxy = env->GetFloatField(maxPoint, fieldy);
	jfloat maxz = env->GetFloatField(maxPoint, fieldz);
	point3d min = point3d(minx, miny, minz);
	point3d max = point3d(maxx, maxy, maxz);
	octree->setBBXMin(min);
	octree->setBBXMax(max);
}

/**
 * Updates the min point of the BBX.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_setBBXMin
  (JNIEnv *env, jobject jtree, jobject minPoint){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field information of the class
	jclass clsPoint = env->FindClass(CLS_POINT3D);
	jfieldID fieldx = env->GetFieldID(clsPoint, FIELD_X, SIGNATURE_FLOAT);
	jfieldID fieldy = env->GetFieldID(clsPoint, FIELD_Y, SIGNATURE_FLOAT);
	jfieldID fieldz = env->GetFieldID(clsPoint, FIELD_Z, SIGNATURE_FLOAT);
	//recover information of point3d: min
	jfloat minx = env->GetFloatField(minPoint, fieldx);
	jfloat miny = env->GetFloatField(minPoint, fieldy);
	jfloat minz = env->GetFloatField(minPoint, fieldz);
	point3d min = point3d(minx, miny, minz);
	octree->setBBXMin(min);
}

/**
 * Updates the max point of the BBX.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_setBBXMax
  (JNIEnv *env, jobject jtree, jobject maxPoint){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field information of the class
	jclass clsPoint = env->FindClass(CLS_POINT3D);
	jfieldID fieldx = env->GetFieldID(clsPoint, FIELD_X, SIGNATURE_FLOAT);
	jfieldID fieldy = env->GetFieldID(clsPoint, FIELD_Y, SIGNATURE_FLOAT);
	jfieldID fieldz = env->GetFieldID(clsPoint, FIELD_Z, SIGNATURE_FLOAT);
	//recover information of point3d: max
	jfloat maxx = env->GetFloatField(maxPoint, fieldx);
	jfloat maxy = env->GetFloatField(maxPoint, fieldy);
	jfloat maxz = env->GetFloatField(maxPoint, fieldz);
	point3d max = point3d(maxx, maxy, maxz);
	octree->setBBXMax(max);
}

/*
 * Method that obtains the number of leafs in the octree.
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_size
  (JNIEnv *env, jobject jtree){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    double minx, miny, minz, maxx, maxy, maxz;
    //query for min/max points
    octree->getMetricMin(minx, miny, minz);
    octree->getMetricMax(maxx, maxy, maxz);
    //count number of iterations
    int nodes = 0;
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(point3d(minx, miny, minz), point3d(maxx, maxy, maxz), 0), 
            end = octree->end_leafs_bbx(); 
            it != end; 
            ++it){
        nodes++;
    }
    return nodes;
}

/**
 * Method that obtains the octree maximum depth.
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getTreeDepth
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return tree depth
	return static_cast<int>(octree->getTreeDepth());
}

/**
 * Method that obtains the octree minimum resolution.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getResolution
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return resolution
	return static_cast<float>(octree->getResolution());
}

/**
 * Method that obtains the resolution at a current depth.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getNodeSize
  (JNIEnv *env, jobject jtree, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return node size ath the given depth
	return static_cast<float>(octree->getNodeSize(static_cast<unsigned int>(depth)));
}

/**
 * Method that obtains the center of the BBX, if set. NULL otherwise.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getBBXCenter
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//get value
	point3d point = octree->getBBXCenter();
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//new Point3D
	return env->NewObject(clspoint, constructor, static_cast<float>(point.x()), static_cast<float>(point.y()), static_cast<float>(point.z()));
}

/**
 * Method that obtains the min position of the BBX, if set. NULL otherwise.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getBBXMin
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//get value
	point3d min = octree->getBBXMin();
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//new Point3D
	return env->NewObject(clspoint, constructor, static_cast<float>(min.x()), static_cast<float>(min.y()), static_cast<float>(min.z()));
}

/**
 * Method that obtains the max position of the BBX, if set. NULL otherwise.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getBBXMax
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//get value
	point3d max = octree->getBBXMax();
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//new Point3D
	return env->NewObject(clspoint, constructor, static_cast<float>(max.x()), static_cast<float>(max.y()), static_cast<float>(max.z()));
}

/**
 * Method that retrieves the min position of the known space.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getMetricMin
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	double x, y, z;
	//get value
	octree->getMetricMin(x, y, z);
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//new Point3D
	return env->NewObject(clspoint, constructor, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

/**
 * Method that retrieves the max position of the known space.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getMetricMax
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	double x, y, z;
	//get value
	octree->getMetricMax(x, y, z);
	jclass clspoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clspoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//new Point3D
	return env->NewObject(clspoint, constructor, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

/**
 * Method that retrieves the size of the known space in each of the 3 dimensions.
 */
JNIEXPORT jfloatArray JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getMetricSize
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	double x, y, z;
	octree->getMetricSize(x, y, z);
	float xFloat, yFloat, zFloat;
	xFloat = static_cast<float>(x);
	yFloat = static_cast<float>(y);
	zFloat = static_cast<float>(z);
	//create double[3]
	jfloatArray array = env->NewFloatArray(3);
	//set double[i]
	env->SetFloatArrayRegion(array, 0, 1, &xFloat);
	env->SetFloatArrayRegion(array, 1, 1, &yFloat);
	env->SetFloatArrayRegion(array, 2, 1, &zFloat);
	//return values
	return array;
}

/**
 * Returns if a BBX is being applied to the octree.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_isBBXSet
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return if min != max
	return !(octree->getBBXMin() == octree->getBBXMax());
}

/**
 * Retuns if the BBX is set, that is: getBBXMax() != getBBXMin()
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_isBBXApplied
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return if the BBX is being applied to update the octree
	return octree->bbxSet();
}

/**
 * Method that reatrieves a leaf_bbx_iterator for this octree (given a min and max position and depth)
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_leafBBXIterator__Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2I
  (JNIEnv *env, jobject jtree, jobject minPoint, jobject maxPoint, jint maxDepth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field information of the class
	jclass clsPoint = env->FindClass(CLS_POINT3D);
	jfieldID fieldx = env->GetFieldID(clsPoint, FIELD_X, SIGNATURE_FLOAT);
	jfieldID fieldy = env->GetFieldID(clsPoint, FIELD_Y, SIGNATURE_FLOAT);
	jfieldID fieldz = env->GetFieldID(clsPoint, FIELD_Z, SIGNATURE_FLOAT);
	//recover information of point3d: min
	jfloat minx = env->GetFloatField(minPoint, fieldx);
	jfloat miny = env->GetFloatField(minPoint, fieldy);
	jfloat minz = env->GetFloatField(minPoint, fieldz);
	//recover information of point3d: max
	jfloat maxx = env->GetFloatField(maxPoint, fieldx);
	jfloat maxy = env->GetFloatField(maxPoint, fieldy);
	jfloat maxz = env->GetFloatField(maxPoint, fieldz);
	//recover iterator from the octree
	OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(point3d(minx, miny, minz), point3d(maxx, maxy, maxz), maxDepth);
	OcTree::leaf_bbx_iterator it2 = octree->end_leafs_bbx();
	LeafBBXIteratorInformation *info = new LeafBBXIteratorInformation(it, it2);
	//instantiate iterator
	jclass clsIterator = env->FindClass(CLS_LEAFBBXITERATOR);
	jmethodID constructorID = env->GetMethodID(clsIterator, METHOD_CONSTRUCTOR, "(J)V");
	return env->NewObject(clsIterator, constructorID, info);
}

/**
 * Method that reatrieves a leaf_bbx_iterator for this octree (given a min and max node key and depth)
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_leafBBXIterator__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *env, jobject jtree, jobject jminKey, jobject jmaxKey, jint maxDepth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover octree keys
	OcTreeKey *minKey = (OcTreeKey*) getPointer(env, jminKey);
	OcTreeKey *maxKey = (OcTreeKey*) getPointer(env, jmaxKey);
	//recover iterator from the octree
	OcTree::leaf_bbx_iterator *it = new OcTree::leaf_bbx_iterator(octree, *minKey, *maxKey, maxDepth);
	OcTree::leaf_bbx_iterator *it2 = new OcTree::leaf_bbx_iterator(octree->end_leafs_bbx());
	//instantiate iterator
	jclass cls = env->FindClass(CLS_LEAFBBXITERATOR);
	jmethodID constructorID = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(JJ)V");
	return env->NewObject(cls, constructorID, it, it2);
}

/**
 * This method writes an octree, given the pointer to the object and the
 * filename.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_write
  (JNIEnv *env, jobject obj, jstring filename){
	OcTree *octree = (OcTree*) getPointer(env, obj);
	//convert jstring into native char*
	jboolean iscopy = false;
	const char *nativeFilename = env->GetStringUTFChars(filename, &iscopy);
	//write octree in filename
	bool value = octree->write(nativeFilename);
	//release memory of native char*
	env->ReleaseStringUTFChars(filename, nativeFilename);
	//return obtained value
	return (jboolean) value;
}

/**
 * Reads an octree from a ".bt" or ".ot" file and returns the pointer to the
 * created Octree object.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_read
  (JNIEnv *env, jclass cls, jstring filename){
	//find constructor to instantiate the result
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(J)V");
	//convert jstring into native char*
	jboolean iscopy = false;
	/*
	 * This workaround to read an Octree comes from an error using
	 * AbstractOctree::read(filename). The library is unable
	 * to return an Octree of the type specified in the file (.ot), so
	 * the implementation of Octree::readData(stream) must be used to
	 * avoid using the factory above./
	 */
	//a new octree is initialized with a default resolution
	OcTree *ot = new OcTree(0.1);
	//convert jstring to char*
	const char *nativeFilename = env->GetStringUTFChars(filename, &iscopy);
	//open input stream to read data
	ifstream stream; stream.open(nativeFilename);
	//release memory of jstring
	env->ReleaseStringUTFChars(filename, nativeFilename);
	//array to store the current line of the file
	char line[250];
	bool stop = false;
	//read the first lines of the file that start with "#"
	while(!stop){
		//read current line
		stream.getline(line, 256);
		string line_string = line;
		//modify the octree resolution when the "res" line is read
		if(line_string.substr(0, 3).compare("res") == 0){
			//read resolution using istringstream (to avoid locale conflicts -
			//different symbols to separate decimals.
			double res; istringstream istr(line_string.substr(4).c_str());
			istr >> res;
			//modify octree resolution
			ot->setResolution(res);
		}
		//stop when the "data" line is read
		if(line_string.compare("data") == 0){
			stop = true;
		}
	}
	//read data in the current location of the input stream
	ot->readData(stream);
	//close input stream
	stream.close();
        //create joctree object
	jobject jtree =  env->NewObject(cls, constructor, ot);
        return jtree;
}

/**
 * Instantiates a new empty octree with the given resolution.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_create
  (JNIEnv *env, jclass cls, jfloat res){
	//find constructor to instantiate the result
	jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(J)V");
	//instantiate new empty octree
	OcTree *octree = new OcTree(res);
	//return new java instance of the octree
	return env->NewObject(cls, constructor, octree);
}

JNIEXPORT jstring JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getTreeType
        (JNIEnv *env, jobject jtree){
    //recover pointer
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get classname
    string classname = octree->getTreeType();
    //convert to jstring
    const char * charclassname = classname.c_str();
    jstring jclassname = env->NewStringUTF(charclassname);
    //result
    return jclassname;
}

/*
 * Prunes all the nodes of the tree which has the same value to higher resolution nodes.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_prune
  (JNIEnv *env, jobject jtree){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //call to prune() of the octree
    octree->prune();
}

/*
 * Opposite method to prune(), expands all the nodes to have the minimum resolution.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_expand
  (JNIEnv *env, jobject jtree){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //call method expand() of the octree
    octree->expand();
}

/**
 * Updates the occupancy of inner nodes to reflect the children's occupancy.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_updateInnerOccupancy
  (JNIEnv *env, jobject jtree){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //call method expand() of the octree
    octree->updateInnerOccupancy();
}

/*
 * Enables changes detection.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_enableChangeDetection
  (JNIEnv *env, jobject jtree, jboolean enable){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//set flag
	octree->enableChangeDetection(enable);
  }

/*
 * Resets changes detection.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_resetChangeDetection
  (JNIEnv *env, jobject jtree){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
	//reset
	octree->resetChangeDetection();
  }

/*
 * Creates a list with changes since last reset.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_keysChanged
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//create list to store changed keys
	//find ArrayList class
	jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
	//retrieve ArrayList constructor
	jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
	//retrieve ArrayList add method
	jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
	//new arraylist()
	jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
	//iterate over changed keys
	for(KeyBoolMap::const_iterator it = octree->changedKeysBegin(); it != octree->changedKeysEnd(); ++it){
		//find class and constructor to instantiate JOCtreekey
		jclass cls = env->FindClass(CLS_JOCTREEKEY);
		jmethodID constructor = env->GetMethodID(cls, METHOD_CONSTRUCTOR, "(III)V");
		//new JOctreeKey(x, y, z)
		jobject joctreekey = env->NewObject(cls, constructor, static_cast<int>(it->first.k[0]), static_cast<int>(it->first.k[1]), static_cast<int>(it->first.k[2]));
		//add object to the ArrayList
		env->CallBooleanMethod(arrayListObject, addArrayList, joctreekey);
	}
	//return list
	return arrayListObject;
  }
