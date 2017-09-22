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
// Created by adrian.gonzalez on 30/08/17.
//

#ifndef JOCTOMAP_NATIVES_TRANSITIONFUNCTIONUTILS_H
#define JOCTOMAP_NATIVES_TRANSITIONFUNCTIONUTILS_H

#include <math.h>
#include <map>
#include <queue>
#include <unordered_map>
#include "definitions.h"
#include "collisions.h"

/**
 * Custom defined type for Point2D.
 */
class Point2D
{

    double coord_x, coord_y;

public:
    Point2D(){
        coord_x = 0;
        coord_y = 0;
    }

    Point2D(point3d point_3d){
        this->coord_x = point_3d.x();
        this->coord_y = point_3d.y();
    }

    Point2D(double x, double y){
        this->coord_x = x; this->coord_y = y;
    }

    ~Point2D() { }

    double x() const { return this->coord_x; }

    double y() const { return this->coord_y; }

    bool operator== (const Point2D &other) const
    {
        return (this->coord_x == other.coord_x)
               && (this->coord_y == other.coord_y);
    }

};

/**
 * Custom defined type for Point3D.
 */
class Point3D
{

    double coord_x, coord_y, coord_z;

public:
    Point3D(){
        coord_x = 0;
        coord_y = 0;
        coord_z = 0;
    }

    Point3D(point3d point_3d){
        this->coord_x = point_3d.x();
        this->coord_y = point_3d.y();
        this->coord_z = point_3d.z();
    }

    Point3D(double x, double y, double z){
        this->coord_x = x; this->coord_y = y; this->coord_z = z;
    }

    ~Point3D() { }

    double x() const { return this->coord_x; }

    double y() const { return this->coord_y; }

    double z() const { return this->coord_z; }

    bool operator== (const Point3D &other) const
    {
        return (this->coord_x == other.coord_x)
               && (this->coord_y == other.coord_y)
               && (this->coord_z == other.coord_z);
    }

};

//hash for std::pair
struct Pair_Hash {

    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }

};

/**
 * Hash definition for Point2D & Point3D
 */
namespace std {

    template <>
    struct hash<Point3D>
    {

        size_t operator()(const Point3D& point) const
        {
            //already defined hash function for doubles
            size_t h1 = 2 * std::hash<double>()(point.x());
            size_t h2 = 3 * std::hash<double>()(point.y());
            size_t h3 = 5 * std::hash<double>()(point.z());
            return  (h1 ^ (h2 << 1)) ^ h3;
        }

    };

    template <>
    struct hash<point3d>
    {

        size_t operator()(const point3d& point) const
        {
            //already defined hash function for doubles
            size_t h1 = 2 * std::hash<double>()(point.x());
            size_t h2 = 3 * std::hash<double>()(point.y());
            size_t h3 = 5 * std::hash<double>()(point.z());
            return  (h1 ^ (h2 << 1)) ^ h3;
        }

    };

    template <>
    struct hash<Point2D>
    {

        std::size_t operator()(const Point2D &point) const
        {
            //already defined hash function for doubles
            size_t h1 = 2 * std::hash<double>()(point.x());
            size_t h2 = 3 * std::hash<double>()(point.y());
            return  (h1 ^ (h2 << 1));
        }

    };

}

struct NodeInfo {

    jobject jkey;
    OcTreeKey key;
    double size;
    point3d coordinate;

    NodeInfo(OcTreeKey key, double size, jobject jkey, point3d coordinate) { this->key = key; this->size = size; this->jkey = jkey; this->coordinate = coordinate; }

};

struct NodeInfo_Adjacencies : NodeInfo{

    jobject jadjacencies;

    NodeInfo_Adjacencies(OcTreeKey key, double size, jobject jkey, point3d coordinate, jobject jadjacencies) : NodeInfo (key, size, jkey, coordinate) { this->jadjacencies = jadjacencies; }

};

/*
 *  Comparison returns true when p1 > p2, doing this the priority queue
 *  will return the lowest element at first
 */
struct ComparePoint3D {

    //comparator argument
    point3d reference;

    //constructor
    ComparePoint3D(point3d reference) { this->reference = reference; }

