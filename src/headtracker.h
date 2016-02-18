/*
* headtracker.h
*
*  Created on: Feb 16, 2016
*      Author: Bjorn Blissing
*/
#pragma once

#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>

#include <osg/Referenced>
#include <osg/Vec3>

#include <opencv2/opencv.hpp>

class TrackerThread : public OpenThreads::Thread {
public:
	TrackerThread(int deviceId, int sensorX, int sensorY, int fps);
	virtual void run();
	virtual int cancel();
	bool initialized() const { return _init; }
	int sensor_size_x() const { return _sensor_size_x; }
	int sensor_size_y() const { return _sensor_size_y; }
	double frame_rate() const { return _frame_rate; }
	osg::Vec3 position() const;
private:
	void detectFaces(const cv::Mat& frame);

	bool _init = { false };
	bool _done = { false };
	cv::VideoCapture* _camera_device = { nullptr };
	cv::Mat _frame;
	cv::CascadeClassifier _face_cascade;
	cv::Point _position;
	cv::Point _position_delta;
	int _area = { 0 };
	int _area_delta = { 0 };
	osg::Vec3 _backBufferPosition = { osg::Vec3(0.0, 0.0, 0.0) };
	osg::Vec3 _frontBufferPosition = { osg::Vec3(0.0, 0.0, 0.0) };
	int _sensor_size_x = { 0 };
	int _sensor_size_y = { 0 };
	double _frame_rate = { 0.0 };
	mutable OpenThreads::Mutex _mutex;
};

class HeadTracker : public osg::Referenced {
public:
	HeadTracker();

	void open_camera(int deviceId, int sensorX = 640, int sensorY=480, int fps=30);
	osg::Vec3 position() const { return _tracker_thread ? _tracker_thread->position() : osg::Vec3(0.0, 0.0, 0.0); }

protected:
	~HeadTracker(); // Since we inherit from osg::Referenced we must make destructor protected
private:
	TrackerThread* _tracker_thread = { nullptr };
};

