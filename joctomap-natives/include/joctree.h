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
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class es_usc_citius_lab_joctomap_octree_JOctree */

#ifndef _Included_es_usc_citius_lab_joctomap_octree_JOctree
#define _Included_es_usc_citius_lab_joctomap_octree_JOctree
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_dispose
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    coordToKey
 * Signature: (FFF)Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__FFF
  (JNIEnv *, jobject, jfloat, jfloat, jfloat);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    coordToKey
 * Signature: (FFFI)Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__FFFI
  (JNIEnv *, jobject, jfloat, jfloat, jfloat, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    adjustKeyAtDepth
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctreeKey;I)Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_adjustKeyAtDepth
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    coordToKey
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2
  (JNIEnv *, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    coordToKey
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;I)Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_coordToKey__Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2I
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    keyToCoord
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_keyToCoord__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2
  (JNIEnv *, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    keyToCoord
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctreeKey;I)Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_keyToCoord__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    search
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctreeKey;I)Les/usc/citius/lab/joctomap/octree/JOctreeNode;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_search__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    search
 * Signature: (FFFI)Les/usc/citius/lab/joctomap/octree/JOctreeNode;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_search__FFFI
  (JNIEnv *, jobject, jfloat, jfloat, jfloat, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    isNodeOccupied
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctreeNode;)Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_isNodeOccupied
  (JNIEnv *, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    updateNode
 * Signature: (FFFZ)Les/usc/citius/lab/joctomap/octree/JOctreeNode;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_updateNode
  (JNIEnv *, jobject, jfloat, jfloat, jfloat, jboolean);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    useBBXLimit
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_useBBXLimit
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    setBBX
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_setBBX
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    setBBXMin
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_setBBXMin
  (JNIEnv *, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    setBBXMax
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_setBBXMax
  (JNIEnv *, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_size
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getTreeDepth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getTreeDepth
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getResolution
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getResolution
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getNodeSize
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getNodeSize
  (JNIEnv *, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getBBXCenter
 * Signature: ()Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getBBXCenter
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getBBXMin
 * Signature: ()Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getBBXMin
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getBBXMax
 * Signature: ()Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getBBXMax
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getMetricMin
 * Signature: ()Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getMetricMin
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getMetricMax
 * Signature: ()Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getMetricMax
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    getMetricSize
 * Signature: ()[F
 */
JNIEXPORT jfloatArray JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_getMetricSize
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    isBBXSet
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_isBBXSet
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    isBBXApplied
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_isBBXApplied
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    leafBBXIterator
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;Les/usc/citius/lab/motionplanner/core/spatial/Point3D;I)Les/usc/citius/lab/joctomap/iterators/OctreeIterator;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_leafBBXIterator__Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2Les_usc_citius_lab_motionplanner_core_spatial_Point3D_2I
  (JNIEnv *, jobject, jobject, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    leafBBXIterator
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctreeKey;Les/usc/citius/lab/joctomap/octree/JOctreeKey;I)Les/usc/citius/lab/joctomap/iterators/OctreeIterator;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_leafBBXIterator__Les_usc_citius_lab_joctomap_octree_JOctreeKey_2Les_usc_citius_lab_joctomap_octree_JOctreeKey_2I
  (JNIEnv *, jobject, jobject, jobject, jint);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    prune
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_prune
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    expand
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_expand
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    updateInnerOccupancy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_updateInnerOccupancy
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    write
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_write
  (JNIEnv *, jobject, jstring);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    read
 * Signature: (Ljava/lang/String;)Les/usc/citius/lab/joctomap/octree/JOctree;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_read
  (JNIEnv *, jclass, jstring);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    create
 * Signature: (F)Les/usc/citius/lab/joctomap/octree/JOctree;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_create
  (JNIEnv *, jclass, jfloat);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    enableChangeDetection
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_enableChangeDetection
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    resetChangeDetection
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_resetChangeDetection
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_octree_JOctree
 * Method:    keysChanged
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_octree_JOctree_keysChanged
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
