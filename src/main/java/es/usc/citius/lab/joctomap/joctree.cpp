#include  <iostream>
#include <jni.h>
#include "joctree.h"

using namespace std;

JNIEXPORT jboolean JNICALL Java_es_usc_citius_lab_joctomap_JOctree_write
  (JNIEnv *env, jobject obj , jlong octreePointer, jstring filename){
	cout << "WRITE";
}

JNIEXPORT jlong JNICALL Java_es_usc_citius_lab_joctomap_JOctree_read
  (JNIEnv *env, jobject obj, jstring filename){
	cout << "READ";
}
