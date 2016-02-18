/*
* headposition.cpp
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/

#include "headpositionmanipulator.h"

HeadPositionManipulator::HeadPositionManipulator(double depth) : _depth(depth)
{
	_tracker = new HeadTracker;
	_tracker->open_camera(0);
	
}

void HeadPositionManipulator::updateCamera(osg::Camera & camera)
{
	double x = 0.0;
	double y = 0.0;
	double z = 1.0;
	double w = 2.0;
	if (_tracker.valid()) {
		osg::Vec3 position = _tracker->position();
		x = position.x();
		y = position.y();
		z = position.z() * _depth;
		if (z < 1.0) z = 1.0;
	}
	
	camera.setViewMatrixAsLookAt(osg::Vec3(x, y, z), osg::Vec3(x, y, 0.0), osg::Vec3(0.0, 1.0, 0.0));
	camera.setProjectionMatrixAsFrustum(-1.0 - x , 1.0 - x, -1.0 - y, 1.0 - y, z, z + w);
}
