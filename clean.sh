#!bin/sh
echo -e "[exec] C/C++ clean script begins"

# save current directory
dir=`pwd`

# remove octomap build
rm -rf joctomap-natives/build
rm -rf joctomap-natives/lib
rm -rf joctomap-natives/bin
rm src/main/resources/libjoctomap_natives.so


echo -e "[exec] C/C++ clean script ends"