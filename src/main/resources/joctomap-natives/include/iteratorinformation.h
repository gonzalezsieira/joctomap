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

class IteratorInformation {
	private:
		OcTree::leaf_bbx_iterator it;
		OcTree::leaf_bbx_iterator end;
	public:
		IteratorInformation(OcTree::leaf_bbx_iterator it, OcTree::leaf_bbx_iterator end);
		virtual ~IteratorInformation();
		bool hasNext();
		OcTree::leaf_bbx_iterator next();
};

#endif /* ITERATORINFORMATION_H_ */
