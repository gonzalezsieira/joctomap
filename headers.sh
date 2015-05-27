#!bin/sh

# save current directory
dir=`pwd`
classpath=~/.m2/repository/es/usc/citius/lab/motionplanner-core/1.0/motionplanner-core-1.0.jar:.

# enter in directory where the native methods are
cd src/main/java/es/usc/citius/lab/joctomap/octree

# compile files
javac -cp "$classpath" ../util/Obstacle.java ../util/JOctreeUtils.java ../util/NativeUtils.java  ../util/NativeObject.java ../iterators/OctreeIterator.java ../iterators/LeafBBXIterator.java JOctree.java JOctreeKey.java JOctreeNode.java ../distance/JOctreeDistanceMap.java ../util/CollisionChecker2D.java


# go to root of src
cd "$dir"/src/main/java

# generate JNI headers
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.octree.JOctree
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.octree.JOctreeNode
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.distance.JOctreeDistanceMap
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.iterators.LeafBBXIterator
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.CollisionChecker2D
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.JOctreeUtils

# remove current compiled files
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/octree
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/util
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/distance
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/iterators
rm *.class

# move headers
cd "$dir/src/main/java"
mv *JOctree.h ../resources/joctomap-natives/include/joctree.h
mv *JOctreeNode.h ../resources/joctomap-natives/include/joctreenode.h
mv *JOctreeDistanceMap.h ../resources/joctomap-natives/include/joctreedistancemap.h
mv *LeafBBXIterator.h ../resources/joctomap-natives/include/leafbbxiterator.h
mv *CollisionChecker2D.h ../resources/joctomap-natives/include/collisionchecker2d.h
mv *JOctreeUtils.h ../resources/joctomap-natives/include/joctreeutils.h

# go to initial directory
cd "$dir"
