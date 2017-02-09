package es.usc.citius.lab.joctomap.module;

import es.usc.citius.lab.joctomap.module.Module;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.motionplanner.core.spatial.Point2D;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;

/**
 * Created by adrian on 9/02/17.
 */
public class CreateOctomapLab extends Module {

    static float size_box_x = 0.5f;
    static float size_box_y = 1.0f;
    static float resolution = 0.1f;
    static Point2D min_position_flyarea = new Point2D(0, 0);
    static Point2D max_position_flyarea = new Point2D(5, 4);
    static Point2D min_position_map = new Point2D(-1, -1);
    static Point2D max_position_map = new Point2D(6, 5);
    static Point2D[] boxesCenter = new Point2D[]{
            new Point2D(2, 2),
            new Point2D( 3, 2)
    };

    @Override
    public Options getModuleOptions() {
        return getGenericOptions();
    }


    @Override
    public void execute(CommandLine args) {
        JOctree octree = JOctree.create(resolution);
        for(float x = min_position_map.getX(); x <= max_position_map.getX(); x += resolution / 2f){
            for(float y = min_position_map.getY(); y <= max_position_map.getY(); y += resolution / 2f){
                boolean occupied = false;
                // Check fly area
                if (x < min_position_flyarea.getX() || x > max_position_flyarea.getX()
                        || y < min_position_flyarea.getY() || y > max_position_flyarea.getY() || isPointInsideBox(x, y)){
                    occupied = true;
                }
                // Update map
                octree.updateNode(x, y, 0f, occupied);
            }
        }
        octree.write("octree_lab_test.ot");
    }

    public boolean isPointInsideBox(float x, float y){
        boolean isInsideBox = false;
        for (Point2D box : boxesCenter){
            if ( x >= box.getX() - size_box_x / 2f && x <= box.getX() + size_box_x / 2f
                    && y >= box.getY() - size_box_y / 2f && y <= box.getY() + size_box_y / 2f){
                isInsideBox = true;
                break;
            }
        }
        return isInsideBox;
    }

}
