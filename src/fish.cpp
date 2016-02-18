/*
* fish.cpp
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/

#include "fish.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

osg::Node* Fish::node() {
	if (!_node.valid()) {
		// Create geometry
		osg::ref_ptr<osg::Geode> quad_geode = new osg::Geode;
		osg::ref_ptr<osg::Geometry> quad_geometry = new osg::Geometry;

		osg::ref_ptr<osg::Vec3Array> vertex_array = new osg::Vec3Array;
		vertex_array->push_back(osg::Vec3(-_size.x()/2.0, -_size.y()/2.0, 0.0));
		vertex_array->push_back(osg::Vec3(_size.x()/2.0, -_size.y()/2.0, 0.0));
		vertex_array->push_back(osg::Vec3(_size.x()/2.0, _size.y()/2.0, 0.0));
		vertex_array->push_back(osg::Vec3(-_size.x()/2.0, _size.y()/2.0, 0.0));
		quad_geometry->setVertexArray(vertex_array);

		osg::ref_ptr<osg::Vec3Array> normal_array = new osg::Vec3Array;
		normal_array->push_back(osg::Vec3(0.0, 0.0, 1.0));
		quad_geometry->setNormalArray(normal_array, osg::Array::BIND_OVERALL);

		osg::ref_ptr<osg::Vec2Array> texture_array = new osg::Vec2Array;
		texture_array->push_back(osg::Vec2(0, 0));
		texture_array->push_back(osg::Vec2(1, 0));
		texture_array->push_back(osg::Vec2(1, 1));
		texture_array->push_back(osg::Vec2(0, 1));
		quad_geometry->setTexCoordArray(0, texture_array);

		osg::ref_ptr<osg::Vec4Array> color_array = new osg::Vec4Array;
		color_array->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		quad_geometry->setColorArray(color_array, osg::Array::BIND_OVERALL);
		
		quad_geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertex_array->size()));
		quad_geode->addDrawable(quad_geometry);

		// Load image
		std::string fullpath = osgDB::findDataFile(_filename);
		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(fullpath);
		if (image.valid()) {
			osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
			texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
			texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
			texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
			texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
			texture->setImage(image);

			// Apply texture to geometry
			osg::ref_ptr<osg::StateSet> quad_state = quad_geode->getOrCreateStateSet();
			quad_state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
			// Enable transparency in texture
			quad_state->setMode(GL_BLEND, osg::StateAttribute::ON);

			// Disable lighting
			quad_geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		}


		_node = quad_geode->asNode();

	}
	return _node.get();
}