    //comparison function
    bool operator()(point3d const & p1, point3d const & p2){
        return p1.distance(reference) > p2.distance(reference);
    }
};

struct CompareValues {

    //comparator argument
    float value;

    //constructor
    CompareValues(float value) {this->value = value; }

    //comparison function
    bool operator()(float const & v1, float const & v2){
        return abs(v1 - value) > abs(v2 - value);
    }

};


struct StaticInformation{
    //environment (to release references later)
    JNIEnv *env;

    //other information used by the transition function
    jobject jadjacencymap;
    float radius_optimistic;
    float min_resolution_trajectories;

    //static information
    OcTree* octree;

    //Java classes
    jclass cls_adjacencymap;
    jclass cls_float;
    jclass cls_joctreekey;
    jclass cls_arraylist;
    jclass cls_transition;
    jclass cls_pair;
    jclass cls_map;
    jclass cls_point3d;

    //java method
    jmethodID method_adjacency;
    jmethodID method_nodeinfo;
    jmethodID method_constructor_arraylist;
    jmethodID method_constructor_joctreekey;
    jmethodID method_create_transition;
    jmethodID method_constructor_float;
    jmethodID method_map_get;
    jmethodID method_get_arraylist;
    jmethodID method_size_arraylist;
    jmethodID method_add_arraylist;

    //java fields ID
    jfieldID field_joctreekey_x;
    jfieldID field_joctreekey_y;
    jfieldID field_joctreekey_z;
    jfieldID field_pair_first;
    jfieldID field_pair_second;
    jfieldID field_float_value;
    jfieldID field_point3d_x;
    jfieldID field_point3d_y;
    jfieldID field_point3d_z;

    //auxiliary information
    int maxdepth;
    float maxdepthsize;


