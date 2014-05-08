package es.usc.citius.lab.joctomap.util;

/**
 * Class defining a point in the 3D-space.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class Point3D {

	private double x;
	private double y;
	private double z;
	
	/**
	 * Constructor that initializes a 3D point from its coordinates.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 */
	public Point3D(double x, double y, double z){
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	/**
	 * Constructor that clones a {@link Point3D} existing object.
	 * 
	 * @param point instance to copy
	 */
	public Point3D(Point3D point){
		this.x = point.x;
		this.y = point.y;
		this.z = point.z;
	}
	
	@Override
	public String toString() {
		return "[x=" + x + ", y=" + y + ", z=" + z + "]";
	}
	
	/************************************************************************
	 * 								GETTERS
	 ************************************************************************/
	
	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}

	public double getZ() {
		return z;
	}

	/************************************************************************
	 * 					EQUALS/HASH-CODE METHODS
	 ************************************************************************/
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		long temp;
		temp = Double.doubleToLongBits(x);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		temp = Double.doubleToLongBits(y);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		temp = Double.doubleToLongBits(z);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Point3D other = (Point3D) obj;
		if (Double.doubleToLongBits(x) != Double.doubleToLongBits(other.x))
			return false;
		if (Double.doubleToLongBits(y) != Double.doubleToLongBits(other.y))
			return false;
		if (Double.doubleToLongBits(z) != Double.doubleToLongBits(other.z))
			return false;
		return true;
	}
	
}
