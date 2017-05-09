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
