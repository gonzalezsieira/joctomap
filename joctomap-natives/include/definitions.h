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
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   definitions.h
 * Author: adrian.gonzalez
 *
 * Created on 31 de mayo de 2016, 18:38
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//Classes
#define CLS_STRING "Ljava/lang/String;"
#define CLS_LEAFBBXITERATOR "Les/usc/citius/lab/joctomap/iterators/LeafBBXIterator;"
#define CLS_POINT3D "Les/usc/citius/lab/motionplanner/core/spatial/Point3D;"
#define CLS_POINT2D "Les/usc/citius/lab/motionplanner/core/spatial/Point2D;"
#define CLS_JOCTREENODE "Les/usc/citius/lab/joctomap/octree/JOctreeNode;"
#define CLS_JOCTREEKEY "Les/usc/citius/lab/joctomap/octree/JOctreeKey;"
#define CLS_JOCTREE "Les/usc/citius/lab/joctomap/octree/JOctree;"
#define CLS_SET "Ljava/util/Set;"
#define CLS_ITERATOR "Ljava/util/Iterator;"
#define CLS_JADJACENCYMAP "Les/usc/citius/lab/joctomap/util/AdjacencyMap;"
#define CLS_NATIVEOBJECT "Les/usc/citius/lab/joctomap/util/NativeObject;"
#define CLS_MAP "Ljava/util/Map;"
#define CLS_PAIR "Les/usc/citius/lab/motionplanner/core/util/Pair;"
#define CLS_FLOAT "Ljava/lang/Float;"
#define CLS_ARRAYLIST "Ljava/util/ArrayList;"
#define CLS_TRANSITION "Les/usc/citius/hipster/model/Transition;"
#define CLS_OBJECT "Ljava/lang/Object;"
#define CLS_QUEUE "Ljava/util/Queue;"

//Fields
#define FIELD_PATH "path"
#define FIELD_X "x"
#define FIELD_Y "y"
#define FIELD_Z "z"
#define FIELD_ADJACENCYMAP_OCTREE "octree"
#define FIELD_ADJACENCYMAP_NODESINFO "nodesInfo"
#define FIELD_ADJACENCYMAP_ADJACENCIES "adjacencies"
#define FIELD_NATIVEOBJECT_POINTER "pointer"
#define FIELD_PAIR_KEY "key"
#define FIELD_PAIR_CONTENT "content"
#define FIELD_VALUE "value"

//Methods
#define METHOD_CONSTRUCTOR "<init>"

//Signatures
#define SIGNATURE_FLOAT "F"
#define SIGNATURE_INT "I"
#define SIGNATURE_LONG "J"

#endif /* DEFINITIONS_H */

