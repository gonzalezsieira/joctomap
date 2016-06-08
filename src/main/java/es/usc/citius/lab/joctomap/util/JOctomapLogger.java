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
package es.usc.citius.lab.joctomap.util;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.ConsoleHandler;
import java.util.logging.FileHandler;
import java.util.logging.Formatter;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

/**
 * Custom class to extend the capabilities of {@link Logger} for the JOctomap library. 
 * This class enables an interface to call logger features without using the {@link Logger#getLogger(String)}
 * method in all calls.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public enum JOctomapLogger {
	INSTANCE;
	
	private Logger logger;
	
	/**
	 * Default constructor of the class. Initializes the Java logger and
	 * the handler options to print in a readable format.
	 */
	private JOctomapLogger(){
		//retrieve logger
		this.logger = Logger.getLogger("JOctomap");
		//add handler
		this.setHandler(new ConsoleHandler());
	}
	
	/**
	 * Removes all existing handlers for the logger, and attaches a new one 
	 * with a custom format, for all levels of the logger.
	 * 
	 * @param handler new instance of handler to set as default of this logger
	 */
	private void setHandler(Handler handler){
		//define scope of the handler
		handler.setLevel(Level.ALL);
		//disable parent handlers
		logger.setUseParentHandlers(false);
		//define format of the handler
        handler.setFormatter(new Formatter() {
            
            private final SimpleDateFormat format = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
            
            @Override
            public String format(LogRecord lr) {
                StringBuilder buf = new StringBuilder(1000);
                buf.append("[" + format.format(new Date(lr.getMillis())) + "] ");
                buf.append(lr.getLevel() + ": ");
                buf.append(formatMessage(lr));
                buf.append('\n');
                return buf.toString();
            }
        });
        //remove all existing handlers
        for(Handler current : logger.getHandlers()){
        	logger.removeHandler(current);
        }
        //add newly created handler
		logger.addHandler(handler);
	}
	
	/**
	 * Sets the output stream of the logger in a file.
	 * @param path
	 */
	public static void output(String path){
		try{
			INSTANCE.setHandler(new FileHandler(path));
		} catch (IOException ex){
			throw new RuntimeException("Cannot initialize logger output file : " + ex);
		}
	}
	
	/*
	 * *******************************************************************************
	 * *							Log functions                                    *
	 * *******************************************************************************
	 */
	/**
	 * Prints a message at "info" {@link Level} of this logger.
	 * 
	 * @param msg
	 */
	public static void info(String msg){
		INSTANCE.logger.info(msg);
	}
	
	/**
	 * Prints a message at "severe" {@link Level} of this logger.
	 * 
	 * @param msg
	 */
	public static void severe(String msg){
		INSTANCE.logger.severe(msg);
	}
	
	/**
	 * Prints a message at "warning" {@link Level} of this logger.
	 * 
	 * @param msg
	 */
	public static void warning(String msg){
		INSTANCE.logger.warning(msg);
	}
	
	/**
	 * Prints a message at "fine" {@link Level} of this logger.
	 * 
	 * @param msg
	 */
	public static void fine(String msg){
		INSTANCE.logger.fine(msg);
	}
	
	/**
	 * Prints a message at "finer" {@link Level} of this logger.
	 * 
	 * @param msg
	 */
	public static void finer(String msg){
		INSTANCE.logger.finer(msg);
	}
	
	/**
	 * Prints a message at "finest" {@link Level} of this logger.
	 * 
	 * @param msg
	 */
	public static void finest(String msg){
		INSTANCE.logger.finest(msg);
	}
}
