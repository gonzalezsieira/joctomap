#!bin/sh
echo -e "[exec] C/C++ build script begins"

# save current directory
dir=`pwd`

# enter in resource directory to compile the library
cd src/main/resources/octomap

# octomap compile
echo -e "[exec] Building Octomap..."
mkdir build
cd build
cmake ..
make

# go to beginning directory
cd "$dir"

# enter in native packages to compile .c and .cpp sources
cd src/main/java/es/usc/citius/lab/joctomap/

# compile
echo "[exec] Building libjoctomap.so..."
g++ -I"/usr/lib/jvm/java-7-openjdk-amd64/include/" -I"/usr/lib/jvm/java-7-openjdk-amd64/include/linux/" -o "$dir"/libjoctomap.so -fPIC -shared *.cpp

# go to beginning directory
cd "$dir"

echo -e "[exec] C/C++ build script ends"