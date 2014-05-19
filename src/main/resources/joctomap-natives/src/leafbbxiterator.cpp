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
	IteratorInformation *information = (IteratorInformation*) getPointer(env, jiterator);
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
	IteratorInformation *information = (IteratorInformation*) getPointer(env, jiterator);
	//retrieve current key
	OcTreeKey key = information->next().getKey();
	//find class and constructor to instantiate JOCtreekey
	jclass clsKey = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
	jmethodID constructor = env->GetMethodID(clsKey, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(clsKey, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}
