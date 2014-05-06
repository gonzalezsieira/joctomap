######################################################################################################
# 									PARAMETERS														 #
######################################################################################################	
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
OCTOMAP=octomap

#dynamicEDT3D library root
DYNAMIC=dynamicEDT3D

#Linking libraries
LINK=-L"lib/" -loctomath -loctomap

# Includes to use with the compiler
INCLUDE=-I"$(JAVA)/include/" -I"$(JAVA)/include/linux/" -I"$(OCTOMAP)/include/" -I"include/"

######################################################################################################
# 									SHARED LIBRARY BUILD											 #
######################################################################################################	
.PHONY : octomap
.PHONY : clean

all: libjoctomap.so

libjoctomap.so: octomap dynamic joctree.o joctreenode.o
	$(CC) $(DEBUG) build/*.o $(SHARED) $(INCLUDE)  $(LINK) -o $(TARGET)  

######################################################################################################
# 									NATIVE IMPLEMENTATIONS BUILD									 #
######################################################################################################	
joctree.o: nativeobject.o
	$(CC) $(DEBUG) $(FILE) src/joctree.cpp $(INCLUDE) $(LINK) -o build/joctree.o 
	
joctreenode.o: nativeobject.o
	$(CC) $(DEBUG) $(FILE) src/joctreenode.cpp $(INCLUDE) $(LINK) -o build/joctreenode.o 

######################################################################################################
# 									NATIVE OBJECTS DEPENDENT FILES 		 							 #
######################################################################################################	
nativeobject.o: src/nativeobject.cpp include/nativeobject.h
	$(CC) $(DEBUG) $(FILE) src/nativeobject.cpp $(INCLUDE) $(LINK) -o build/nativeobject.o

######################################################################################################
# 									OCTOMAP BUILD													 #
######################################################################################################	
octomap:
	mkdir lib; \
	mkdir build; \
	mkdir build/octomap; \
	cd build/octomap; \
	cmake ../../$(OCTOMAP); \
	make; \
	cp ../../$(OCTOMAP)/lib/*.a ../../lib/
	
dynamic:
	mkdir lib; \
	mkdir build; \
	mkdir build/dynamicEDT3D; \
	cd build/dynamicEDT3D; \
	cmake ../../$(DYNAMIC); \
	make; \
	cp ../../$(DYNAMIC)/lib/*.a ../../lib/
	
######################################################################################################
# 									CLEAN RULE														 #
######################################################################################################	
clean:
	rm -rf $(TARGET)
	rm -rf lib
	rm -rf build
	rm -rf $(OCTOMAP)/lib
	rm -rf $(OCTOMAP)/bin
	rm -rf $(DYNAMIC)/lib
	rm -rf $(DYNAMIC)/bin