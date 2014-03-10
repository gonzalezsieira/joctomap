#include  <iostream>
#include <jni.h>
#include "joctomap.h"

using namespace std;

JNIEXPORT void JNICALL Java_es_usc_citius_lab_joctomap_JOctomap_test(JNIEnv *hello, jobject object){
	cout << "Método en C++!\n" << cout;
}
