**Joctomap** is a **Java/Android wrapper** for [Octomap](http://octomap.github.io), a mapping library which allows you to generate, update and manage 3D occupancy maps. These maps use multi-resolution strategies to efficiently represent the space, and they are widely used in mapping and robotic applications.

# Features
It **allows using the main features of Octomap in Java**, but it also has **several functionalities** which might be interesting for planning and navigation problems, like:
 - **Collision check** between circles and oriented rectangles and the obstacles in the map
 - **Query the resolution**, given a position in the map
 - Query **which cells are occupied** (and retrieve their positions or keys) within a bounding box
 - Build an **adjacency map** to easily retrieve the adjacent cells to a given one
 - **Build an octree from PPM (ASCII) image**

# Read more
 - [Compilation & Usage](usage)
 - [ROS/Rosjava integration](ros)
