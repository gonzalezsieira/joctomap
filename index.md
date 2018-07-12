# Joctomap: A Java/Android wrapper for Octomap
**Joctomap** is a **Java/Android wrapper** for [Octomap](http://octomap.github.io), a mapping library which allows you to generate, update and manage 3D occupancy maps. These maps use multi-resolution strategies to efficiently represent the space, and they are widely used in mapping and robotic applications.

# Functionalities
It **allows using the main functions of Octomap in Java**, but it also has **several functionalities** which might be interesting for planning and navigation problems, like:
 - **Collision check** between circles and oriented rectangles and the obstacles in the map
 - **Query the resolution**, given a position in the map
 - Query **which cells are occupied** (and retrieve their positions or keys) within a bounding box
 - Build an **adjacency map** to easily retrieve the adjacent cells to a given one
 - **Build an octree from PPM (ASCII) image**

## Integration with ROS & Rosjava
  <img align="right" height="40" src="http://www.ros.org/wp-content/uploads/2013/10/rosorg-logo1.png" style="margin: 5px;">
  
  Since March 26th, 2018, it is possible to serialize and deserialize [octomap_msgs/Octomap](http://docs.ros.org/jade/api/octomap_msgs/html/msg/Octomap.html) messages used in [ROS](www.ros.org). Thus, it is possible to integrate Octomap in ROS applications written in Java
   and communicate with other nodes using [Rosjava](https://github.com/rosjava).

  ![Image of ROS message](http://persoal.citius.usc.es/adrian.gonzalez/static/octomap_msg.png)
