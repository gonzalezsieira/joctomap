#include <jni.h>
#include <octomap/OcTree.h>
#include "leafbbxiterator.h"
#include "iteratorinformation.h"
#include "nativeobject.h"

using namespace std;
using namespace octomap;

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
	jclass clsKey = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
	jmethodID constructor = env->GetMethodID(clsKey, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(clsKey, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
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
	jclass clsKey = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
	jmethodID constructor = env->GetMethodID(clsKey, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(clsKey, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
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
	jclass clsPoint = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
	jmethodID constructor = env->GetMethodID(clsPoint, "<init>", "(FFF)V");
	//new Point3D(x, y, z)
	return env->NewObject(clsPoint, constructor, static_cast<float>(coordinate.x()), static_cast<float>(coordinate.y()), static_cast<float>(coordinate.z()));
}

/**
 * Returns the x coordinate of the center of the current node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_x
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return static_cast<float>(information->current().getX());
}

/**
 * Returns the y coordinate of the center of the current node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_y
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return static_cast<float>(information->current().getY());
}

/**
 * Returns the z coordinate of the center of the current node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_z
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return static_cast<float>(information->current().getZ());
}

/**
 * Returns the size of the node.
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_size
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	return static_cast<float>(information->current().getSize());
}

/**
 * Returns the current node.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_node
  (JNIEnv *env, jobject jiterator){
	//get native object
	LeafBBXIteratorInformation *information = (LeafBBXIteratorInformation*) getPointer(env, jiterator);
	OcTreeNode *node = (OcTreeNode*) information->current().operator ->();
	jclass clsNode = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeNode");
	jmethodID constructor = env->GetMethodID(clsNode, "<init>", "(J)V");
	return env->NewObject(clsNode, constructor, node);
}
