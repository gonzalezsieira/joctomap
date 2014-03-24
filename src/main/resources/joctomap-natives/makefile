# Compiler to use
CC=g++

# Flags to create the shared library
SHARED=-fPIC -shared

# Output file
TARGET=libjoctomap.so

#Octomap library root
OCTOMAP=/home/local/adrian.gonzalez/Dropbox/Trabajo/librerias/octomap-1.6.5

#Linking libraries
LINK=-L"lib/" -loctomap -loctomath

# Includes to use with the compiler
INCLUDE=-I"/usr/lib/jvm/java-7-openjdk-amd64/include/" -I"/usr/lib/jvm/java-7-openjdk-amd64/include/linux/" -I"$(OCTOMAP)/octomap/include/" -I"include/"

all: libjoctomap.so

libjoctomap.so: octomap joctree.o
	$(CC) build/*.o $(SHARED) $(INCLUDE)  $(LINK) -o $(TARGET)  

joctree.o:
	$(CC) -c -fPIC src/joctree.cpp $(INCLUDE) $(LINK) -o build/joctree.o 
	
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