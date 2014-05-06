#include <jni.h>
#include <octomap/OcTree.h>
#include <dynamicEDT3D/dynamicEDTOctomap.h>
#include "nativeobject.h"
#include "joctreedistancemap.h"

using namespace octomap;

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
	//method before does not really calculate the map distances, this is done by the following command
	//update({default = true}) is slower because it computes squared roots of the distances, this is, the real ones
	distanceMap->update();
	//instantiate java object JOctreeDistanceMap
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
	return env->NewObject(cls, constructor, distanceMap);
}
