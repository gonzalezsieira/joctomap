#include <jni.h>
#include <octomap/OcTree.h>
#include <dynamicEDT3D/dynamicEDTOctomap.h>
#include "nativeobject.h"
#include "joctreedistancemap.h"

using namespace octomap;

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
	distanceMap->update();
	//instantiate java object JOctreeDistanceMap
	jmethodID constructor = env->GetMethodID(cls, "<init>", "(J)V");
	return env->NewObject(cls, constructor, distanceMap);
}
