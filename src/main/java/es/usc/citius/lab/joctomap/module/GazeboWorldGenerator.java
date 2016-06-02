package es.usc.citius.lab.joctomap.module;

import es.usc.citius.lab.joctomap.module.Module;
import es.usc.citius.lab.joctomap.iterators.OctreeIterator;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.motionplanner.core.spatial.Point3D;
import org.apache.commons.cli.CommandLine;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by adrian.gonzalez on 22/10/15.
 */
public class GazeboWorldGenerator extends Module {

    @Override
    public String getName() {
        return "gworld";
    }

    @Override
    public void execute(CommandLine args) {
        //collect input arg from command line
        String inputArg = args.getOptionValue("i");
        String outputArg = args.getOptionValue("o");
        //read input octree from file
        JOctree octree = JOctree.read(inputArg);
        //write header
        String xmlWorld =
                "<?xml version='1.0'?>\n" +
                "<sdf version='1.4'>\n" +
                "<world name ='octomap_world'>\n" +
                "<!-- A global light source -->\n" +
                "<include>\n" +
                "<uri>model://sun</uri>\n" +
                "</include>\n" +
                "<!-- A ground plane -->\n" +
                "<include>\n" +
                "<uri>model://ground_plane</uri>\n" +
                "</include>\n" +
                "<model name ='octomap'>\n" +
                "<pose>0 0 0 0 0 0</pose>\n" +
                "<static>true</static>\n";
        //iterate over the occupied cells
        int n = 0;
        for (OctreeIterator it = octree.leafBBXIterator(octree.getMetricMin(), octree.getMetricMax(), 0); it.hasNext(); it.next()){
            if(!octree.isNodeOccupied(it.node())) continue;
            float resolution = it.size();
            Point3D coordinate = it.coordinate();
            n++;
            xmlWorld +=
                    "<link name = 'box" + n + "'>\n" +
                    "<pose>\n" +
                            coordinate.getX() + " " + coordinate.getY() + " " + coordinate.getZ() + " 0 0 0\n" +
                    "</pose>\n" +
                    "<collision name = 'collision'>\n" +
                    "<geometry><box><size>" +
                    resolution + " " + resolution + " " + resolution +
                    "</size></box></geometry>\n" +
                    "</collision>\n" +
                    "<visual name = 'visual'>\n" +
                    "<geometry><box><size>" +
                    resolution + " " + resolution + " " + resolution +
                    "</size></box></geometry>\n" +
                    "</visual>\n" +
                    "</link>\n";
        }
        xmlWorld +=
                "</model>\n" +
                "</world>\n" +
                "</sdf>";

        //save XML in output file
        try {
            File file = new File(outputArg);
            if (!file.exists()) file.createNewFile();
            FileOutputStream stream = new FileOutputStream(file);
            stream.write(xmlWorld.getBytes());
            stream.close();
        } catch (IOException ex) {
            System.err.println(ex);
        }
    }

}