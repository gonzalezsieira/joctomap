package es.usc.citius.lab.joctomap;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import es.usc.citius.lab.joctomap.util.JOctomapLogger;
import es.usc.citius.lab.joctomap.util.Module;

/**
 * This class executes the utilities stored in this library, to make them accesible by
 * command-line arguments.
 * 
 * Current executions implemented in this class are:
 * <ul>
 * 		<li>-ppmot -i input -o output</li>
 * </ul>
 *
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public class Main {
	
	static{
		System.loadLibrary("joctomap");		
	}
	
	@SuppressWarnings("static-access")
	public static void main(String[] args) {
		Options op = new Options();
		op.addOption(
				OptionBuilder
				.withDescription("Input .ppm file, size X (m) and size Y (m)")
				.hasArgs(3)
				.withLongOpt("input")
				.withArgName("ppmFile> <sizeX> <sizeY")
				.create("i")
				);
		op.addOption(
				OptionBuilder
				.withDescription("Output .ot file")
				.hasArg()
				.withLongOpt("output")
				.withArgName("ot")
				.create("o")
				);
		op.addOption(
				OptionBuilder
				.withDescription("Tool to execute")
				.hasArg()
				.isRequired()
				.withArgName("tool_name")
				.create("t")
				);
		
		
		//default parser creation
		CommandLineParser parser = new GnuParser();
		CommandLine commands = null;
		//try to parse the input arguments
		try{
			commands = parser.parse(op, args);
		} catch (ParseException ex) {
			//create formatter to print help
            HelpFormatter formatter = new HelpFormatter();
            //print help
            formatter.printHelp(
            		160, 
            		"joctomap: Java wrapper for Octomap", 
            		"This is a development version for research purposes", 
            		op, 
            		"Developed at CITIUS (http://citius.usc.es)"
        		);
            //exit with code error 1
            System.exit(1);
		}
		
		//execute option depending on the execution mode
		try{
			Module module = Module.getModule(commands.getOptionValue("t")).newInstance();
			module.execute(commands);
		}catch (Exception ex){
			JOctomapLogger.severe("Error initializing module of the application -> " + commands.getOptionValue("t") + ": " + ex);
			System.exit(1);
		}
	}

}
