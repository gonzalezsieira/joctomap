#!bin/sh

# save current directory
dir=`pwd`
classpath=~/.m2/repository/es/usc/citius/lab/motionplanner-core/1.0.0-SNAPSHOT/motionplanner-core-1.0.0-SNAPSHOT.jar:~/.m2/repository/es/usc/citius/hipster/hipster-core/1.0.1/hipster-core-1.0.1.jar:.

# enter in directory where the native methods are
cd src/main/java/es/usc/citius/lab/joctomap/octree

# compile files
javac -cp "$classpath" ../octree/Cell.java ../util/JOctomapLogger.java ../util/AdjacencyMap.java ../util/Obstacle.java ../util/JOctreeUtils.java ../util/NativeUtils.java  ../util/NativeObject.java ../iterators/OctreeIterator.java ../iterators/LeafBBXIterator.java JOctree.java JOctreeKey.java JOctreeNode.java ../distance/JOctreeDistanceMap.java ../util/CollisionChecker2D.java ../hipster/H2DMRTransitionFunction.java


# go to root of src
cd "$dir"/src/main/java

# generate JNI headers
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.octree.JOctree
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.octree.JOctreeNode
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.distance.JOctreeDistanceMap
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.iterators.LeafBBXIterator
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.CollisionChecker2D
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.JOctreeUtils
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.AdjacencyMap
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.hipster.H2DMRTransitionFunction

mv *JOctree.h ../../../joctomap-natives/include/joctree.h
mv *JOctreeNode.h ../../../joctomap-natives/include/joctreenode.h
mv *JOctreeDistanceMap.h ../../../joctomap-natives/include/joctreedistancemap.h
mv *LeafBBXIterator.h ../../../joctomap-natives/include/leafbbxiterator.h
mv *CollisionChecker2D.h ../../../joctomap-natives/include/collisionchecker2d.h
mv *JOctreeUtils.h ../../../joctomap-natives/include/joctreeutils.h
mv *AdjacencyMap.h ../../../joctomap-natives/include/adjacencymap.h
mv *H2DMRTransitionFunction.h ../../../joctomap-natives/include/h2dmrtransitionfunction.h
rm *AdjacencyMap_Cache.h

# remove current compiled files
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/octree
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/util
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/hipster
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/distance
rm *.class
cd "$dir"/src/main/java/es/usc/citius/lab/joctomap/iterators
rm *.class

# go to initial directory
cd "$dir"
