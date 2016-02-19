/*
* fish.h
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/
#pragma once

#include <osg/Node>

class Fish : public osg::Referenced {
public:
	explicit Fish(std::string filename, osg::Vec2 size) : _filename(filename), _size(size) {};
	osg::Node* node();
private:
	std::string _filename = { "" };
	osg::Vec2 _size = { osg::Vec2(0.0, 0.0) };
	osg::ref_ptr<osg::Node> _node = { nullptr };
};
