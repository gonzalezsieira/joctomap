/*
 * nativeobject.h
 *
 *  Created on: 11/04/2014
 *      Author: adrian.gonzalez
 */

#include <jni.h>

#ifndef NATIVEOBJECT_H_
#define NATIVEOBJECT_H_

const long INVALID_POINTER = -1;

/**
 * Retrieves the native pointer stored in the Java object, accessing the field of
 * the class.
 */
jlong getPointer(JNIEnv *, jobject);

/**
 * Deletes the native pointer stored in the java object.
 */
void deletePointer(JNIEnv *, jobject);


#endif /* NATIVEOBJECT_H_ */
