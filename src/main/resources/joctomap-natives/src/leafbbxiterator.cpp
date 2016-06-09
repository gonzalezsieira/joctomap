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
#include <octomap/OcTree.h>
#include "leafbbxiterator.h"
#include "iteratorinformation.h"
#include "nativeobject.h"
#include "definitions.h"

using namespace std;
using namespace octomap;

/*
 * Frees memory used by this object. Called when finalize() in jvm.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_dispose
  (JNIEnv *env, jobject jiterator){
        //get pointer
        long pointer = getPointer(env, jiterator);
        //if not released yet
        if(pointer != INVALID_POINTER){
            //recover octree
            LeafBBXIteratorInformation *object = (LeafBBXIteratorInformation*) pointer;
            //delete object
            delete object;
            //delete pointer
            deletePointer(env, jiterator);
        }
}

/**
 * Queries if this iterator has reached its end point.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_hasNext
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	//retrieve hasNext
	return information->hasNext();
}

/**
 * Retrieves the next element for this iterator. Does not check hasNext(), that must be performed
 * manually before this method.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_next
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	//retrieve current key
	OcTreeKey key = information->next().getKey();
	//find class and constructor to instantiate JOCtreekey
	jclass clsKey = env->FindClass(CLS_JOCTREEKEY);
	jmethodID constructor = env->GetMethodID(clsKey, METHOD_CONSTRUCTOR, "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(clsKey, constructor, (int) key.k[0], (int) key.k[1], (int) key.k[2]);
}

/**
 * Returns the key of the current node.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_key
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	//retrieve current key
	OcTreeKey key = information->current().getKey();
	//find class and constructor to instantiate JOCtreekey
	jclass clsKey = env->FindClass(CLS_JOCTREEKEY);
	jmethodID constructor = env->GetMethodID(clsKey, METHOD_CONSTRUCTOR, "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(clsKey, constructor, (int) key.k[0], (int) key.k[1], (int) key.k[2]);
}

/**
 * Returns the depth of the current node.
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_depth
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	//return current node depth
	return information->current().getDepth();
}

/**
 * Returns the 3D coordinates of the center of the current node.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_coordinate
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	point3d coordinate = information->current().getCoordinate();
	jclass clsPoint = env->FindClass(CLS_POINT3D);
	jmethodID constructor = env->GetMethodID(clsPoint, METHOD_CONSTRUCTOR, "(FFF)V");
	//new Point3D(x, y, z)
	return env->NewObject(clsPoint, constructor, (float) coordinate.x(), (float) coordinate.y(), (float) coordinate.z());
}

/**
 * Returns the x coordinate of the center of the current node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_x
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return (float) information->current().getX();
}

/**
 * Returns the y coordinate of the center of the current node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_y
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return (float) information->current().getY();
}

/**
 * Returns the z coordinate of the center of the current node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_z
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return (float) information->current().getZ();
}

/**
 * Returns the size of the node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_size
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return (float) information->current().getSize();
}

/**
 * Returns the current node.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_node
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	OcTreeNode *node = (OcTreeNode*) information->current().operator ->();
	jclass clsNode = env->FindClass(CLS_JOCTREENODE);
	jmethodID constructor = env->GetMethodID(clsNode, METHOD_CONSTRUCTOR, "(J)V");
	return env->NewObject(clsNode, constructor, node);
}
