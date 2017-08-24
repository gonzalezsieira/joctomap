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
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class es_usc_citius_lab_joctomap_util_CollisionChecker2D */

#ifndef _Included_es_usc_citius_lab_joctomap_util_CollisionChecker2D
#define _Included_es_usc_citius_lab_joctomap_util_CollisionChecker2D
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     es_usc_citius_lab_joctomap_util_CollisionChecker2D
 * Method:    collides
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Point2D;F)Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Point2D_2F
  (JNIEnv *, jclass, jobject, jobject, jfloat);

/*
 * Class:     es_usc_citius_lab_joctomap_util_CollisionChecker2D
 * Method:    collides
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Les/usc/citius/lab/motionplanner/core/spatial/Pose2D;FF)Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_util_CollisionChecker2D_collides__Les_usc_citius_lab_joctomap_octree_JOctree_2Les_usc_citius_lab_motionplanner_core_spatial_Pose2D_2FF
  (JNIEnv *, jclass, jobject, jobject, jfloat, jfloat);

#ifdef __cplusplus
}
#endif
#endif
