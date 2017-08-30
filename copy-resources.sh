#!/usr/bin/env bash
echo "Copy resources to target/lib..."
if [[ "$OSTYPE" == "linux-gnu" ]]; then
    cp joctomap-natives/lib/*.so* target/lib
elif [[ "$OSTYPE" == "darwin"* ]]; then
    cp joctomap-natives/lib/*.dylib* target/lib
fi
