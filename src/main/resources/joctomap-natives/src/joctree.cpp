#include <iostream>
#include <fstream>
#include <sstream>
#include <jni.h>
#include <octomap/OcTree.h>
#include <octomap/AbstractOcTree.h>
#include <octomap/OccupancyOcTreeBase.h>
#include "joctree.h"
#include "nativeobject.h"

using namespace std;
using namespace octomap;

/*
 * This method finds the position of a cell given a real position in the map (x, y, z)
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_JOctree_cellKeyAt__FFF
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	OcTreeKey key = octree->coordToKey(x, y, z);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/JOctreeKey");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/*
 * This method finds the position of a cell given a real position in the map (x, y, z) at
 * a given depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_JOctree_cellKeyAt__FFFI
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	OcTreeKey key = octree->coordToKey(x, y, z, depth);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/JOctreeKey");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/**
 * This method adjusts the key of a node at a givn depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_JOctree_adjustKeyAt
  (JNIEnv *env, jobject jtree, jobject jkey, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field IDs
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/JOctreeKey");
	jfieldID fieldX = env->GetFieldID(cls, "x", "I");
	jfieldID fieldY = env->GetFieldID(cls, "y", "I");
	jfieldID fieldZ = env->GetFieldID(cls, "z", "I");
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
 * This method writes an octree, given the pointer to the object and the
 * filename.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_JOctree_write
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
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_JOctree_read
  (JNIEnv *env, jclass obj, jstring filename){
	//find constructor to instantiate the result
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/JOctree");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
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
	return env->NewObject(cls, constructor, ot);
}
