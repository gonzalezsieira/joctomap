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
