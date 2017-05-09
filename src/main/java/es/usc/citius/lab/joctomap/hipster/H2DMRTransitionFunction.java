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
package es.usc.citius.lab.joctomap.hipster;

import es.usc.citius.hipster.model.Transition;
import es.usc.citius.hipster.model.function.TransitionFunction;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.util.AdjacencyMap;
import es.usc.citius.lab.joctomap.util.NativeObject;
import es.usc.citius.lab.motionplanner.core.shapes.Shape2D;
import es.usc.citius.lab.motionplanner.core.spatial.Point2D;

/**
 * This is the implementation of H2DMR transition function implemented in
 * JNI for faster access to elements in the octree.
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class H2DMRTransitionFunction extends NativeObject implements TransitionFunction<Void, Point2D>{

    /**
     * This constructor will be hidden.
     *
     * @param pointer
     */
    private H2DMRTransitionFunction(long pointer) {
        super(pointer);
    }

    public H2DMRTransitionFunction(JOctree octree, AdjacencyMap map, Shape2D optimisticShape, float minimumResolutionTrajectories){
        //initialize with an empty pointer
        super(0);

        //replace pointer to the initialized structure
        this.pointer = initialize(octree.getPointer(), map, optimisticShape.getMinRadius(), minimumResolutionTrajectories);
    }

    /**
     * This will find all reference methods and store them in a structure to
     * be accessed later.
     *
     * @return pointer to the native structure
     */
    private native long initialize(
            long octreePointer,
            AdjacencyMap map,
            float radiusOptimisticShape,
            float minimumResolutionTrajectories
    );

    @Override
    public native void dispose();

    @Override
    protected void finalize() throws Throwable {
        super.finalize(); //To change body of generated methods, choose Tools | Templates.
        dispose();
    }

    @Override
    public native Iterable<Transition<Void, Point2D>> transitionsFrom(Point2D s);

}
