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
/* Header for class es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction */

#ifndef _Included_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
#define _Included_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    initialize
 * Signature: (JLes/usc/citius/lab/joctomap/util/AdjacencyMap;FFLjava/lang/Object;)J
 */
JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_initialize
  (JNIEnv *, jobject, jlong, jobject, jfloat, jfloat, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    transitionsFrom
 * Signature: (Les/usc/citius/lab/motionplanner/core/spatial/Point3D;)Ljava/lang/Iterable;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_transitionsFrom
  (JNIEnv *, jobject, jobject);

/*
 * Class:     es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_hipster_H3DMRTransitionFunction_dispose
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
