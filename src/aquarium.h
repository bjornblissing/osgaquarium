/*
* aquarium.h
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/
#pragma once

#include <osg/Group>

#include "fish.h"

class Aquarium : public osg::Referenced{
public:
	explicit Aquarium(double aspect) : _aspect(aspect){};
	osg::Group* group();
	double depth() const { return size_from_aspect(_aspect).z(); }
private:
	osg::Node* node();
	osg::Vec3 size_from_aspect(double aspect) const;
	osg::PositionAttitudeTransform* add_fish_at_position(osg::ref_ptr<Fish> fish, const osg::Vec3f& position);

	double _aspect = { 1.0 };
	osg::ref_ptr<osg::Node> _node = { nullptr };
	osg::ref_ptr<osg::Group> _group = { nullptr };
	osg::ref_ptr<osg::Group> _fish_group = { nullptr };
};
