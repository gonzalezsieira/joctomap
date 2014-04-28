package es.usc.citius.lab.joctomap.util;

/**
 * Represents an object that supports native calls.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public abstract class NativeObject {

	//stores the pointer used by native calls
	protected long pointer;
	
	
	/**
	 * Initializes a native object with the pointer
	 * to the native object.
	 * 
	 * @param pointer
	 */
	public NativeObject(long pointer){
		this.pointer = pointer;
	}
	
	/**
	 * Retrieves the assigned native pointer
	 * 
	 * @return pointer to the native OcTree
	 */
	public long getPointer() {
		return pointer;
	}
	
}
