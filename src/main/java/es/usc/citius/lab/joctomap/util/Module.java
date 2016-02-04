package es.usc.citius.lab.joctomap.util;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.cli.CommandLine;

/**
 * Definition of an executable module for the application. The
 * program will find all the subclasses of Module and re
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public abstract class Module {
	
	private static final Map<String, Class<?>> modules = registerModules();

	/**
	 * Name of the module that will be called by the "-t" argument of the
	 * application
	 * 
	 * @return module name
	 */
	public abstract String getName();
	
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
        modulesMap.put("ppmot", JOCtreeBuilder.class);
        modulesMap.put("gworld", GazeboWorldGenerator.class);
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
        if(!modules.containsKey(s)){
            throw new RuntimeException("Module not found: " + s);
        }
        return (Class<Module>) modules.get(s);
    }
    
}
