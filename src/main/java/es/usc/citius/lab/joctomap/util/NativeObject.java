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

/**
 * Represents an object that supports native calls.
 * 
 * @author Adrián González Sieira <adrian.gonzalez@usc.es>
 */
public abstract class NativeObject {

        //place to load the dynamic libraries used in the code
        static{
            try {
                NativeUtils.loadLibraryFromJar("/joctomap-natives/libjoctomap.so");
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        }
    
	//stores the pointer used by native calls
	protected long pointer;
	
	
	/**
	 * Initializes a native object with the pointer
	 * to the native object.
	 * 
	 * @param pointer
	 */
	public NativeObject(long pointer){
		this.pointer = pointer;
	}
	
	/**
	 * Retrieves the assigned native pointer
	 * 
	 * @return pointer to the native OcTree
	 */
	public long getPointer() {
		return pointer;
	}
        
        /**
         * CAUTION: ONLY TO BE CALLED BY THE FINALIZE METHOD OF EACH CLASS IMPLEMENTING
         * NATIVEOBJECT; THIS WILL FREE THE MEMORY USED BY THE POINTER STORED IN THIS
         * CLASS, SO THIS OBJECT IS NOT BEING USABLE ANYMORE.
         */
        public abstract void dispose();
        
}
