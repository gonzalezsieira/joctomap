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

    static float size_box_x = 0.35f;
    static float size_box_y = 0.48f;
    static float resolution = 0.03f;
    static Point2D min_position_flyarea = new Point2D(-0.4f, -0.4f);
    static Point2D max_position_flyarea = new Point2D(5.4f, 4.4f);
    static Point2D min_position_map = new Point2D(-1, -1);
    static Point2D max_position_map = new Point2D(6, 5);
    static Point2D[] boxesCenter = new Point2D[]{
            new Point2D(0.49f, 0.44f),
            new Point2D( 0.51f, 2.19f),
            new Point2D(2.55f, 1.14f),
            new Point2D(2.55f, 0.66f),
            new Point2D(2.57f, 2.82f),
            new Point2D(2.57f, 3.30f),
            new Point2D(4.40f, 3.54f),
            new Point2D(4.36f, 1.60f)
    };

    @Override
    public Options getModuleOptions() {
        return getGenericOptions();
    }


    @Override
    public void execute(CommandLine args) {
        JOctree octree = JOctree.create(resolution);
        for(float x = min_position_map.getX(); x <= max_position_map.getX(); x += resolution / 4f){
            for(float y = min_position_map.getY(); y <= max_position_map.getY(); y += resolution / 4f){
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
