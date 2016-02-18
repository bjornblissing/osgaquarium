/*
* headtracker.cpp
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/

#include "headtracker.h"

#include <OpenThreads/ScopedLock>

HeadTracker::HeadTracker() : _tracker_thread(0) {}

HeadTracker::~HeadTracker() {
	while (_tracker_thread->isRunning()) {
		_tracker_thread->cancel();
	}
	
	delete _tracker_thread;
}

void HeadTracker::open_camera(int deviceId, int sensorX, int sensorY, int fps)
{
	_tracker_thread = new TrackerThread(deviceId, sensorX, sensorY, fps);
	_tracker_thread->start();
}

TrackerThread::TrackerThread(int deviceId, int sensorX, int sensorY, int fps) : OpenThreads::Thread()
{
	// Open video device
	_camera_device = new cv::VideoCapture(deviceId);

	// check if we succeeded
	if (!_camera_device->isOpened()) {
		return;
	}

	// Try set camera properties
	_camera_device->set(cv::CAP_PROP_FRAME_WIDTH, (double) sensorX);
	_camera_device->set(cv::CAP_PROP_FRAME_HEIGHT, (double) sensorY);
	_camera_device->set(cv::CAP_PROP_FPS, (double) fps);

	// Get camera properties
	_sensor_size_x = static_cast<int> (_camera_device->get(cv::CAP_PROP_FRAME_WIDTH));
	_sensor_size_y = static_cast<int> (_camera_device->get(cv::CAP_PROP_FRAME_HEIGHT));
	_frame_rate = _camera_device->get(cv::CAP_PROP_FPS);

	// Grab one image to determine format
	_camera_device->grab();
	_camera_device->retrieve(_frame);

	// Make sure that grabbed images is continuous
	if (!_frame.isContinuous()) {
		return;
	}

	// Load haar cascade for face tracking
	_face_cascade.load("../haarcascades/haarcascade_frontalface_alt.xml");

	_init = true;
}

void TrackerThread::run()
{
	while (!_done) {
		if (_init) {
			// Get new image
			_camera_device->grab();
			_camera_device->retrieve(_frame);
			detectFaces(_frame);
		}
		{
			OpenThreads::ScopedLock< OpenThreads::Mutex > lock(_mutex);
			_frontBufferPosition = _backBufferPosition;
		}
		OpenThreads::Thread::microSleep(1);
	}
}

int TrackerThread::cancel()
{
	_done = true;

	while (isRunning()) {
		YieldCurrentThread();
	}

	// Release camera device
	while (_camera_device->isOpened()) {
		_camera_device->release();
		OpenThreads::Thread::microSleep(1);
	}

	_init = false;
	
	delete _camera_device;
	return 0;
}

osg::Vec3 TrackerThread::position() const {
	OpenThreads::ScopedLock< OpenThreads::Mutex > lock(_mutex);

	// Proportional controller
	return _frontBufferPosition;
}

void TrackerThread::detectFaces(const cv::Mat& frame) {
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;
	// Convert to gray scale
	cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
	// Equalize histogram
	cv::equalizeHist(frame_gray, frame_gray);

	// Detect faces
	_face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	// Iterate over all of the faces
	for (auto face : faces) {
		// Find center of faces
		cv::Point center(face.x + face.width / 2, face.y + face.height / 2);
			
		_position_delta = center - _position;
		
		_area_delta = face.area() - _area;
		// We only care for the largest detected face
		break;
	}
	
	// P-controller to smooth tracking
	_position_delta.x = _position_delta.x / 2;
	_position_delta.y = _position_delta.y / 2;
	_area_delta /= 4;

	_position += _position_delta;
	_area += _area_delta;
	
	// No negative areas allowed
	if (_area < 1) _area = 1;
	
	// Area to distance
	double distance = 100.0 /sqrt(double(_area));
	
	// No closer than 0.3 meters
	if (distance < 0.3) distance = 0.3;
	// No further than 2.0 meters
	if (distance > 2.0) distance = 2.0;

	// Scale position from -1.0 to 1.0 in X and Y
	_backBufferPosition = osg::Vec3(-float(_position.x - (_sensor_size_x/2)) / float(_sensor_size_x / 2), -float(_position.y - (_sensor_size_y / 2)) / float(_sensor_size_y / 2), distance);
}
