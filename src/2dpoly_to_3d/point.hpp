/*
 * File: point.hpp
 * --------------------
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */
#pragma once

namespace graph{

	class point {
	private:
		double x;
		double y;
		double z;
		size_t index;

	public:
		point(double px, double py, double pz = 0, size_t id = 0);
		void setXYZ(double px, double py, double pz = 0);
		double getX();
		double getY();
		double getZ();
		void setZ(double pz);
		void setID(size_t id);
		size_t getID();
	};

	//c->a X c->b
	double crossprod2D(point* a, point* b, point* c);

	//calcurate normal vector (normalize(c->a X c->b))
	void calNormal(point* a,point* b,point* c, double* normal);

	//c->a * c->b
	double innerprod2D(point* a, point* b, point* c);

	//|a->b|^2
	double squaredDistance(point* a, point* b);

	//|a->b|
	double distance(point* a, point* b);

	//square p1->p2->p3->p4->p1 is convex or not
	bool isConvexSquare(point* p1, point* p2, point* p3, point* p4);

	//edge p1->p2 crosses edge p3->p4 or not
	bool isCross(point* p1, point* p2, point* p3, point* p4);

	//mid point between p1 and p2
	point* mid(point* p1, point* p2);

	//center point of the set (p1, p2, p3)
	point* center(point* p1, point* p2, point* p3);

	//target is in a circle or not. p1 and p2 are on the circle. p1->p2 is the diameter of the circle
	bool isInCircle(point* target, point* p1, point* p2);
}