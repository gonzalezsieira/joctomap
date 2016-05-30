#include <jni.h>
#include <octomap/OcTree.h>
#include <dynamicEDT3D/dynamicEDTOctomap.h>
#include "nativeobject.h"
#include "joctreedistancemap.h"

using namespace octomap;

/*
 * Class:     es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap
 * Method:    freeMemory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap_dispose
  (JNIEnv *env, jobject jdistancemap){
        //get pointer
        long pointer = getPointer(env, jdistancemap);
        //if not released yet
        if(pointer != INVALID_POINTER){
            //recover octree
            DynamicEDTOctomap *object = (DynamicEDTOctomap*) pointer;
            //delete object
            delete object;
            //delete pointer
            deletePointer(env, jdistancemap);
        }
}

/**
 * Executes the update operation of the distance map.
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap_update
  (JNIEnv *env, jobject jdistancemap){
	//retrieve pointer to native object
	DynamicEDTOctomap *distancemap = (DynamicEDTOctomap*) getPointer(env, jdistancemap);
	//update({default = true}) is slower because it computes squared roots of the distances, this is, the real ones
	distancemap->update();
}

/**
 * Retrieves the distance to the nearest occupied cell, up to maxDistance. (-1 is returned if no obstacle is found)
 */
JNIEXPORT jdouble JNICALL Java_es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap_getDistance__DDD
  (JNIEnv *env, jobject jdistancemap, jdouble x, jdouble y, jdouble z){
	//retrieve pointer to native object
	DynamicEDTOctomap *distancemap = (DynamicEDTOctomap*) getPointer(env, jdistancemap);
	//obtain distance
	return distancemap->getDistance(point3d(x, y, z));
}

/**
 * Retrieves the distance to the nearest occupied cell, given an octree key, up to maxDistance (-1 is returned if no obstacle if found)
 */
JNIEXPORT jdouble JNICALL Java_es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap_getDistance__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2
  (JNIEnv *env, jobject jdistancemap, jobject joctreekey){
	//retrieve pointer to native object
	DynamicEDTOctomap *distancemap = (DynamicEDTOctomap*) getPointer(env, jdistancemap);
	//retrieve pointer to native object
	OcTreeKey *octreekey = (OcTreeKey*) getPointer(env, joctreekey);
	//obtain distance
	return distancemap->getDistance(*octreekey);
}

/**
 * Retrieves the nearest obstacle (and distance vector to it).
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap_getDistanceAndClosestObstacle
  (JNIEnv *env, jobject jdistancemap, jdouble x, jdouble y, jdouble z){
	//retrieve pointer to native object
	DynamicEDTOctomap *distancemap = (DynamicEDTOctomap*) getPointer(env, jdistancemap);
	//define variables to store the result
	float distance;
	point3d obstacle(0, 0, 0);
	point3d position(x, y, z);
	distancemap->getDistanceAndClosestObstacle(position, distance, obstacle);
	//find constructor method IDs
	jclass clsObstacle = env->FindClass("es/usc/citius/lab/joctomap/util/Obstacle");
	jclass clsPoint3d = env->FindClass("es/usc/citius/lab/motionplanner/core/Point3D");
	jmethodID constructorObstacle = env->GetMethodID(clsObstacle, "<init>", "(Les/usc/citius/lab/motionplanner/core/Point3D;Les/usc/citius/lab/motionplanner/core/Point3D;)V");
	jmethodID constructorPoint3D = env->GetMethodID(clsPoint3d, "<init>", "(DDD)V");
	//instantiate points
	jobject pointPosition = env->NewObject(clsPoint3d, constructorPoint3D, x, y, z);
	jobject pointObstacle = env->NewObject(clsPoint3d, constructorPoint3D, obstacle.x(), obstacle.y(), obstacle.z());
	//instantiate obstacle
	return env->NewObject(clsObstacle, constructorObstacle, pointPosition, pointObstacle);
}

/**
 * Obtains a new instance of JOctreeDistanceMap from a JOctree.
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_distance_JOctreeDistanceMap_create
  (JNIEnv *env, jclass cls, jobject jtree, jdouble maxDistance){
	//retrieve octree pointer
	OcTree *octree = (OcTree*) getPointer(env, jtree);
	double x, y, z;
	//compute min and max points of the octree
	octree->getMetricMin(x, y, z);
	point3d min(x, y, z);
	octree->getMetricMax(x, y, z);
	point3d max(x, y, z);
	//treat unknown cells as occupied ones
	bool unknownAsOccupied = true;
	//obtain JOctree distance map
	DynamicEDTOctomap *distanceMap = new DynamicEDTOctomap(maxDistance, octree, min, max, unknownAsOccupied);
	//instantiate java object JOctreeDistanceMap
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
	return env->NewObject(cls, constructor, distanceMap);
}
