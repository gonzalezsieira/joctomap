#!bin/sh
echo -e "[exec] C/C++ clean script begins"

# remove octomap build
rm -rf src/main/resources/octomap/build

# remove dynamic library build
rm *.so

echo -e "[exec] C/C++ clean script ends"