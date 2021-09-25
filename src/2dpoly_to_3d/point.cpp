/*
 * File: point.cpp
 * --------------------
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */
#include "point.hpp"
#include <cmath>

namespace graph {

	point::point(double px, double py, double pz, size_t id) {
		x = px;
		y = py;
		z = pz;
		index = id;
	}

	void point::setXYZ(double px, double py, double pz) {
		x = px;
		y = py;
		z = pz;
	}

	double point::getX() {
		return x;
	}

	double point::getY() {
		return y;
	}

	double point::getZ() {
		return z;
	}

	void point::setZ(double pz) {
		z = pz;
	}

	void point::setID(size_t id) {
		index = id;
	}
	size_t point::getID() {
		return index;
	}

	//c->a X c->b
	double crossprod2D(point* a, point* b, point* c) {
		return (a->getX() - c->getX()) * (b->getY() - c->getY()) - (b->getX() - c->getX()) * (a->getY() - c->getY());
	};

	void calNormal(point* a, point* b, point* c, double* normal) {
		double ax = a->getX() - c->getX();
		double ay = a->getY() - c->getY();
		double az = a->getZ() - c->getZ();
		double bx = b->getX() - c->getX();
		double by = b->getY() - c->getY();
		double bz = b->getZ() - c->getZ();
		normal[0] = ay * bz - az * by;
		normal[1] = az * bx - ax * bz;
		normal[2] = ax * by - ay * bx;		
		double d = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
		normal[0] = normal[0] / d;
		normal[1] = normal[1] / d;
		normal[2] = normal[2] / d;
	}

	//c->a * c->b
	double innerprod2D(point* a, point* b, point* c) {
		return (a->getX() - c->getX()) * (b->getX() - c->getX()) + (b->getY() - c->getY()) * (a->getY() - c->getY());
	};

	//|a->b|^2
	double squaredDistance(point* a, point* b) {
		double x = a->getX() - b->getX();
		double y = a->getY() - b->getY();
		return x * x + y * y;
	}

	//|a->b|
	double distance(point* a, point* b) {
		return sqrt(squaredDistance(a, b));
	}

	//square p1->p2->p3->p4->p1 is convex or not
	bool isConvexSquare(point* p1, point* p2, point* p3, point* p4) {
		return (crossprod2D(p1, p2, p3) > 0) ==
			(crossprod2D(p2, p3, p4) > 0) ==
			(crossprod2D(p3, p4, p1) > 0) ==
			(crossprod2D(p4, p1, p2) > 0);
	}

	//edge p1->p2 crosses edge p3->p4 or not
	bool isCross(point* p1, point* p2, point* p3, point* p4) {
		return (crossprod2D(p2, p3, p1) * crossprod2D(p2, p4, p1) < 0) &&
			(crossprod2D(p4, p1, p3) * crossprod2D(p4, p2, p3) < 0);
	}

	//mid point between p1 and p2
	point* mid(point* p1, point* p2) {
		return new point((p1->getX() + p2->getX()) / 2, (p1->getY() + p2->getY()) / 2);
	}

	

	//center point of the set (p1, p2, p3)
	point* center(point* p1, point* p2, point* p3) {
		point* p = new point((p1->getX() + p2->getX() + p3->getX()) / 3,
			(p1->getY() + p2->getY() + p3->getY()) / 3);
		return p;
	}

	//target is in a circle or not. p1 and p2 are on the circle. p1->p2 is the diameter of the circle
	bool isInCircle(point* target, point* p1, point* p2) {
		point* p = mid(p1, p2);
		double centerX = (p1->getX() + p2->getX()) / 2;
		double centerY = (p1->getY() + p2->getY()) / 2;
		double radius = distance(p1, p2) / 2;
		bool ret = (distance(p, target) <= radius);
		delete p;
		return ret;
	}
}