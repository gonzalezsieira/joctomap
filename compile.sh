#!bin/sh
echo -e "[exec] C/C++ build script begins"
# save current directory
dir=`pwd`

# enter in native packages to compile .c and .cpp sources
cd joctomap-natives

# compile
echo "[exec] Building libjoctomap.so... (JAVA_JDK="$1")"
mkdir build
cd build
cmake ../ -DJDK:STRING="$1"

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
		cp build/libjoctomap_natives.so ../src/main/resources/
		mkdir ../src/main/resources/lib
		cp lib/*.a ../src/main/resources/lib

		# go to beginning directory
		cd "$dir"

		echo -e "[exec] C/C++ build script ends"
		result=0
	fi

fi

exit $failure


