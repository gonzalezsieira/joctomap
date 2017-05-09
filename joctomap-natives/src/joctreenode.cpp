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
#include <octomap/OcTreeNode.h>
#include "nativeobject.h"
#include "joctreenode.h"

using namespace std;
using namespace octomap;

/*
 * Freees memory when calling finalize() in java.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_dispose
  (JNIEnv *env, jobject jnode){
    //Not needed to do anything to free memory of this object.
}

/**
 * Retrieves the occupancy probability of the node.
 */
JNIEXPORT jdouble JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_getOccupancy
  (JNIEnv * env, jobject jnode){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//return occupancy probability
	return node->getOccupancy();
}

/**
 * Retrieves the value stored in the node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_getValue
  (JNIEnv *env, jobject jnode){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//return occupancy probability
	return node->getValue();
}
