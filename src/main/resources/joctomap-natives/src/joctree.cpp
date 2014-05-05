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
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_cellKeyAt__FFF
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	OcTreeKey key = octree->coordToKey(x, y, z);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/*
 * This method finds the position of a cell given a real position in the map (x, y, z) at
 * a given depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_cellKeyAt__FFFI
  (JNIEnv *env, jobject jtree, jfloat x, jfloat y, jfloat z, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	OcTreeKey key = octree->coordToKey(x, y, z, depth);
	//find class and constructor to instantiate JOCtreekey
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(III)V");
	//new JOctreeKey(x, y, z)
	return env->NewObject(cls, constructor, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
}

/**
 * This method adjusts the key of a node at a givn depth.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_adjustKeyAt
  (JNIEnv *env, jobject jtree, jobject jkey, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover field IDs
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
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
 * This method searches in the octree a node given by a key and a depth (depth = 0 means search
 * in the whole octree).
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_search__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *env, jobject jtree, jobject jkey, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//recover key field IDs
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
	jfieldID fieldX = env->GetFieldID(cls, "x", "I");
	jfieldID fieldY = env->GetFieldID(cls, "y", "I");
	jfieldID fieldZ = env->GetFieldID(cls, "z", "I");
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
		cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeNode");
		jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
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
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_search__DDDI
  (JNIEnv *env, jobject jtree, jdouble x, jdouble y, jdouble z, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//search node in the octree
	OcTreeNode *node = octree->search(x, y, z, depth);
	//return new node instance if the node is found in the octree
	if(node != NULL){
		//find class and constructor to instantiate joctreenode
		jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeNode");
		jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
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
  (JNIEnv *env, jobject jtree, jdouble x, jdouble y, jdouble z, jboolean occupied){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//update information of the node
	OcTreeNode *node = octree->updateNode(x, y, z, (bool) occupied, false);
	//find class and constructor to instantiate joctreenode
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeNode");
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
	//return new instance
	return env->NewObject(cls, constructor, node);
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
JNIEXPORT jdouble JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getResolution
  (JNIEnv *env, jobject jtree){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return resolution
	return octree->getResolution();
}

/**
 * Method that obtains the resolution at a current depth.
 */
JNIEXPORT jdouble JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getNodeSize
  (JNIEnv *env, jobject jtree, jint depth){
	//recover octree
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	//return node size ath the given depth
	return octree->getNodeSize(static_cast<unsigned int>(depth));
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

/**
 * Instantiates a new empty octree with the given resolution.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_create
  (JNIEnv *env, jclass cls, jdouble res){
	//find constructor to instantiate the result
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
	//instantiate new empty octree
	OcTree *octree = new OcTree(res);
	//return new java instance of the octree
	return env->NewObject(cls, constructor, octree);
}
