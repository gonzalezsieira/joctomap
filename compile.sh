#!bin/sh
echo -e "[exec] C/C++ build script begins"
# save current directory
dir=`pwd`

# enter in native packages to compile .c and .cpp sources
cd src/main/resources/joctomap-natives

# compile
echo "[exec] Building libjoctomap.so... (JAVA_JDK="$1")"

# Get release type
release=$2
if [ "$release" == "Debug" ]; then
	echo "Release type: Debug"
	make JAVA=`echo $1` CC_FLAGS="-g"
elif [ "$release" == "Release" ]; then
	echo "Release type: Release"
	make JAVA=`echo $1` CC_FLAGS="-O3"
else
	echo "Release type not recognized, must be Release or Debug"
	exit 1
fi

# Check failure
result=$?
failure=0; [ $result -ne 0 ] && failure=1

# go to beginning directory
cd "$dir"

echo -e "[exec] C/C++ build script ends"

exit $failure
