/**
 * Copyright (C) 2014-2017 Adrián González Sieira (adrian.gonzalez@usc.es)
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
 * File:   collisions.h
 * Author: adrian.gonzalez
 *
 * Created on 29 de junio de 2016, 16:45
 */
#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <octomap/math/Vector3.h>
#include <octomap/OcTree.h>

using namespace octomap;

point3d rotate(point3d point, double yaw, double pitch, double roll);

point3d intersection_line_and_plane(point3d a, point3d b, octomath::Vector3 normal, double distance);

point3d relative_position_border_cell(double resolution, double yaw, double pitch);

bool checkCollision(point3d point, float radius, OcTree* octree);

#endif /* COLLISIONS_H */

