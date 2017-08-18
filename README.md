# Joctomap - A Java wrapper for Octomap

[Octomap](http://octomap.github.io) is a C++ framework to generate, update and manage 3D occupancy maps. These maps are 
multi-resolution and widely used in mapping and robotic applications, among others.

Joctomap is a Java wrapper for the Octomap mapping framework. Provides access to the main functionalities
of the library and makes possible its usage in Java/Android projects. It maintains
the structure and philosophy of the parent project, also adding several functions which might be interesting
for planning and navigation problems.

## Build status 
 - Branch 1.7.X [![Build Status](https://travis-ci.org/gonzalezsieira/joctomap.svg?branch=1.7.X)](https://travis-ci.org/gonzalezsieira/joctomap)
 - Branch 1.8.X [![Build Status](https://travis-ci.org/gonzalezsieira/joctomap.svg?branch=1.8.X)](https://travis-ci.org/gonzalezsieira/joctomap)
 - Branch 1.9.X [![Build Status](https://travis-ci.org/gonzalezsieira/joctomap.svg?branch=1.9.X)](https://travis-ci.org/gonzalezsieira/joctomap)

## How do I use it?
This project depends on spatial-utils, a lightweight library for geometrical 
operations in cartesian coordinate systems. This library has to be compiled first, since
it is not uploaded to Maven Central yet:
```bash
git clone https://github.com/gonzalezsieira/spatial-utils
cd spatial-utils
mvn install
```

Joctomap is compatible for JDK1.6+. Here is how you can compile it:

```bash
git clone https://github.com/gonzalezsieira/joctomap
cd joctomap
mvn install
```

When compilation and installation finishes you can import the jar file into your
project or, if you use maven, add the following dependency:

```xml
<dependency>
    <groupId>es.usc.citius.lab</groupId>
    <artifactId>joctomap</artifactId>
    <version>1.9.0.0</version>
</dependency>
```
