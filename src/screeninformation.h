/*
* screeninformation.h
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/
#pragma once

#include <osg/GraphicsContext>

class ScreenInformation : public osg::Referenced {
public:
	osg::Vec2i size() const;
	double aspect_ratio() const;
private:
	mutable bool _init = { false };
	mutable osg::Vec2i _size = { osg::Vec2i(0, 0) };
};