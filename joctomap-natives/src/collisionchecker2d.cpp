/**
 * Copyright (C) 2014-2017 Adrián González Sieira (adrian.gonzalez@usc.es)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * collisionchecker2d.cpp
 *
 *  Created on: 29/01/2015
 *      Author: adrian.gonzalez
 */

#include <jni.h>
#include <map>
#include <math.h>
#include <octomap/OcTree.h>
#include <octomap/OccupancyOcTreeBase.h>
#include <octomap/OcTreeNode.h>
#include <octomap/math/Vector3.h>
#include "nativeobject.h"
#include "collisionchecker2d.h"
#include "collisions.h"

using namespace octomap;

/* 
 * Returns if there is a collision in a given pose using a 2D circular robot shape.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Point_2F
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpoint, jfloat radius){
    //get values of the Pose2D object
    jclass cls_point = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point");
    jmethodID methodGetX = env->GetMethodID(cls_point, "getX", "()F");
    jmethodID methodGetY = env->GetMethodID(cls_point, "getY", "()F");
    float pointX = env->CallFloatMethod(jpoint, methodGetX);
    float pointY = env->CallFloatMethod(jpoint, methodGetY);
    //get octree object
    OcTree *octree = (OcTree*) getPointer(env, joctree);
    return checkCollision(point3d(pointX, pointY, 0), radius, octree);
}

/* 
 * Returns if there is a collision in a given pose using a 2D rectangular robot shape.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Pose_2FF
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpose, jfloat dx, jfloat dy){
    //get values of the Pose2D object
    jclass cls_pose = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Pose");
    jmethodID methodGetX = env->GetMethodID(cls_pose, "getX", "()F");
    jmethodID methodGetY = env->GetMethodID(cls_pose, "getY", "()F");
    jmethodID methodGetYaw = env->GetMethodID(cls_pose, "getYaw", "()F");
    float poseX = env->CallFloatMethod(jpose, methodGetX);
    float poseY = env->CallFloatMethod(jpose, methodGetY);
    float poseYaw = env->CallFloatMethod(jpose, methodGetYaw);
    //determine current pose from jpose fields
    point3d pose = point3d(poseX, poseY, 0);
    //determine pessimistic radius of the robot
    float radius = sqrt(poseX * poseX + poseY * poseY);
    //determine min and max dimensions of the BBX to iterate the octree
    point3d min = point3d(poseX - radius, poseY - radius, 0);
    point3d max = point3d(poseX + radius, poseY + radius, 0);
    return false;
}


