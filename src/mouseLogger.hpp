/*
 * File: mouseLogger.hpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once
#include <cmath>
namespace mouseLogger {
	//coordinates logger for mouse
	class mouseLogger {

	private:

		double* points;//the coordinates of logged points
		size_t pnum;//the number of logged points
		size_t maxPnum;//limit number
		double minDist = 0.01;//minimum distance between each point

		double oldX;//last x
		double oldY;//last y

	public:
		mouseLogger(size_t maxPointNum = 200, double minDistance=0.01);

		//start logging
		void start(double x, double y);

		//record (x,y)
		void log(double x, double y);
		
		//get pnum
		int getPnum();

		//get points
		double* getPoints();

		//generate points between end point and start point
		bool finishStroke();
	};
}