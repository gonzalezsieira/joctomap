/**
 * Copyright (C) 2014-2017 Adrián González Sieira (adrian.gonzalez@usc.es)
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
