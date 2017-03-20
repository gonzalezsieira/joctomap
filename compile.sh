#!bin/sh
echo -e "[exec] C/C++ build script begins"
# save current directory
dir=`pwd`

# enter in native packages to compile .c and .cpp sources
cd joctomap-natives

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

	# Make native library
	make

	# Check failure (after cmake)
	result=$?
	failure=0; [ $result -ne 0 ] && failure=1

	if [ "$result" == 0 ]; then
		cd ..

		# Copy target
		mkdir ../src/main/resources/
		if [[ "$OSTYPE" == "linux-gnu" ]]; then
            cp build/libjoctomap_natives.so ../src/main/resources/
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            cp build/libjoctomap_natives.dylib ../src/main/resources/libjoctomap_natives.so
        fi

		# go to beginning directory
		cd "$dir"

		echo -e "[exec] C/C++ build script ends"
		result=0
	fi

fi

exit $failure


