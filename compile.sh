#!bin/sh
echo -e "[exec] C/C++ build script begins"

# save current directory
dir=`pwd`

# enter in resource directory to compile the library
echo -e "[exec] Entering Octomap directory..."
cd src/main/resources/joctomap-natives

# octomap compile
echo -e "[exec] Building Octomap..."
mkdir build
cd build
cmake ..
make

# go to beginning directory
cd "$dir"

# enter in native packages to compile .c and .cpp sources
cd src/main/resources/joctomap-natives

# compile
echo "[exec] Building libjoctomap.so..."
make
cp libjoctomap.so "$dir"/

# go to beginning directory
cd "$dir"

echo -e "[exec] C/C++ build script ends"