/*
* headposition.h
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/
#pragma once

#include <osgGA/CameraManipulator>

#include "headtracker.h"

class HeadPositionManipulator : public osgGA::CameraManipulator {
public:

	explicit HeadPositionManipulator(double depth);

	const char* className() const override { return "HeadPositionManipulator"; }

	/** set the position of the matrix manipulator using a 4x4 Matrix.*/
	void setByMatrix(const osg::Matrixd& matrix) override { _viewMatrix = osg::Matrixd(matrix); }

	/** set the position of the matrix manipulator using a 4x4 Matrix.*/
	void setByInverseMatrix(const osg::Matrixd& matrix) override { setByMatrix(osg::Matrixd::inverse(matrix)); }

	/** get the position of the manipulator as 4x4 Matrix.*/
	osg::Matrixd getMatrix() const override { return _viewMatrix; }

	/** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
	osg::Matrixd getInverseMatrix() const override { return osg::Matrixd::inverse(_viewMatrix); }

	/* update the camera for the current frame */
	void updateCamera(osg::Camera& camera) override ;

private:
	osg::Matrix _viewMatrix = { osg::Matrix::identity() };
	osg::Matrix _projectionMatrix = { osg::Matrix::identity() };
	osg::ref_ptr<HeadTracker> _tracker = { nullptr };
	double _depth = { 1.0 };
};