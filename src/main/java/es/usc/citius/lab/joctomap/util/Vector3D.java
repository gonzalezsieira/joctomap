package es.usc.citius.lab.joctomap.util;

/**
 * Defines a 3D vector. A vector can be instantiated from two points,
 * from another vector, or manually given its three components.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class Vector3D {
	private double x;
	private double y;
	private double z;
	
	/**
	 * Creates a vector [x, y, z] given its three elements.
	 * 
	 * @param x first element
	 * @param y second element
	 * @param z third element
	 */
	public Vector3D(double x, double y, double z){
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	/**
	 * Creates a copy of a given vector.
	 * 
	 * @param vector original
	 */
	public Vector3D(Vector3D vector){
		this.x = vector.x;
		this.y = vector.y;
		this.z = vector.z;
	}
	
	/**
	 * Computes the distance vector (p2 - p1).
	 * 
	 * @param p1 origin
	 * @param p2 destination
	 */
	public Vector3D(Point3D p1, Point3D p2){
		this.x = p2.getX() - p1.getX();
		this.y = p2.getY() - p1.getY();
		this.z = p2.getZ() - p1.getZ();
	}

	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}

	public double getZ() {
		return z;
	}
	
}