    StaticInformation(JNIEnv *env, long octree_pointer, jobject jadjacencymap, float radius_optimistic, float min_resolution_trajectories){
        //java java excution environment
        this->env = env;
        //variables
        this->jadjacencymap = (jobject) env->NewGlobalRef(jadjacencymap);
        this->radius_optimistic = radius_optimistic;
        this->min_resolution_trajectories = min_resolution_trajectories;

        //retrieve native objects
        this->octree = (OcTree*) octree_pointer;

        //LOCAL REFERENCES
        jclass cls_adjacencymap_local = env->FindClass(CLS_JADJACENCYMAP);
        jclass cls_float_local = env->FindClass(CLS_FLOAT);
        jclass cls_joctreekey_local = env->FindClass(CLS_JOCTREEKEY);
        jclass cls_arraylist_local = env->FindClass(CLS_ARRAYLIST);
        jclass cls_transition_local = env->FindClass(CLS_TRANSITION);
        jclass cls_pair_local = env->FindClass(CLS_PAIR);
        jclass cls_map_local = env->FindClass(CLS_MAP);
        jclass cls_point3d_local = env->FindClass(CLS_POINT3D);

        //CONVERT TO GLOBAL REFERENCES
        this->cls_adjacencymap = (jclass) env->NewGlobalRef(cls_adjacencymap_local);
        this->cls_float = (jclass) env->NewGlobalRef(cls_float_local);
        this->cls_joctreekey = (jclass) env->NewGlobalRef(cls_joctreekey_local);
        this->cls_arraylist = (jclass) env->NewGlobalRef(cls_arraylist_local);
        this->cls_transition = (jclass) env->NewGlobalRef(cls_transition_local);
        this->cls_pair = (jclass) env->NewGlobalRef(cls_pair_local);
        this->cls_map = (jclass) env->NewGlobalRef(cls_map_local);
        this->cls_point3d = (jclass) env->NewGlobalRef(cls_point3d_local);

        //DELETE LOCAL REFERENCES (no longer used)
        env->DeleteLocalRef(cls_adjacencymap_local);
        env->DeleteLocalRef(cls_arraylist_local);
        env->DeleteLocalRef(cls_joctreekey_local);
        env->DeleteLocalRef(cls_pair_local);
        env->DeleteLocalRef(cls_transition_local);
        env->DeleteLocalRef(cls_map_local);
        env->DeleteLocalRef(cls_float_local);
        env->DeleteLocalRef(cls_point3d_local);

        //retrieve argument-passed object methods
        this->method_adjacency = env->GetMethodID(cls_adjacencymap, "adjacency", "(Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Ljava/util/List;");
        this->method_nodeinfo = env->GetMethodID(cls_adjacencymap, "nodeInfo", "(Les/usc/citius/lab/joctomap/octree/JOctreeKey;)Les/usc/citius/lab/motionplanner/core/util/Pair;");

        //retrieve constructors of classes used in this structure
        this->method_constructor_arraylist = env->GetMethodID(cls_arraylist, METHOD_CONSTRUCTOR, "()V");
        this->method_constructor_joctreekey = env->GetMethodID(cls_joctreekey, METHOD_CONSTRUCTOR, "(III)V");
        this->method_create_transition = env->GetStaticMethodID(cls_transition, "create", "(Ljava/lang/Object;Ljava/lang/Object;)Les/usc/citius/hipster/model/Transition;");
        this->method_constructor_float = env->GetMethodID(cls_float, METHOD_CONSTRUCTOR, "(F)V");
        this->method_map_get = env->GetMethodID(cls_map, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

        //retrieve fields used by classes in this method
        this->field_joctreekey_x = env->GetFieldID(cls_joctreekey, FIELD_X, SIGNATURE_INT);
        this->field_joctreekey_y = env->GetFieldID(cls_joctreekey, FIELD_Y, SIGNATURE_INT);
        this->field_joctreekey_z = env->GetFieldID(cls_joctreekey, FIELD_Z, SIGNATURE_INT);
        this->field_pair_first = env->GetFieldID(cls_pair, FIELD_PAIR_KEY, CLS_OBJECT);
        this->field_pair_second = env->GetFieldID(cls_pair, FIELD_PAIR_CONTENT, CLS_OBJECT);
        this->field_float_value = env->GetFieldID(cls_float, FIELD_VALUE, SIGNATURE_FLOAT);
        this->field_point3d_x = env->GetFieldID(cls_point3d, FIELD_X, SIGNATURE_FLOAT);
        this->field_point3d_y = env->GetFieldID(cls_point3d, FIELD_Y, SIGNATURE_FLOAT);
        this->field_point3d_z = env->GetFieldID(cls_point3d, FIELD_Z, SIGNATURE_FLOAT);

        //retrieve methods of classes used in this code
        this->method_get_arraylist = env->GetMethodID(cls_arraylist, "get", "(I)Ljava/lang/Object;");
        this->method_size_arraylist = env->GetMethodID(cls_arraylist, "size", "()I");
        this->method_add_arraylist = env->GetMethodID(cls_arraylist, "add", "(Ljava/lang/Object;)Z");

        //calculate max depth
        this->maxdepth = octree->getTreeDepth();
        this->maxdepthsize = this->octree->getNodeSize(maxdepth);
        while((this->maxdepthsize / 2) < min_resolution_trajectories){
            maxdepth--;
            maxdepthsize = octree->getNodeSize(maxdepth);
        }

    }

    ~StaticInformation(){

        env->DeleteGlobalRef(cls_adjacencymap);
        env->DeleteGlobalRef(cls_arraylist);
        env->DeleteGlobalRef(cls_joctreekey);
        env->DeleteGlobalRef(cls_pair);
        env->DeleteGlobalRef(cls_transition);
        env->DeleteGlobalRef(cls_map);
        env->DeleteGlobalRef(cls_float);
        env->DeleteGlobalRef(jadjacencymap);
        env->DeleteGlobalRef(cls_point3d);

    }

};

//define priority queue
typedef std::priority_queue<point3d, std::vector<point3d>, ComparePoint3D> point3d_priorityqueue;

typedef std::priority_queue<float, std::vector<float>, CompareValues> float_priorityqueue;

float closestOrientationTo(std::list<float> neighbors_orientations, float value);

float_priorityqueue closestOrientations(std::list<float> neighbors_orientations, float value);

NodeInfo node_info(StaticInformation* information, OcTreeKey key, jobject jkey);

NodeInfo_Adjacencies search_node(StaticInformation* information, point3d point);


#endif //JOCTOMAP_NATIVES_TRANSITIONFUNCTIONUTILS_H
