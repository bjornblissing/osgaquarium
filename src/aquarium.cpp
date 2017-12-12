/*
* aquarium.cpp
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/

#include "aquarium.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

osg::Node* Aquarium::node() {
	if (!_node.valid()) {
		// Create geometry
		osg::ref_ptr<osg::Geode> quad_geode = new osg::Geode;
		osg::ref_ptr<osg::Geometry> quad_geometry = new osg::Geometry;

		// Get size
		osg::Vec3 size = size_from_aspect(_aspect);
		
		// Vertex
		osg::ref_ptr<osg::Vec3Array> vertex_array = new osg::Vec3Array;
		// Back plane
		vertex_array->push_back(osg::Vec3(-1.0f, -1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(1.0f, -1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(1.0f, 1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, 1.0f, -2.0f));
		// Bottom plane
		vertex_array->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(1.0f, -1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(1.0f, -1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, -1.0f, -2.0f));
		// Top plane
		vertex_array->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, 1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, 1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(1.0f, 1.0f, -2.0f));
		// Left plane
		vertex_array->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, -1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, 1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(-1.0f, 1.0f, 0.0f));
		// Right plane
		vertex_array->push_back(osg::Vec3(1.0f, -1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
		vertex_array->push_back(osg::Vec3(1.0f, 1.0f, -2.0f));
		vertex_array->push_back(osg::Vec3(1.0f, -1.0f, -2.0f));

		quad_geometry->setVertexArray(vertex_array.get());

		// Normals
		osg::ref_ptr<osg::Vec3Array> normal_array = new osg::Vec3Array;
		// Back plane
		normal_array->push_back(osg::Vec3(0.0, 0.0, 1.0));
		normal_array->push_back(osg::Vec3(0.0, 0.0, 1.0));
		normal_array->push_back(osg::Vec3(0.0, 0.0, 1.0));
		normal_array->push_back(osg::Vec3(0.0, 0.0, 1.0));
		// Bottom plane
		normal_array->push_back(osg::Vec3(0.0, 1.0, 0.0));
		normal_array->push_back(osg::Vec3(0.0, 1.0, 0.0));
		normal_array->push_back(osg::Vec3(0.0, 1.0, 0.0));
		normal_array->push_back(osg::Vec3(0.0, 1.0, 0.0));
		// Top plane
		normal_array->push_back(osg::Vec3(0.0, -1.0, 0.0));
		normal_array->push_back(osg::Vec3(0.0, -1.0, 0.0));
		normal_array->push_back(osg::Vec3(0.0, -1.0, 0.0));
		normal_array->push_back(osg::Vec3(0.0, -1.0, 0.0));
		// Left plane
		normal_array->push_back(osg::Vec3(1.0, 0.0, 0.0));
		normal_array->push_back(osg::Vec3(1.0, 0.0, 0.0));
		normal_array->push_back(osg::Vec3(1.0, 0.0, 0.0));
		normal_array->push_back(osg::Vec3(1.0, 0.0, 0.0));
		// Right plane
		normal_array->push_back(osg::Vec3(-1.0, 0.0, 0.0));
		normal_array->push_back(osg::Vec3(-1.0, 0.0, 0.0));
		normal_array->push_back(osg::Vec3(-1.0, 0.0, 0.0));
		normal_array->push_back(osg::Vec3(-1.0, 0.0, 0.0));
		quad_geometry->setNormalArray(normal_array.get(), osg::Array::BIND_PER_VERTEX);

		// Texture coordinates
		osg::ref_ptr<osg::Vec2Array> texture_array = new osg::Vec2Array;
		// Back plane
		texture_array->push_back(osg::Vec2(0.0, 0.0));
		texture_array->push_back(osg::Vec2(size.x(), 0.0));
		texture_array->push_back(osg::Vec2(size.x(), size.y()));
		texture_array->push_back(osg::Vec2(0.0, size.y()));
		// Bottom plane
		texture_array->push_back(osg::Vec2(0.0, 0.0));
		texture_array->push_back(osg::Vec2(size.x(), 0.0));
		texture_array->push_back(osg::Vec2(size.x(), size.z()));
		texture_array->push_back(osg::Vec2(0.0, size.z()));
		// Top plane
		texture_array->push_back(osg::Vec2(size.x(), 0.0));
		texture_array->push_back(osg::Vec2(0.0, 0.0));
		texture_array->push_back(osg::Vec2(0.0, size.z()));
		texture_array->push_back(osg::Vec2(size.x(), size.z()));
		// Left plane
		texture_array->push_back(osg::Vec2(0.0, 0.0));
		texture_array->push_back(osg::Vec2(0.0, size.z()));
		texture_array->push_back(osg::Vec2(size.y(), size.z()));
		texture_array->push_back(osg::Vec2(size.y(), 0.0));
		// Right plane
		texture_array->push_back(osg::Vec2(0.0, 0.0));
		texture_array->push_back(osg::Vec2(size.y(), 0.0));
		texture_array->push_back(osg::Vec2(size.y(), size.z()));
		texture_array->push_back(osg::Vec2(0.0, size.z()));

		quad_geometry->setTexCoordArray(0, texture_array.get());

		osg::ref_ptr<osg::Vec4Array> color_array = new osg::Vec4Array;
		color_array->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		quad_geometry->setColorArray(color_array.get(), osg::Array::BIND_OVERALL);

		quad_geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertex_array->size()));
		quad_geode->addDrawable(quad_geometry);

		// Load image
		std::string fullpath = osgDB::findDataFile("../textures/grid.png");
		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(fullpath);
		if (image.valid()) {
			osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
			texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
			texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
			texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			texture->setMaxAnisotropy(4.0f);
			texture->setImage(image);

			// Apply texture to geometry
			osg::ref_ptr<osg::StateSet> quad_state = quad_geode->getOrCreateStateSet();
			quad_state->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

			// Disable lighting
			quad_geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		}

		_node = quad_geode->asNode();
	}
	return _node.get();
}

osg::Group* Aquarium::group() {
	if (!_group.valid()) {
		_group = new osg::Group;
		_group->addChild(node());
	
		osg::ref_ptr<Fish> fish1 = new Fish("../textures/fish1.png", osg::Vec2(0.6f, 0.3f));
		osg::ref_ptr<Fish> fish2 = new Fish("../textures/fish2.png", osg::Vec2(0.3f, 0.15f));
		osg::ref_ptr<Fish> fish3 = new Fish("../textures/fish3.png", osg::Vec2(0.4f, 0.2f));
		
		_group->addChild(add_fish_at_position(fish1, osg::Vec3(0.0, 0.0, 0.5)));
		_group->addChild(add_fish_at_position(fish1, osg::Vec3(0.5, 0.5, 0.75)));

		_group->addChild(add_fish_at_position(fish2, osg::Vec3(-0.5, -0.5, 0.5)));
		_group->addChild(add_fish_at_position(fish2, osg::Vec3(0.0, 0.0, 0.25)));

		_group->addChild(add_fish_at_position(fish3, osg::Vec3(0.5, -0.5, 0.5)));
		_group->addChild(add_fish_at_position(fish3, osg::Vec3(-0.5, 0.5, 0.25)));
	}
	return _group.get();
}

osg::PositionAttitudeTransform* Aquarium::add_fish_at_position(osg::ref_ptr<Fish> fish, const osg::Vec3f& position) {
	osg::Vec3f local_pos = componentMultiply(position, osg::Vec3(1.0, 1.0, -2.0));
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->setPosition(local_pos);
	pat->setScale(osg::Vec3(1.0, _aspect, 1.0));
	pat->addChild(fish->node());
	return pat.release();
}

osg::Vec3 Aquarium::size_from_aspect(double aspect) const {
	osg::Vec3 aspect_ratio(10.0, 10.0, 10.0);
	if (aspect < 1.01) {
		// 1:1 aspect ratio
		aspect_ratio = osg::Vec3(10.0, 10.0, 10.0);
	}
	else if (aspect < 1.26) {
		// 5:4 aspect ratio
		aspect_ratio = osg::Vec3(10.0, 8.0, 8.0);
	}
	else if (aspect < 1.34) {
		// 4:3 aspect ratio
		aspect_ratio = osg::Vec3(12.0, 9.0, 9.0);
	}
	else if (aspect < 1.51) {
		// 3:2 aspect ratio
		aspect_ratio = osg::Vec3(14.0, 8.0, 8.0);
	}
	else if (aspect < 1.61) {
		// 16:10 aspect ratio
		aspect_ratio = osg::Vec3(16.0, 10.0, 10.0);
	}
	else if (aspect < 1.67) {
		// 5:3 aspect ratio
		aspect_ratio = osg::Vec3(15.0, 9.0, 9.0);
	}
	else if (aspect < 1.78) {
		// 16:9 aspect ratio
		aspect_ratio = osg::Vec3(16.0, 9.0, 9.0);
	}
	else {
		// Really large aspect ratio, probably 21:9
		aspect_ratio = osg::Vec3(21.0, 9.0, 9.0);
	}

	return aspect_ratio;
}
