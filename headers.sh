#!bin/sh

# save current directory
dir=`pwd`

# enter in directory where the native methods are
cd src/main/java/es/usc/citius/lab/joctomap

# compile files
javac JOctree.java

# go to root of src
cd "$dir"
cd src/main/java

# generate JNI headers
javah -jni es.usc.citius.lab.joctomap.JOctree

# remove current headers and compiled files
cd es/usc/citius/lab/joctomap
rm *.h
rm *.class

# move headers
cd "$dir"/src/main/java
mv *.h es/usc/citius/lab/joctomap/joctree.h

# go to initial directory
cd "$dir"