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
