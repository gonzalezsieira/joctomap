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
/* Header for class es_usc_citius_lab_joctomap_ros_Messages */

#ifndef _Included_es_usc_citius_lab_joctomap_ros_Messages
#define _Included_es_usc_citius_lab_joctomap_ros_Messages
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     es_usc_citius_lab_joctomap_ros_Messages
 * Method:    getJOctreeBytes
 * Signature: (Les/usc/citius/lab/joctomap/octree/JOctree;Z)[B
 */
JNIEXPORT jbyteArray JNICALL Java_es_usc_citius_lab_joctomap_ros_Messages_getJOctreeBytes
  (JNIEnv *, jclass, jobject, jboolean);

/*
 * Class:     es_usc_citius_lab_joctomap_ros_Messages
 * Method:    convert
 * Signature: (Ljava/lang/String;DZ[B)Les/usc/citius/lab/joctomap/octree/JOctree;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_ros_Messages_convert
  (JNIEnv *, jclass, jstring, jdouble, jboolean, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif
