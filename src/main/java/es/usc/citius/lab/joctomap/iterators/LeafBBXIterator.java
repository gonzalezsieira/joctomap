package es.usc.citius.lab.joctomap.iterators;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.octree.JOctreeKey;
import es.usc.citius.lab.joctomap.octree.JOctreeNode;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;

/**
 * Implements an iterator for the {@link JOctreeNode} of a
 * given {@link JOctree}, which  
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class LeafBBXIterator extends OctreeIterator{
	
	/**
	 * Default constructor for this class, that takes the native pointer to the iterator and
	 * the native pointer to the end point of the created iterator.
	 * 
	 * @param pointer
	 */
	private LeafBBXIterator(long pointer) {
		super(pointer);
	}

	@Override
	public native boolean hasNext();

	@Override
	public native JOctreeKey next();

	@Override
	public native JOctreeKey key();

	@Override
	public native int depth();

	@Override
	public native Point3D coordinate();

	@Override
	public native float x();

	@Override
	public native float y();

	@Override
	public native float z();

	@Override
	public native float size();

	@Override
	public native JOctreeNode node();

        @Override
        public native void dispose();
        
        @Override
        protected void finalize() throws Throwable {
            super.finalize(); //To change body of generated methods, choose Tools | Templates.
            dispose();
        }
}
