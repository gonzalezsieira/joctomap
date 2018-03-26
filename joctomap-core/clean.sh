#!bin/sh

# remove octomap build
echo -e "[joctomap-core] cleaning $1/joctomap-natives build directories..."
rm -rf ../joctomap-natives/build
rm -rf ../joctomap-natives/lib
rm -rf ../joctomap-natives/bin

# remove resources
echo -e "[joctomap-core] cleaning $2/src/main/resources directories..."
rm -rf $2/src/main/resources/lib
rm -f $2/src/main/resources/*.so*

echo -e "[joctomap-core] finished cleaning resources"