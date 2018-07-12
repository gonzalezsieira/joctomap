/**
 * Copyright (C) 2014-2017 Adrián González Sieira (adrian.gonzalez@usc.es)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package es.usc.citius.lab.joctomap.util;

import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import es.usc.citius.lab.motionplanner.core.spatial.Vector3D;

/**
 * Class defining an obstacle, formed by a 3D point and the distance.
 *
 * @author Adrián González Sieira {@literal <adrian.gonzalez@usc.es>}
 */
public class Obstacle {
	private Point3D obstacle;
	private Vector3D distance;
	
	/**
	 * Unique constructor that initializes the element with the obstacle location and 
	 * current distance.
	 * 
	 * @param point origin of coordinates to measure the distances (X, Y, Z), i.e. location of of the robot in the map
	 * @param obstacle coordinate (X, Y, Z) of the obstacle
	 */
	public Obstacle(Point3D point, Point3D obstacle){
		this.obstacle = obstacle;
		this.distance = new Vector3D(point, obstacle);
	}

	/**
	 * @return coordinate (X, Y, Z) of the obstacle
	 */
	public Point3D getObstacle() {
		return obstacle;
	}

	/**
	 * @return distance vector (X, Y, Z) between the origin and the obstacle
	 */
	public Vector3D getDistance() {
		return distance;
	}
	
}
