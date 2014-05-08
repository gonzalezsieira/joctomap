#!bin/sh
echo -e "[exec] C/C++ build script begins"
# save current directory
dir=`pwd`

# enter in native packages to compile .c and .cpp sources
cd src/main/resources/joctomap-natives

# compile
echo "[exec] Building libjoctomap.so..."
make JAVA=`echo $JAVA_JDK`
cp libjoctomap.so "$dir"/

# go to beginning directory
cd "$dir"

echo -e "[exec] C/C++ build script ends"