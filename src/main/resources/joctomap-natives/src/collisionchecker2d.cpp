/**
 * Copyright (C) 2014-2016 Adrián González Sieira (adrian.gonzalez@usc.es)
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

using namespace octomap;

//global variables
point3d ZERO = point3d(0, 0, 0);
point3d UNIT = point3d(1, 0, 0);
double M_3PI_4 = M_PI_4 * 3;

/*
    Calculates the rotation of the points given the original and the 3D angles.
 */
point3d rotate(point3d point, double yaw, double pitch, double roll){
    //current coordinates
    double x = point.x();
    double y = point.y();
    double z = point.z();
    //sin and cos values for given angles
    double sinyaw = sin(yaw);
    double sinpitch = sin(pitch);
    double sinroll = sin(roll);
    double cosyaw = cos(yaw);
    double cospitch = cos(pitch);
    double cosroll = cos(roll);
    //point3d construction
    return point3d(
        (x * (cosyaw * cospitch) 
            + y * (-sinyaw * cosroll + cosyaw * sinpitch * sinroll) 
            + z * (sinyaw * sinroll + cosyaw * sinpitch * cosroll)),
        (x * (sinyaw * cospitch)
            + y * (cosyaw * cosroll + sinyaw * sinpitch * sinroll)
            + z * (-cosyaw * sinroll + sinyaw * sinpitch * cosroll)),
        (x * (-sinpitch)
            + y * (cospitch * sinroll)
            + z * (cospitch * cosroll))
    );
}

/**
 * This method calculates the projection between a line and a plane.
 * 
 * @param a 1st point defining the line
 * @param b 2nd point defining the line
 * @param normal normal vector of the plane
 * @param distance between the plane and the point ZERO
 * @return intersection between the line and the plane
 */
point3d intersection_line_and_plane(point3d a, point3d b, octomath::Vector3 normal, double distance){
    //a mult normal
    double normal_dot_a = normal.dot(a);
    octomath::Vector3 ba = b - a;
    //ba mult normal
    double normal_dot_ba = normal.dot(ba);
    //projection index
    double projIndex = (distance - normal_dot_a) / normal_dot_ba;
    //calculate projection
    return a + b * projIndex;
}

/*
    This calculates the relative position of the border of the cell given the angles.
 */
point3d relative_position_border_cell(double resolution, double yaw, double pitch){
    //rotate original point
    point3d point = rotate(UNIT, yaw, pitch, 0);
    //calculate absolute values
    double abspitch = std::abs(pitch);
    double absyaw = std::abs(yaw);
    //this sign is used to change the projection to be positive or negative
    double sign;
    octomath::Vector3 normal;
    if(abspitch <= M_PI_4 || abspitch >= M_3PI_4){
        if(absyaw <= M_PI_4){
            sign = 1;
            normal = octomath::Vector3(1, 0, 0);
        }
        else if (absyaw >= M_3PI_4) {
            sign = -1;
            normal = octomath::Vector3(1, 0, 0);
        } else if (yaw > M_PI_4 && yaw < M_3PI_4) {
            sign = 1;
            normal = octomath::Vector3(0, 1, 0);
        } else {
            sign = -1;
            normal = octomath::Vector3(0, 1, 0);
        }
    }
    else if(pitch > M_PI_4 && pitch < M_3PI_4){
        sign = 1;
        normal = octomath::Vector3(0, 0, 1);
    }
    else{
        sign = -1;
        normal = octomath::Vector3(0, 0, 1);
    }
    return intersection_line_and_plane(ZERO, point, normal, sign * resolution / 2);
}

/* 
 * Returns if there is a collision in a given pose using a 2D circular robot shape.
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Point2D_2F
  (JNIEnv *env, jclass cls, jobject joctree, jobject jpoint, jfloat radius){
    //get values of the Pose2D object
    jclass clsPoint2D = env->FindClass("es/usc/citius/lab/motionplanner/core/spatial/Point2D");
    float pointX = env->GetFloatField(jpoint, env->GetFieldID(clsPoint2D, "x", "F"));
    float pointY = env->GetFloatField(jpoint, env->GetFieldID(clsPoint2D, "y", "F"));
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
        if(octree->isNodeOccupied(currentNode)){
            //retrieve center of the current cell
            point3d center_cell = it.getCoordinate();
            //calculate vector distance between the center and the given point
            float distance[2] = {center_cell.x() - pointX, center_cell.y() - pointY};
            //calculate angle of the distance vector (angle from the center of the cell to the given point)
            double angle_cell = atan2(-distance[1], -distance[0]);
            //get relative position of the border of the cell
            point3d border = relative_position_border_cell(it.getSize(), angle_cell, 0);
            distance[0] += border.x();
            distance[1] += border.y();
            if(hypot(distance[0], distance[1]) <= radius){
                return true;
            }
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


