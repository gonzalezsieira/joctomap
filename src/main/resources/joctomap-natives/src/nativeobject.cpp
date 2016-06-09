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
#include "nativeobject.h"
#include "definitions.h"

/*
 * Piece of code to retrieve native pointer.
 */
jlong getPointer(JNIEnv *env, jobject obj){
	//find field to access the OcTree pointer
	jclass cls = env->FindClass(CLS_NATIVEOBJECT);
	jfieldID field = env->GetFieldID(cls, FIELD_NATIVEOBJECT_POINTER, SIGNATURE_LONG);
	//retrieve value of field
	return env->GetLongField(obj, field);
}

/*
 * Delete native pointer
 */
void deletePointer(JNIEnv *env, jobject obj){
    	//find field to access the OcTree pointer
	jclass cls = env->FindClass(CLS_NATIVEOBJECT);
	jfieldID field = env->GetFieldID(cls, FIELD_NATIVEOBJECT_POINTER, SIGNATURE_LONG);
	//retrieve value of field
	env->SetLongField(obj, field, INVALID_POINTER);
}
