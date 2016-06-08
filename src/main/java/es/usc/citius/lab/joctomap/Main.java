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
package es.usc.citius.lab.joctomap;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import es.usc.citius.lab.joctomap.util.JOctomapLogger;
import es.usc.citius.lab.joctomap.module.Module;
import org.apache.commons.cli.DefaultParser;

/**
 * This class executes the utilities stored in this library, to make them
 * accesible by command-line arguments.
 *
 * Current executions implemented in this class are:
 * <ul>
 * <li>-ppmot -i input -o output</li>
 * </ul>
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class Main {

    @SuppressWarnings("static-access")
    public static void main(String[] args) {
        //get generic options
        Options op = Module.getGenericOptions();

        //default parser creation
        CommandLineParser parser = new DefaultParser();
        CommandLine commands = null;
        //try to parse the input arguments (first level: determine module)
        try {
            commands = parser.parse(op, args, true);
        } catch (ParseException ex) {
            printHelp(null, op);
            //exit with code error 1
            System.exit(1);
        }

        Module module = null;
        //retrieve module name
        String moduleName = commands.getOptionValue("t");
        try{
            //execute option depending on the execution mode
            module = Module.getModule(moduleName).newInstance();
        } catch (InstantiationException ex) {
            JOctomapLogger.severe("Error initializing module " + moduleName + ": " + ex);
            throw new RuntimeException(ex);
        } catch (IllegalAccessException ex) {
            JOctomapLogger.severe("Illegal access to module " + moduleName + ": " + ex);
            throw new RuntimeException(ex);
        }
        //now obtain the options of the module
        JOctomapLogger.info("Using module " + module.getClass().getSimpleName());
        try{
            commands = parser.parse(module.getModuleOptions(), args);
        } catch (ParseException ex){
            printHelp(module.getClass().getSimpleName(), module.getModuleOptions());
            System.exit(1);
        }
        //execute actions of the module
        module.execute(commands);
    }
    
    /**
     * Prints the help of the program according to the actions of the module.
     * @param options 
     */
    private static void printHelp(String moduleName, Options options){
        //create formatter to print help
        HelpFormatter formatter = new HelpFormatter();
        String toolName = "joctomap: Java wrapper for Octomap\nRegistered modules accesible via -t are:";
        for(String current : Module.MODULES.keySet()){
            toolName = toolName.concat("\t* " + current);
        }
        if(moduleName != null){
            toolName = toolName.concat("\n(Selected module was = " + moduleName + ")");
        }
        //print help
        formatter.printHelp(
                160,
                toolName,
                "This is a development version for research purposes",
                options,
                "Developed at CITIUS (http://citius.usc.es)"
        );
    }
    
}
