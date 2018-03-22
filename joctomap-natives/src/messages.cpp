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
//
// Created by adrian.gonzalez on 19/03/18.
//

#include "messages.h"
#include "definitions.h"
#include <sstream>
#include <octomap/AbstractOcTree.h>
#include <octomap/ColorOcTree.h>
#include <octomap/OcTree.h>

/*
 * Class:     es_usc_citius_lab_joctomap_ros_Messages
 * Method:    convert
 * Signature: (Loctomap_msgs/Octomap;)Les/usc/citius/lab/joctomap/octree/JOctree;
 */
JNIEXPORT jobject JNICALL Java_es_usc_citius_lab_joctomap_ros_Messages_convert
        (JNIEnv *env, jclass jclassMessages, jstring jid, jdouble resolution, jboolean binary, jbyteArray bytes){

    //find class
    jclass jclassMsg = env->FindClass("octomap_msgs/Octomap");
    jclass jclassJoctree = env->FindClass(CLS_JOCTREE);
    //find constructor to instantiate the result
    jmethodID constructor = env->GetMethodID(jclassJoctree, METHOD_CONSTRUCTOR, "(J)V");

    //convert data type
    std::stringstream datastream;
    jsize len = env->GetArrayLength(bytes);
    //convert jstring to std::string
    jboolean isCopy;
    const char* idchars = env->GetStringUTFChars(jid, &isCopy);
    std::string id(idchars, env->GetStringLength(jid));
    if (len > 0) {
        //copy data in buffer
        char *buf = new char[len];
        env->GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte *>(buf));
        //write datastream
        datastream.write((const char *) buf, len);

        //create octree
        octomap::AbstractOcTree* tree;
        if (!binary) {
            tree = octomap::AbstractOcTree::createTree(id, resolution);
            tree->readData(datastream);
        }
        else {
            if (id == "ColorOcTree"){
                octomap::ColorOcTree* octree = new octomap::ColorOcTree(resolution);
                octree->readBinaryData(datastream);
                tree = octree;
            } else {
                octomap::OcTree* octree = new octomap::OcTree(resolution);
                octree->readBinaryData(datastream);
                tree = octree;
            }
        }
        jobject jtree = env->NewObject(jclassJoctree, constructor, tree);
        return jtree;
    }
    else{
        // exception?
    }
    //release references
    env->DeleteLocalRef(jclassMsg);
    env->DeleteLocalRef(jclassJoctree);
    env->ReleaseStringUTFChars(jid, idchars);
}
