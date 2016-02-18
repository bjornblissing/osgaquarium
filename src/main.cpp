/*
 * main.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Bjorn Blissing
 */

#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include "aquarium.h"
#include "screeninformation.h"
#include "headpositionmanipulator.h"

int main( int argc, char** argv )
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);

	// Create viewer
	osgViewer::Viewer viewer(arguments);

	// Get screen properties
	osg::ref_ptr<ScreenInformation> screen = new ScreenInformation;
	
	//  Create objects
	osg::ref_ptr<Aquarium> aquarium = new Aquarium(screen->aspect_ratio());
	
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.setSceneData(aquarium->group());

	// Create a new head tracked manipulator
	osg::ref_ptr<HeadPositionManipulator> manipulator = new HeadPositionManipulator(aquarium->depth());

	// Connect the manipulator to the viewer
	viewer.setCameraManipulator(manipulator);

	viewer.run();

	return 0;
}
