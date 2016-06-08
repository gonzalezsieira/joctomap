/**
 * Copyright (C) 2014-2016 Adrián González Sieira (adrian.gonzalez@usc.es)
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
 * Class defining an obstacle, formed by a 3D point and the distance
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class Obstacle {
	private Point3D obstacle;
	private Vector3D distance;
	
	/**
	 * Unique constructor that initializes the element with the obstacle location and 
	 * current distance.
	 * 
	 * @param obstacle
	 * @param distance
	 */
	public Obstacle(Point3D pose, Point3D obstacle){
		this.obstacle = obstacle;
		this.distance = new Vector3D(pose, obstacle);
	}

	public Point3D getObstacle() {
		return obstacle;
	}

	public Vector3D getDistance() {
		return distance;
	}
	
}
