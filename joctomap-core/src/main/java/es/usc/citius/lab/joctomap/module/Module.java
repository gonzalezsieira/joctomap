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
package es.usc.citius.lab.joctomap.module;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.reflections.Reflections;

/**
 * Definition of an executable module for the application. The
 * program will find all the subclasses of Module and re
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public abstract class Module {
	
    public static final Map<String, Class<?>> MODULES = registerModules();

    /**
     * Retrieves the set of generic options used by all modules.
     *
     * @return module-independent {@link Options}
     */
    public static Options getGenericOptions() {
        Options op = new Options();
        op.addOption(
                Option.builder("t")
                        .desc("Tool to execute")
                        .hasArg()
                        .required()
                        .argName("tool_name")
                        .build()
        );
        return op;
    }
	
	/**
	 * Executes the functions of the module based on the {@link CommandLine} parsed
	 * arguments.
	 * 
	 * @param args
	 */
	public abstract void execute(CommandLine args);
	
	/**
	 * Register all available modules in the application.
	 * @return
	 */
    private static Map<String, Class<?>> registerModules(){
        Map<String, Class<?>> modulesMap = new HashMap<String, Class<?>>();
        Reflections subclasses = new Reflections(Module.class.getPackage().getName());
        for(Class current : subclasses.getSubTypesOf(Module.class)){
            modulesMap.put(current.getSimpleName(), current);
        }
        return modulesMap;
    }
    
    /**
     * Retrieves a module from a String query
     * @param s name of the module to be retrieved
     * @return module class
     * @throws RuntimeException if the module is not registered in the application
     */
    @SuppressWarnings("unchecked")
	public static Class<Module> getModule(String s) throws RuntimeException{
        if(!MODULES.containsKey(s)){
            throw new IllegalArgumentException("Module not found: " + s + ". Available modules are: " + MODULES.keySet());
        }
        return (Class<Module>) MODULES.get(s);
    }
    
    /**
     * This method constructs a list of required arguments in order to validate
     * inputs.
     * 
     * @return instance of {@link Options} to let CLI parse inputs
     */
    public abstract Options getModuleOptions();
    
}
