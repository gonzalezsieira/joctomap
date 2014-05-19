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
IteratorInformation::IteratorInformation(OcTree::leaf_bbx_iterator it, OcTree::leaf_bbx_iterator end) {
	this->it = it;
	this->end = end;
}

/**
 * Retrieves if the iteator has reached its end.
 */
bool IteratorInformation::hasNext(){
	return it != end;
}

/**
 * Retrieves the next step of the iterator.
 */
OcTree::leaf_bbx_iterator IteratorInformation::next(){
	OcTree::leaf_bbx_iterator it_before = this->it;
	this->it = it.operator ++();
	return it_before;
}

/**
 * Default destructor.
 */
IteratorInformation::~IteratorInformation() {
	// TODO Auto-generated destructor stub
}

