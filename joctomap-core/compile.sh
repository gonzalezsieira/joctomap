#!bin/sh
# save current directory
dir=`pwd`
echo -e "[exec] C/C++ build script begins. Base dir is $dir"

# enter in native packages to compile .c and .cpp sources
cd ../joctomap-natives

# Start compiling
echo "[exec] Building libjoctomap.so... (JAVA_JDK="$1")"
mkdir build
cd build

# Get release type
release=$2
if [ "$release" == "Debug" ]; then
	echo "Release type: Debug"	
	cmake .. -DJDK:STRING="$1" -DCMAKE_BUILD_TYPE=Debug
elif [ "$release" == "Release" ]; then
	echo "Release type: Release" 
	cmake .. -DJDK:STRING="$1" -DCMAKE_BUILD_TYPE=Release
else
	echo "Release type not recognized, must be Release or Debug"
	exit 1
fi

# Check failure (after cmake)
result=$?
failure=0; [ $result -ne 0 ] && failure=1

if [ "$result" == 0 ]; then

    echo "Building shared library: libjoctomap_natives"

	# Make native library
	make

	# Check failure (after cmake)
	result=$?
	failure=0; [ $result -ne 0 ] && failure=1

fi

cd $dir

exit $failure


