/*
 * iteratorinformation.h
 *
 *  Created on: 19/05/2014
 *      Author: adrian.gonzalez
 */
#include <octomap/OcTree.h>

using namespace octomap;

#ifndef ITERATORINFORMATION_H_
#define ITERATORINFORMATION_H_

class LeafBBXIteratorInformation {
	private:
		OcTree::leaf_bbx_iterator it;
		OcTree::leaf_bbx_iterator end;
	public:
		LeafBBXIteratorInformation(OcTree::leaf_bbx_iterator it, OcTree::leaf_bbx_iterator end);
		virtual ~LeafBBXIteratorInformation();
		bool hasNext();
		OcTree::leaf_bbx_iterator next();
		OcTree::leaf_bbx_iterator current();
};

class LeafIteratorInformation {
	private:
		OcTree::leaf_iterator it;
		OcTree::leaf_iterator end;
	public:
		LeafIteratorInformation(OcTree::leaf_iterator it, OcTree::leaf_iterator end);
		virtual ~LeafIteratorInformation();
		bool hasNext();
		OcTree::leaf_iterator next();
		OcTree::leaf_iterator current();
};

#endif /* ITERATORINFORMATION_H_ */
