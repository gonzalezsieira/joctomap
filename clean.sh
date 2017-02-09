#!bin/sh
echo -e "[exec] C/C++ clean script begins"

# save current directory
dir=`pwd`

# remove octomap build
rm -rf joctomap-natives/build
rm -rf joctomap-natives/lib
rm -rf joctomap-natives/bin
rm -rf src/main/resources/lib
rm src/main/resources/libjoctomap.so


echo -e "[exec] C/C++ clean script ends"