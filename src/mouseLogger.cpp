/*
 * File: mouseLogger.cpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#include "mouseLogger.hpp"

namespace mouseLogger {

	double length(double x, double y) {
		return sqrt(x * x + y * y);
	}

	double dist(double x1, double y1, double x2, double y2) {
		return length(x1 - x2, y1 - y2);
	}

	mouseLogger::mouseLogger(size_t maxPointNum, double minDistance)
		:oldX(0), oldY(0), pnum(0), points(),maxPnum(0) {
		points = new double[maxPointNum * 2];
		maxPnum = maxPointNum;
		minDist = minDistance;
	}

	void mouseLogger::start(double x, double y) {
		oldX = x;
		oldY = y;
		pnum = 1;
		points[0] = x;
		points[1] = y;
	}

	void mouseLogger::log(double x, double y) {
		double d = dist(x, y, oldX, oldY);
		if (d < minDist) return;
		if (pnum == maxPnum) return;

		if (d >= 2*minDist) {
			points[pnum * 2] = (x+oldX)/2;
			points[pnum * 2 + 1] = (y+oldY)/2;
			pnum += 1;
		}
		if (pnum == maxPnum) return;

		points[pnum * 2] = x;
		points[pnum * 2 + 1] = y;
		pnum += 1;

		oldX = x;
		oldY = y;
	}

	int mouseLogger::getPnum() {
		return pnum;
	}

	double* mouseLogger::getPoints() {
		return &points[0];
	}

	bool mouseLogger::finishStroke() {
		double startX = points[0];
		double startY = points[1];
		double vecX = startX - oldX;
		double vecY = startY - oldY;
		double d = length(vecX,vecY)/minDist;
		if (d == 0) {
			pnum -= 1;
			return true;
		}
		vecX = vecX / d;
		vecY = vecY / d;
		double newX = oldX;
		double newY = oldY;
		bool flag=true;
		while (dist(newX,newY,startX,startY)>= minDist) {
			if (pnum == maxPnum) { flag = false; break; }
			newX += vecX;
			newY += vecY;
			points[pnum * 2] = newX;
			points[pnum * 2+1] = newY;
			pnum += 1;
		}
		return flag;
	}
}