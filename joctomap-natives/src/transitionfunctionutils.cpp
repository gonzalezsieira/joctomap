#include <jni.h>
#include "transitionfunctionutils.h"

Point2D jpoint2d_to_Point2D(jobject jpoint2d, JNIEnv* env, jfieldID field_x, jfieldID field_y){
    //retrieve argument-passed object field values
    jfloat point2d_x = env->GetFloatField(jpoint2d, field_x);
    jfloat point2d_y = env->GetFloatField(jpoint2d, field_y);
    //return value
    return Point2D(point2d_x, point2d_y);
}


void frontier_points2d(float size_cell, Point2D center, priorityqueue &queue){
    float diff = size_cell / 4.0;
    queue.push(point3d(center.x() - diff, center.y() - diff, 0));
    queue.push(point3d(center.x() - diff, center.y() + diff, 0));
    queue.push(point3d(center.x() + diff, center.y() + diff, 0));
    queue.push(point3d(center.x() + diff, center.y() - diff, 0));
}

NodeInfo node_info(StaticInformation* information, OcTreeKey key, jobject jkey){
    //get node info from java object
    jobject jnode_info = information->env->CallObjectMethod(information->jadjacencymap, information->method_nodeinfo, jkey);
    //get key value
    jobject size_float = information->env->GetObjectField(jnode_info, information->field_pair_first);
    //get Float value
    jfloat size = information->env->GetFloatField(size_float, information->field_float_value);
    //get coordinate value
    jobject jpoint2d = information->env->GetObjectField(jnode_info, information->field_pair_second);
    //transform point3d from java to C++
    Point2D point2d = jpoint2d_to_Point2D(jpoint2d, information->env, information->field_jpoint2d_x, information->field_jpoint2d_y);
    //convert float to
    return NodeInfo(key, size, jkey, point2d);

}

NodeInfo_Adjacencies search_node(StaticInformation* information, point3d point){
    //get max depth
    int depth = information->octree->getTreeDepth() + 1;
    OcTreeKey key;
    jobject jkey, jadjacencies;
    //this loop breaks when a key in the adjacency map is found (means that it exists in the tree)
    do{
        //find
        depth--;
        key = information->octree->coordToKey(point, depth);
        //instantiate key in java
        jkey = information->env->NewObject(information->cls_joctreekey, information->method_constructor_joctreekey, key.k[0], key.k[1], key.k[2]);
        jadjacencies = information->env->CallObjectMethod(information->jadjacencymap, information->method_adjacency, jkey);
    } while(depth > 0 && jadjacencies == NULL);
    return NodeInfo_Adjacencies( key, information->octree->getNodeSize(depth), jkey, Point2D(information->octree->keyToCoord(key, depth)), jadjacencies );

}

bool isInBounds(double octree_min_x, double octree_min_y, double octree_min_z, double octree_max_x, double octree_max_y, double octree_max_z, point3d point){
    return point.x() > octree_min_x
           && point.x() < octree_max_x
           && point.y() > octree_min_y
           && point.y() < octree_max_y
           && point.z() > octree_min_z
           && point.z() < octree_max_z;
}

float closestOrientationTo(std::list<float> neighbors_orientations, float value){
    float closestori = -1;
    float diff = std::numeric_limits<float>::infinity();
    for (std::list<float>::iterator list_iterator = neighbors_orientations.begin(); list_iterator != neighbors_orientations.end(); list_iterator++){
        float actualdiff = fabs(value - *list_iterator);
        if(actualdiff < diff){
            closestori = *list_iterator;
            diff = actualdiff;
        }
    }
    //switch to PI instead of -PI if needed
    if(fabs(-M_PI - closestori) < 0.0001){
        closestori = M_PI;
    }
    return closestori;
}

bool checkCollision2d_Cached(StaticInformation* information, point3d point){
    Point2D point_2d = Point2D(point);
    //not found, calculate and cache
    bool result;
    if(information->cache_collisions.find(point_2d) == information->cache_collisions.end()){
        result = checkCollision(point, information->radius_optimistic, information->octree);
        information->cache_collisions[point_2d] = result;
    }
    else{
        result = information->cache_collisions[point_2d];
    }
    return result;
}