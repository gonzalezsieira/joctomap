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
/* Header for class es_usc_citius_lab_joctomap_iterators_LeafBBXIterator */

#ifndef _Included_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
#define _Included_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    hasNext
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_hasNext
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    next
 * Signature: ()Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_next
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    key
 * Signature: ()Les/usc/citius/lab/joctomap/octree/JOctreeKey;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_key
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    depth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_depth
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    coordinate
 * Signature: ()Les/usc/citius/lab/motionplanner/core/spatial/Point3D;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_coordinate
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    x
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_x
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    y
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_y
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    z
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_z
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    size
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_size
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    node
 * Signature: ()Les/usc/citius/lab/joctomap/octree/JOctreeNode;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_node
  (JNIEnv *, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_iterators_LeafBBXIterator
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_iterators_LeafBBXIterator_dispose
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif