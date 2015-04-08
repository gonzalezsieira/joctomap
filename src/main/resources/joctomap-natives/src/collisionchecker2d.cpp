/*
 * collisionchecker2d.cpp
 *
 *  Created on: 29/01/2015
 *      Author: adrian.gonzalez
 */

#include <jni.h>
#include <math.h>
#include <octomap/OcTree.h>
#include <octomap/OccupancyOcTreeBase.h>
#include <octomap/OcTreeNode.h>
#include "nativeobject.h"
#include "collisionchecker2d.h"

using namespace octomap;

/* 
 * Returns if there is a collision in a given pose using a 2D circular robot shape.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Point2D_2F
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpoint, jfloat radius){
    //get values of the Pose2D object
    jclass clsPoint2D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point2D");
    float pointX = env->GetFloatField(jpoint, env->GetFieldID(clsPoint2D, "x", "F"));
    float pointY = env->GetFloatField(jpoint, env->GetFieldID(clsPoint2D, "y", "F"));
    //determine current pose from jpose fields
    point3d pose = point3d(pointX, pointY, 0);
    //determine min and max dimensions of the BBX to iterate the octree
    point3d min = point3d(pointX - radius, pointY - radius, 0);
    point3d max = point3d(pointX + radius, pointY + radius, 0);
    //get octree object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    //iterate over the obstacles in the octree
    for(OcTree::leaf_bbx_iterator it = octree->begin_leafs_bbx(min, max, 0), end = octree->end_leafs_bbx(); it != end; ++it){
        //transform current leaf_bbx_iterator to node
        OcTreeNode *currentNode = (OcTreeNode*) it.operator ->();
        //check colliding obstacles only
        if(octree->isNodeOccupied(currentNode) && it.getCoordinate().distanceXY(pose) <= radius){
            return true;
        }
    }
    //if we found no obstacles below radius, return not occupied
    return false;
}

/* 
 * Returns if there is a collision in a given pose using a 2D rectangular robot shape.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Pose2D_2FF
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpose, jfloat dx, jfloat dy){
    //get values of the Pose2D object
    jclass clsPose2D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Pose2D");
    float poseX = env->GetFloatField(jpose, env->GetFieldID(clsPose2D, "x", "F"));
    float poseY = env->GetFloatField(jpose, env->GetFieldID(clsPose2D, "y", "F"));
    float poseYaw = env->GetFloatField(jpose, env->GetFieldID(clsPose2D, "yaw", "F"));
    //determine current pose from jpose fields
    point3d pose = point3d(poseX, poseY, 0);
    //determine pessimistic radius of the robot
    float radius = sqrt(poseX * poseX + poseY * poseY);
    //determine min and max dimensions of the BBX to iterate the octree
    point3d min = point3d(poseX - radius, poseY - radius, 0);
    point3d max = point3d(poseX + radius, poseY + radius, 0);
    return false;
}


