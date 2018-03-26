#!bin/sh
echo -e "[joctomap-ros] C/C++ clean script begins"

# remove resources
echo -e "[joctomap-ros] cleaning $2/src/main/resources directories..."
rm -rf $2/src/main/resources/lib
rm -f $2/src/main/resources/*.so*


echo -e "[joctomap-ros] finished cleaning resources"