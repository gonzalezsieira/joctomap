#!/usr/bin/env bash
# Check if destination directory exist
if [ ! -d "$2/target/lib" ]; then
    echo "Creating directory $2/target/lib"
    mkdir -p "$2/target/lib"
fi

if [ ! -d "$2/src/main/resources/lib" ]; then
    echo "Creating directory $2/src/main/resources/lib"
    mkdir -p "$2/src/main/resources/lib"
fi

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    echo "[linux-gnu] copy resources from $1/joctomap-natives/ to $2/target/lib..."
    # Copy to target
    cp $1/joctomap-natives/lib/*.so* $2/target/lib
    cp $1/joctomap-natives/build/libjoctomap_natives.so $2/target

    echo "[linux-gnu] copy resources from $1/joctomap-natives/ to $2/src/main/resources..."
    # Copy to /src/main/resources
    cp $1/joctomap-natives/lib/*.so* $2/src/main/resources/lib
    cp $1/joctomap-natives/build/libjoctomap_natives.so $2/src/main/resources

elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "[darwin*] copy resources from $1/joctomap-natives/ to $2/target/lib..."
    # Copy to target
    cp $1/joctomap-natives/lib/*.dylib* $2/target/lib
    cp $1/joctomap-natives/build/libjoctomap_natives.dylib $2/target/libjoctomap_natives.so

    echo "[darwin*] copy resources from $1/joctomap-natives/ to $2/src/main/resources..."
    # Copy to /src/main/resources
    cp $1/joctomap-natives/lib/*.dylib* $2/src/main/resources/lib
    cp $1/joctomap-natives/build/libjoctomap_natives.dylib $2/src/main/resources/libjoctomap_natives.so
fi

