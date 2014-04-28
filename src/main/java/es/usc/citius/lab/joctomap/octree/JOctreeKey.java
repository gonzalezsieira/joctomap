package es.usc.citius.lab.joctomap.octree;


/**
 * Represents a key of the {@link JOctree}, which is the identifier
 * used to store the nodes in the data structure. This is not a native
 * object wrapper since it has not associated functions.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class JOctreeKey{

	private int x;
	private int y;
	private int z;
	
	/**
	 * Default constructor that initializes the 3D coordinates
	 * of the key.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 */
	public JOctreeKey(int x, int y, int z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + x;
		result = prime * result + y;
		result = prime * result + z;
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
		JOctreeKey other = (JOctreeKey) obj;
		if (x != other.x)
			return false;
		if (y != other.y)
			return false;
		if (z != other.z)
			return false;
		return true;
	}
	
	@Override
	public String toString() {
		return "{" + x + ", " + y + ", " + z + "}";
	}
	
	public int getX() {
		return x;
	}
	
	public int getY() {
		return y;
	}
	
	public int getZ() {
		return z;
	}
	
}
