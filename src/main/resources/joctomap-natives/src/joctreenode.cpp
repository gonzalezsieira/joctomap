#include <iostream>
#include <jni.h>
#include <octomap/OcTreeNode.h>
#include "nativeobject.h"
#include "joctreenode.h"

using namespace std;
using namespace octomap;

/**
 * Retrieves the occupancy probability of the node.
 */
JNIEXPORT jdouble JNICALL Java_es_usc_citius_lab_joctomap_JOctreeNode_getOccupancy
  (JNIEnv * env, jobject obj){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, obj);
	//return occupancy probability
	return node->getOccupancy();
}

/**
 * Retrieves the value stored in the node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_JOctreeNode_getValue
  (JNIEnv *env, jobject obj){
	//retrieve native pointer
	OcTreeNode *node = (OcTreeNode*) getPointer(env, obj);
	//return occupancy probability
	return node->getValue();
}
