#include <iostream>
#include <jni.h>
#include <octomap/OcTree.h>
#include <octomap/OcTreeKey.h>
#include <octomap/OccupancyOcTreeBase.h>
#include <octomap/OcTreeNode.h>
#include <float.h>
#include "nativeobject.h"
#include "joctreeutils.h"

using namespace std;
using namespace octomap;

/*
 *
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_getOccupiedKeysBetween
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpointmin, jobject jpointmax){
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    
    //get values of the Point3D objects
    float pointminX = env->GetFloatField(jpointmin, fieldX);
    float pointminY = env->GetFloatField(jpointmin, fieldY);
    float pointminZ = env->GetFloatField(jpointmin, fieldZ);
    float pointmaxX = env->GetFloatField(jpointmax, fieldX);
    float pointmaxY = env->GetFloatField(jpointmax, fieldY);
    float pointmaxZ = env->GetFloatField(jpointmax, fieldZ);
    
    //define min/max points to iterate
    point3d min = point3d(pointminX, pointminY, pointminZ);
    point3d max = point3d(pointmaxX, pointmaxY, pointmaxZ);
    
    //find class and constructor to instantiate JOCtreekey
    jclass clsKey = env->FindClass("es/usc/citius/lab/joctomap/octree/JOctreeKey");
    //find ArrayList class
    jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
    //retrieve JOctreeKey constructor
    jmethodID constructorKey = env->GetMethodID(clsKey, "<init>", "(III)V");
    //retrieve ArrayList constructor
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    //retrieve ArrayList add method
    jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    	
    //new arraylist()
    jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
    //get octree object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    //iterate and store objects in a java arraylist
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(min, max, 0), end = octree->end_leafs_bbx(); it != end; ++it){
        //transform current leaf_bbx_iterator to node
        OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
        //check colliding obstacles only
        if(octree->isNodeOccupied(currentNode)){
            OcTreeKey key = it.getKey();
            jobject currentKey = env->NewObject(clsKey, constructorKey, static_cast<int>(key.k[0]), static_cast<int>(key.k[1]), static_cast<int>(key.k[2]));
            //add object to the ArrayList
            env->CallBooleanMethod(arrayListObject, addArrayList, currentKey);
        }
    }
    return arrayListObject;
}

/*
 *
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_getOccupiedCoordinatesBetween
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpointmin, jobject jpointmax){
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    
    //get values of the Point3D objects
    float pointminX = env->GetFloatField(jpointmin, fieldX);
    float pointminY = env->GetFloatField(jpointmin, fieldY);
    float pointminZ = env->GetFloatField(jpointmin, fieldZ);
    float pointmaxX = env->GetFloatField(jpointmax, fieldX);
    float pointmaxY = env->GetFloatField(jpointmax, fieldY);
    float pointmaxZ = env->GetFloatField(jpointmax, fieldZ);
    
    //define min/max points to iterate
    point3d min = point3d(pointminX, pointminY, pointminZ);
    point3d max = point3d(pointmaxX, pointmaxY, pointmaxZ);

    //find Pair class
    jclass clsPair = env->FindClass("es/usc/citius/lab/motionplanner/core/util/Pair");
    //find ArrayList class
    jclass arrayListClass = env->FindClass("Ljava/util/ArrayList;");
    //find Float class
    jclass floatClass = env->FindClass("Ljava/lang/Float;");
    //retrieve Point3D constructor
    jmethodID constructorPoint3D = env->GetMethodID(clsPoint3D, "<init>", "(FFF)V");
    //retrieve Float constructor
    jmethodID floatConstructor = env->GetMethodID(floatClass, "<init>", "(F)V");
    //retrieve Pair constructor
    jmethodID constructorPair = env->GetMethodID(clsPair, "<init>", "(Ljava/lang/Object;Ljava/lang/Object;)V");
    //retrieve ArrayList constructor
    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    //retrieve ArrayList add method
    jmethodID addArrayList = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    	
    //new arraylist()
    jobject arrayListObject = env->NewObject(arrayListClass, arrayListConstructor);
    //get octree object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    //iterate and store objects in a java arraylist
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(min, max, 0), end = octree->end_leafs_bbx(); it != end; ++it){
        //transform current leaf_bbx_iterator to node
        OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
        //check colliding obstacles only
        if(octree->isNodeOccupied(currentNode)){
            point3d coordinate = it.getCoordinate();
            jobject currentPoint3D = env->NewObject(clsPoint3D, constructorPoint3D, static_cast<float>(coordinate.x()), static_cast<float>(coordinate.y()), static_cast<float>(coordinate.z()));
            jobject currentFloat = env->NewObject(floatClass, floatConstructor, static_cast<float>(it.getSize()));
            jobject currentPair = env->NewObject(clsPair, constructorPair, currentPoint3D, currentFloat);
            //add object to the ArrayList
            env->CallBooleanMethod(arrayListObject, addArrayList, currentPair);
        }
    }
    return arrayListObject;
}


/*
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    resolutionAt
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_resolutionAt
  (JNIEnv *env, jclass cls, jobject jtree, jobject jpoint){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    //get values of the Point3D objects
    float pointX = env->GetFloatField(jpoint, fieldX);
    float pointY = env->GetFloatField(jpoint, fieldY);
    float pointZ = env->GetFloatField(jpoint, fieldZ);
    //query resolution of the octree
    OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(point3d(pointX, pointY, pointZ), point3d(pointX, pointY, pointZ), 0);
    float resolution = FLT_MAX;
    if(it != octree->end_leafs_bbx()){
        resolution = static_cast<float>(it.getSize());
    }
    return resolution;
}

/*
 * Class:     es_usc_citius_lab_joctomap_util_JOctreeUtils
 * Method:    resolutionAddedIn
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_util_JOctreeUtils_resolutionAddedIn
  (JNIEnv *env, jclass cls, jobject jtree, jobject jpoint1, jobject jpoint2){
    //recover octree
    OcTree *octree = (OcTree*) getPointer(env, jtree);
    //get fields of Point3D object
    jclass clsPoint3D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point3D");
    jfieldID fieldX = env->GetFieldID(clsPoint3D, "x", "F");
    jfieldID fieldY = env->GetFieldID(clsPoint3D, "y", "F");
    jfieldID fieldZ = env->GetFieldID(clsPoint3D, "z", "F");
    //get values of the Point3D objects
    float point1X = env->GetFloatField(jpoint1, fieldX);
    float point1Y = env->GetFloatField(jpoint1, fieldY);
    float point1Z = env->GetFloatField(jpoint1, fieldZ);
    float point2X = env->GetFloatField(jpoint2, fieldX);
    float point2Y = env->GetFloatField(jpoint2, fieldY);
    float point2Z = env->GetFloatField(jpoint2, fieldZ);
    //query first resolution of the octree
    OcTree::leaf_bbx_iterator it1 = octree->begin_leafs_bbx(point3d(point1X, point1Y, point1Z), point3d(point1X, point1Y, point1Z), 0);
    float resolution1 = FLT_MAX;
    if(it1 != octree->end_leafs_bbx()){
        resolution1 = static_cast<float>(it1.getSize());
    }
    //query second resolution of the octree
    OcTree::leaf_bbx_iterator it2 = octree->begin_leafs_bbx(point3d(point2X, point2Y, point2Z), point3d(point2X, point2Y, point2Z), 0);
    float resolution2 = FLT_MAX;
    if(it2 != octree->end_leafs_bbx()){
        resolution2 = static_cast<float>(it2.getSize());
    }
    return resolution1 + resolution2;
}