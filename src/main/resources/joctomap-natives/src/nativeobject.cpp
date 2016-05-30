#include "nativeobject.h"

/*
 * Piece of code to retrieve native pointer.
 */
jlong getPointer(JNIEnv *env, jobject obj){
	//find field to access the OcTree pointer
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/util/NativeObject");
	jfieldID field = env->GetFieldID(cls, "pointer", "J");
	//retrieve value of field
	return env->GetLongField(obj, field);
}

/*
 * Delete native pointer
 */
void deletePointer(JNIEnv *env, jobject obj){
    	//find field to access the OcTree pointer
	jclass cls = env->FindClass("es/usc/citius/lab/joctomap/util/NativeObject");
	jfieldID field = env->GetFieldID(cls, "pointer", "J");
	//retrieve value of field
	env->SetLongField(obj, field, INVALID_POINTER);
}
