# Compiler to use
CC=g++ -std=c++0x

# Flags to create the shared library
SHARED=-shared -fPIC

# Flags to generate debug information
DEBUG=-g

# Flags to copmpile single files
FILE=-c -fPIC

# Output file
TARGET=libjoctomap.so

# Java HOME
JAVA=/usr/lib/jvm/java-7-oracle

#Octomap library root
OCTOMAP=../../../../../../Trabajo/librerias/octomap-1.6.5

#Linking libraries
LINK=-L"lib/" -loctomath -loctomap

# Includes to use with the compiler
INCLUDE=-I"$(JAVA)/include/" -I"$(JAVA)/include/linux/" -I"$(OCTOMAP)/octomap/include/" -I"include/"

all: libjoctomap.so

libjoctomap.so: octomap joctree.o
	$(CC) $(DEBUG) build/*.o $(SHARED) $(INCLUDE)  $(LINK) -o $(TARGET)  

joctree.o:
	$(CC) $(DEBUG) $(FILE) src/joctree.cpp $(INCLUDE) $(LINK) -o build/joctree.o 
	
octomap:
	cd $(OCTOMAP)/octomap/; \
	mkdir build; \
	cd build; \
	cmake ..; \
	make;
	cp $(OCTOMAP)/octomap/lib/*.a lib/
	
clean:
	rm -rf *.o 
	rm -rf $(TARGET)
	rm -rf lib/*
	rm -rf build/*