#!bin/sh

# save current directory
dir=`pwd`

# enter in directory where the native methods are
cd src/main/java/es/usc/citius/lab/joctomap

# compile files
javac JOctree.java

# go to root of src
cd "$dir"/src/main/java

# generate JNI headers
javah -jni es.usc.citius.lab.joctomap.JOctree

# remove current compiled files
cd es/usc/citius/lab/joctomap
rm *.class

# remove current headers
cd "$dir"/src/main/resources/joctomap-natives/include
rm *.h

# move headers
cd "$dir/src/main/java"
mv *JOctree.h ../resources/joctomap-natives/include/joctree.h

# go to initial directory
cd "$dir"