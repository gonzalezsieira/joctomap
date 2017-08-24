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
package es.usc.citius.lab.joctomap.hipster;

import es.usc.citius.hipster.model.Transition;
import es.usc.citius.hipster.model.function.TransitionFunction;
import es.usc.citius.lab.joctomap.util.NativeObject;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;

public class H3DMRTransitionFunction extends NativeObject implements TransitionFunction<Void, Point3D> {

    /**
     * This constructor will be hidden.
     *
     * @param pointer
     */
    private H3DMRTransitionFunction(long pointer) {
        super(pointer);
    }


    public H3DMRTransitionFunction(){
        super(0);
        this.pointer = initialize();
    }

    public native long initialize();

    @Override
    public native Iterable<Transition<Void, Point3D>> transitionsFrom(Point3D point3D);

    @Override
    public native void dispose();

    @Override
    protected void finalize() throws Throwable {
        super.finalize(); //To change body of generated methods, choose Tools | Templates.
        dispose();
    }
}
