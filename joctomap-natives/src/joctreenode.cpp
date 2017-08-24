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

/**
 * Retrieves the list of children of the node. Not implemented yet.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_getChildren
  (JNIEnv *env, jobject jnode){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//retrieve ArrayList class
	jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
	//retrieve ArrayList constructor
	jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
	//retrieve ArrayList add method
	jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
	//retrieve JOctreeNode class
	jclass nodeClass = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeNode");
	//retrieve JOctreeNode constructor
	jmethodID nodeConstructor = env->GetMethodID(nodeClass, "<init>", "(J)V");
	//new arraylist()
	jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
	int i = 0;
	while(true){
		//break when the i-th child does not exist, and return the i-th
		if(!node->childExists(i)){
			break;
		}
		//increase counter
		i++;
		//instantiate current child
		jobject currentChild = env->NewObject(nodeClass, nodeConstructor, node->getChild(i));
		//add object to the ArrayList
		env->CallBooleanMethod(arrayListObject, addArrayList, currentChild);
	}
	//return arraylist object
	return arrayListObject;
}

/**
 * Retrieves the number of children of this node. Not implemented yet.
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_getNumChildren
  (JNIEnv *env, jobject jnode){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	int childs = 0;
	while(true){
		//break when the i-th child does not exist, and return the i-th
		if(!node->childExists(childs)){
			break;
		}
		//increase counter
		childs++;
	}
	//return number of childs
	return childs;
}

/**
 * Retrieves the i-th child of the current node, if exists. NULL otherwise.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_getChild
  (JNIEnv *env, jobject jnode, jint i){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//check if the i-th node exist, return it if exist
	if(node->childExists(i)){
		OcTreeNode *child = node->getChild(i);
			//find class and constructor to instantiate joctreenode
			jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeNode");
			jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
			//return new instance
			return env->NewObject(cls, constructor, child);
	}
	//return null if the child does not exist
	else{
		return NULL;
	}
}

/**
 * Retrieves if the i-th child of this node exists.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_childExists
  (JNIEnv *env, jobject jnode, jint i){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//return result
	return node->childExists(i);
}

/**
 * Queries if the node has children.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_hasChildren
  (JNIEnv *env, jobject jnode){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//return result
	return node->hasChildren();
}

/**
 * Queries if this node is collapsible.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctreeNode_collapsible
  (JNIEnv *env, jobject jnode){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, jnode);
	//return result
	return node->collapsible();
}
