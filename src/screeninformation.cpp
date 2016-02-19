/*
* screeninformation.cpp
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/
#include "screeninformation.h"

osg::Vec2i ScreenInformation::get_screen_size() const
{
	// Try to get the window system interface
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return osg::Vec2i(0, 0);
	}

	// Get the screen identifiers set in environment variable DISPLAY
	osg::GraphicsContext::ScreenIdentifier si;
	si.readDISPLAY();

	// If displayNum has not been set, reset it to 0.
	if (si.displayNum < 0)
	{
		si.displayNum = 0;
		osg::notify(osg::INFO) << "Couldn't get display number, setting to 0" << std::endl;
	}

	// If screenNum has not been set, reset it to 0.
	if (si.screenNum < 0)
	{
		si.screenNum = 0;
		osg::notify(osg::INFO) << "Couldn't get screen number, setting to 0" << std::endl;
	}

	unsigned int width, height;
	wsi->getScreenResolution(si, width, height);

	return osg::Vec2i(width, height);
}

osg::Vec2i ScreenInformation::size() const {
	if (!_init) _size = get_screen_size();

	return _size;
}

double ScreenInformation::aspect_ratio() const {
	if (!_init) _size = get_screen_size();

	return double(_size.x()) / double(_size.y());
}



