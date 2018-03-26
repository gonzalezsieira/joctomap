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
import com.google.common.base.Stopwatch;
import es.usc.citius.lab.joctomap.octree.JOctree;
import es.usc.citius.lab.joctomap.ros.Messages;
import octomap_msgs.Octomap;
import org.junit.Test;

import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.*;

public class MessagesTest {

    private JOctree octree;

    public MessagesTest() throws URISyntaxException, IOException {
        File fileRead = new File(getClass().getClassLoader().getResource("fr_campus.ot").toURI());
        //read octree test
        octree = JOctree.read(fileRead.getAbsolutePath());
    }

    @Test
    public void testBinary(){
        //convert octree -> message
        Stopwatch watch1 = new Stopwatch().start();
        Octomap message = Messages.convert(octree, true);
        watch1 = watch1.stop();
        System.out.printf("[%s ms] Serialized binary octree\n", watch1.elapsedTime(TimeUnit.MILLISECONDS));
        //convert message -> octree
        watch1 = watch1.reset(); watch1.start();
        JOctree deserializedOctree = Messages.convert(message);
        watch1.stop();
        System.out.printf("[%s ms] Deserialized binary octree\n", watch1.elapsedTime(TimeUnit.MILLISECONDS));
        //test serializing - deserializing
        testConditions(octree, deserializedOctree);
    }

    @Test
    public void testNonBinary(){
        //convert octree -> message
        Stopwatch watch1 = new Stopwatch().start();
        Octomap message = Messages.convert(octree, false);
        watch1 = watch1.stop();
        System.out.printf("[%s ms] Serialized complete octree\n", watch1.elapsedTime(TimeUnit.MILLISECONDS));
        //convert message -> octree
        watch1 = watch1.reset(); watch1.start();
        JOctree deserializedOctree = Messages.convert(message);
        watch1.stop();
        System.out.printf("[%s ms] Deserialized complete octree\n", watch1.elapsedTime(TimeUnit.MILLISECONDS));
        //test serializing - deserializing
        testConditions(octree, deserializedOctree);
    }

    private void testConditions(JOctree octree, JOctree deserializedOctree){
        assertEquals("Octree sizes do not match", octree.size(), deserializedOctree.size());
        assertEquals("Min positions do not match", octree.getMetricMin(), deserializedOctree.getMetricMin());
        assertEquals("Max positions do not match", octree.getMetricMax(), deserializedOctree.getMetricMax());
        assertEquals("Octree depths do not match", octree.getTreeDepth(), deserializedOctree.getTreeDepth());
        assertEquals("Octree resolutions do not match", octree.getResolution(), deserializedOctree.getResolution(), 1E-4);
        assertArrayEquals("Octree dimensions do not match", octree.getMetricSize(), deserializedOctree.getMetricSize(), 1E-4f);
    }

}
