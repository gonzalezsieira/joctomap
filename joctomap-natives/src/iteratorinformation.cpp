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
/*
 * iteratorinformation.cpp
 *
 *  Created on: 19/05/2014
 *      Author: adrian.gonzalez
 */

#include "iteratorinformation.h"

/**
 * Default constructor.
 */
LeafBBXIteratorInformation::LeafBBXIteratorInformation(OcTree::leaf_bbx_iterator it, OcTree::leaf_bbx_iterator end) {
	this->it = it;
	this->end = end;
}

/**
 * Retrieves if the iterator has reached its end.
 */
bool LeafBBXIteratorInformation::hasNext(){
	return it != end;
}

/**
 * Retrieves the next step of the iterator.
 */
OcTree::leaf_bbx_iterator LeafBBXIteratorInformation::next(){
	OcTree::leaf_bbx_iterator it_before = this->it;
	this->it = it.operator ++();
	return it_before;
}

/**
 * Retrieves the current step of the iterator.
 */
OcTree::leaf_bbx_iterator LeafBBXIteratorInformation::current(){
	return this->it;
}

/**
 * Default destructor.
 */
LeafBBXIteratorInformation::~LeafBBXIteratorInformation() {
	// TODO Auto-generated destructor stub
}

/**
 * Default constructor.
 */
LeafIteratorInformation::LeafIteratorInformation(OcTree::leaf_iterator it, OcTree::leaf_iterator end) {
	this->it = it;
	this->end = end;
}

/**
 * Retrieves if the iterator has reached its end.
 */
bool LeafIteratorInformation::hasNext(){
	return it != end;
}

/**
 * Retrieves the next step of the iterator.
 */
OcTree::leaf_iterator LeafIteratorInformation::next(){
	OcTree::leaf_iterator it_before = this->it;
	this->it = it.operator ++();
	return it_before;
}

/**
 * Retrieves the current step of the iterator.
 */
OcTree::leaf_iterator LeafIteratorInformation::current(){
	return this->it;
}

/**
 * Default destructor.
 */
LeafIteratorInformation::~LeafIteratorInformation() {
	// TODO Auto-generated destructor stub
}
