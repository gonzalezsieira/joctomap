#!bin/sh

# save current directory
dir=`pwd`

# enter in directory where the native methods are
cd src/main/java/es/usc/citius/lab/joctomap/octree

# compile files
javac ../util/Point3D.java ../util/Vector3D.java ../util/Obstacle.java ../util/NativeObject.java ../iterators/LeafBBXIterator.java JOctree.java JOctreeKey.java JOctreeNode.java ../distance/JOctreeDistanceMap.java

# go to root of src
cd "$dir"/src/main/java

# generate JNI headers
javah -jni es.usc.citius.lab.joctomap.octree.JOctree
javah -jni es.usc.citius.lab.joctomap.octree.JOctreeNode
javah -jni es.usc.citius.lab.joctomap.distance.JOctreeDistanceMap

# remove current compiled files
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/octree
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/util
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/distance
rm *.class

# move headers
cd "$dir/src/main/java"
mv *JOctree.h ../resources/joctomap-natives/include/joctree.h
mv *JOctreeNode.h ../resources/joctomap-natives/include/joctreenode.h
mv *JOctreeDistanceMap.h ../resources/joctomap-natives/include/joctreedistancemap.h

# go to initial directory
cd "$dir"
