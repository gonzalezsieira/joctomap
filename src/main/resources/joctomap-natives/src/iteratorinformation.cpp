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
