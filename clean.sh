#!bin/sh
echo -e "[exec] C/C++ clean script begins"

# save current directory
dir=`pwd`

# remove octomap build
cd src/main/resources/joctomap-natives
make clean


echo -e "[exec] C/C++ clean script ends"