# Joctomap - A Java wrapper for Octomap
[Octomap](http://octomap.github.io) is a C++ framework to generate, update and manage 3D occupancy maps. These maps are
multi-resolution and widely used in mapping and robotic applications, among others.

**Joctomap** is a **wrapper** for this mapping library which allows you to use it in **Java/Android** projects.
It provides access to the main functionalities of Octomap, while retaining
the structure and philosophy of the mapping library.

# Functionalities
It **allows using the main functions of Octomap in Java**, but it also has **several functionalities** which might be interesting
for planning and navigation problems, like:
 - **Check collisions** between circles and oriented rectangles and the obstacles in the map
 - **Query the resolution**, given a position in the map
 - Query **which cells are occupied** (and retrieve their positions or keys) within a bounding box
 - Build an **adjacency map** to easily retrieve the adjacent cells to a given one
 - **Build an octree given an image in PPM (ASCII) format**

## Integration with ROS
  <img align="right" height="40" src="http://www.ros.org/wp-content/uploads/2013/10/rosorg-logo1.png" style="margin: 5px;">
  
  Since March 26th, 2018, it is possible to serialize and deserialize [octomap_msgs/Octomap](http://docs.ros.org/jade/api/octomap_msgs/html/msg/Octomap.html) messages used in [ROS](www.ros.org). Thus, it is possible to integrate Octomap in ROS applications written in Java making use of [Rosjava](https://github.com/rosjava).

  ![Image of ROS message](http://persoal.citius.usc.es/adrian.gonzalez/static/octomap_msg.png)


## Build status
 - Branch 1.7.X [![Build Status](https://travis-ci.org/gonzalezsieira/joctomap.svg?branch=1.7.X)](https://travis-ci.org/gonzalezsieira/joctomap)
 - Branch 1.8.X [![Build Status](https://travis-ci.org/gonzalezsieira/joctomap.svg?branch=1.8.X)](https://travis-ci.org/gonzalezsieira/joctomap)
 - Branch 1.9.X [![Build Status](https://travis-ci.org/gonzalezsieira/joctomap.svg?branch=1.9.X)](https://travis-ci.org/gonzalezsieira/joctomap)

## How do I use it?
#### Compiling Joctomap
This project depends on spatial-utils, a lightweight library for geometrical
operations in cartesian coordinate systems. This library has to be compiled first:
```bash
git clone https://github.com/gonzalezsieira/spatial-utils
cd spatial-utils
mvn install
```

Joctomap is compatible for JDK1.6+. Compile it as follows:

```bash
git clone https://github.com/gonzalezsieira/joctomap
cd joctomap
mvn install -DreleaseType=Release -Djdk=$JDK_HOME
```

 - `-DreleaseType` can be *Release* or *Debug* according to your needs.
 - `-Djdk` must point to the directory where your JDK is installed. (e.g. */usr/lib/jvm/java-8-oracle*)

#### Maven/Gradle
Afterwards you can import the generated jar file into your
project or, if you use **maven**, add the following dependency:

```xml
<dependency>
    <groupId>es.usc.citius.lab</groupId>
    <artifactId>joctomap</artifactId>
</dependency>
```

For **gradle**, it would be:
```
compile 'es.usc.citius.lab:joctomap'
```
