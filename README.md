# Joctomap - A Java wrapper for Octomap

[Octomap](http://octomap.github.io) is a C++ framework to generate, update and manage 3D occupancy maps. These maps are 
multi-resolution and widely used in mapping and robotic applications, among others.

Joctomap is a Java wrapper for the Octomap mapping framework. Provides access to the main functionalities
of the library and makes possible its usage in Java/Android projects. It maintains
the structure and philosophy of the parent project, also adding several functions which might be interesting
for planning and navigation problems.


## How do I use it?
This project is compatible for JDK1.6+. Here is how you can compile it:

```bash
git clone https://gitlab.citius.usc.es/adrian.gonzalez/joctomap.git
mvn install
```

When compilation and installation finishes you can import the jar file into your
project or, if you use maven, add the following dependency:

```xml
<dependency>
    <groupId>es.usc.citius.lab</groupId>
    <artifactId>joctomap</artifactId>
    <version>1.7.2-SNAPSHOT</version>
</dependency>
```
