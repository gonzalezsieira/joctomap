package es.usc.citius.lab.joctomap;

/**
 * Hello world!
 *
 */
public class Main {
	
	static{
		System.loadLibrary("joctomap");
	}
	
    public static void main( String[] args ){
    	new JOctomap().test();
    }
}
