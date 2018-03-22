#!bin/sh

# output directory
mkdir classes

# save current directory
dir=`pwd`
classpath="$dir"/classes:~/.m2/repository/es/usc/citius/spatial-utils/1.1-SNAPSHOT/spatial-utils-1.1-SNAPSHOT.jar:~/.m2/repository/es/usc/citius/hipster/hipster-core/1.0.1/hipster-core-1.0.1.jar:~/.m2/repository/org/ros/rosjava_messages/octomap_msgs/0.3.1/octomap_msgs-0.3.1.jar:~/.m2/repository/org/ros/rosjava_bootstrap/message_generation/0.3.0/message_generation-0.3.0.jar:~/.m2/repository/io/netty/netty/3.10.6.Final/netty-3.10.6.Final.jar:.

# enter in directory where the native methods are
cd "$dir"/joctomap-core/src/main/java/es/usc/citius/lab/joctomap/

# compile files
javac -d "$dir"/classes -cp "$classpath" octree/Cell.java util/JOctomapLogger.java util/AdjacencyMap.java util/Obstacle.java util/JOctreeUtils.java util/NativeUtils.java  util/NativeObject.java iterators/OctreeIterator.java iterators/LeafBBXIterator.java octree/JOctree.java octree/JOctreeKey.java octree/JOctreeNode.java distance/JOctreeDistanceMap.java util/CollisionChecker2D.java

# get into module
cd "$dir"/joctomap-ros/src/main/java/es/usc/citius/lab/joctomap

# compile module
javac -d "$dir"/classes -cp "$classpath" ros/Messages.java

# go to root of src
cd "$dir"/classes

# generate JNI headers
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.octree.JOctree
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.octree.JOctreeNode
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.distance.JOctreeDistanceMap
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.iterators.LeafBBXIterator
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.CollisionChecker2D
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.JOctreeUtils
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.util.AdjacencyMap
javah -classpath "$classpath" -jni es.usc.citius.lab.joctomap.ros.Messages

mv *JOctree.h ../joctomap-natives/include/joctree.h
mv *JOctreeNode.h ../joctomap-natives/include/joctreenode.h
mv *JOctreeDistanceMap.h ../joctomap-natives/include/joctreedistancemap.h
mv *LeafBBXIterator.h ../joctomap-natives/include/leafbbxiterator.h
mv *CollisionChecker2D.h ../joctomap-natives/include/collisionchecker2d.h
mv *JOctreeUtils.h ../joctomap-natives/include/joctreeutils.h
mv *AdjacencyMap.h ../joctomap-natives/include/adjacencymap.h
mv *Messages.h ../joctomap-natives/include/messages.h

# remove current compiled files
rm -rf "$dir"/classes

# go to initial directory
cd "$dir"
