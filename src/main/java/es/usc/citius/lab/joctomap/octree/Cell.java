package es.usc.citius.lab.joctomap.octree;

import es.usc.citius.lab.motionplanner.core.spatial.Point3D;

/**
 * Class representing the geometric information of cell in Java (position and size).
 *
 * @author adrian.gonzalez
 */
public class Cell {

    public Point3D center;
    public float size;

    public Cell(Point3D center, float size) {
        this.center = center;
        this.size = size;
    }

    public Point3D getCenter() {
        return center;
    }

    public float getSize() {
        return size;
    }

}
