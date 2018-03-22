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
package es.usc.citius.lab.joctomap.ros;

import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.util.JOctomapLogger;
import es.usc.citius.lab.joctomap.util.NativeUtils;
import octomap_msgs.Octomap;

public class Messages {

    //place to load the dynamic libraries used in the code
    static{
        try {
            NativeUtils.loadLibraryFromJar("/libjoctomap_natives.so");
        } catch (Throwable ex) {
            JOctomapLogger.severe("Uncaught exception - " + ex.getMessage());
            ex.printStackTrace(System.err);
        }
    }

    /**
     * Converts a message received in ROS format (octomap_msgs/Octomap) to
     * an {@link JOctree}.
     *
     * @param msg ROS message
     * @return instance of octree
     */
    public static JOctree convert(Octomap msg){
        //get octree data
        double resolution = msg.getResolution();
        String id = msg.getId();
        boolean binary = msg.getBinary();
        byte[] data = new byte[msg.getData().readableBytes()];
        msg.getData().readBytes(data);
        return convert(id, resolution, binary, data);
    }

    private static native JOctree convert(String id, double resolution, boolean binary, byte[] data);
}
