# Using Joctomap
#### Compile from sources
This project depends on [spatial-utils](https://github.com/gonzalezsieira/spatial-utils), a lightweight library for geometrical
operations in cartesian coordinate systems. This library has to be compiled first:
```bash
git clone https://github.com/gonzalezsieira/spatial-utils
cd spatial-utils
mvn install
```

Joctomap JDK1.6+ compatible. It is compiled as follows:

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
