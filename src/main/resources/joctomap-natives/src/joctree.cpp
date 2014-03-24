#include  <iostream>
#include <jni.h>
#include <octomap/AbstractOcTree.h>
#include <octomap/OccupancyOcTreeBase.h>
#include "joctree.h"

using namespace std;
using namespace octomap;

/**
 * This method writes an octree, given the pointer to the object and the
 * filename.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_JOctree_write
  (JNIEnv *env, jobject obj , jlong octreePointer, jstring filename){
	//convert jstring into native char*
	jboolean iscopy = false;
	const char *nativeFilename = env->GetStringUTFChars(filename, &iscopy);
	//write octree in filename
	AbstractOcTree *octree = (AbstractOcTree*) octreePointer;
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
JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_JOctree_read
  (JNIEnv *env, jobject obj, jstring filename){
	//convert jstring into native char*
	jboolean iscopy = false;
	const char *nativeFilename = env->GetStringUTFChars(filename, &iscopy);
	//read octree in the given filename
	AbstractOcTree *octree = AbstractOcTree::read(nativeFilename);
	//release memory of native char*
	env->ReleaseStringUTFChars(filename, nativeFilename);
	return (jlong) octree;
}
