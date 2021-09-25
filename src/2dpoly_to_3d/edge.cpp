/*
 * File: edge.cpp
 * --------------------
 * This file contains the implementation of half edge.
 *
 * See edge.hpp for documentation of each member.
 *
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#include "edge.hpp"
#include <stdio.h>

namespace graph {

	edge::edge(point* p1_, point* p2_, edge* pairE_, edge* prevE_)
		:flag(false)
	{
		p1 = p1_;
		p2 = p2_;
		pairE = pairE_;
		prevE = prevE_;
	}

	edge::~edge() {
		disconnectPair();
	}

	double edge::len_sq() {
		return squaredDistance(p1,p2);
	}

	double edge::len() {
		return distance(p1,p2);
	}

	void edge::setPoint(point* p1_, point* p2_) {
		p1 = p1_;
		p2 = p2_;
	}

	void edge::getPoint(point* p1_, point* p2_) {
		p1_ = p1;
		p2_ = p2;
	}

	void edge::getPointID(size_t* p1_id, size_t* p2_id) {
		*p1_id = p1->getID();
		*p2_id = p2->getID();
	}

	point* edge::getP1() {
		return p1;
	}

	point* edge::getP2() {
		return p2;
	}

	point* edge::getP3() {
		return getPrevEdge()->getP1();
	}

	void edge::getCoord(double* coord) {
		coord[0] = p1->getX();
		coord[1] = p1->getY();
		coord[2] = p2->getX();
		coord[3] = p2->getY();
	}

	void edge::setPairEdge(edge* e) {
		pairE = e;
	}

	edge* edge::getPairEdge() {
		return pairE;
	}

	void edge::setPrevEdge(edge* e) {
		prevE = e;
	}

	edge* edge::getPrevEdge() {
		return prevE;
	}

	void edge::setFlag(bool f) {
		flag = f;
	}

	void edge::disconnectPair() {
		if (pairE!=nullptr) pairE->setPairEdge(nullptr);
	}

	bool edge::getFlag() {
		return flag;
	}

	point* edge::getMidPoint() {
		return mid(p1, p2);
	}
	point* edge::getCenterPoint() {
		return center(p1, p2, getP3());
	}

	void edge::printIDs() {
		printf("(%d,%d)",(int)getP1()->getID(), (int)getP2()->getID());
	}

	void makeEdgePair(edge* e1, edge* e2) {
		e1->setPairEdge(e2);
		e2->setPairEdge(e1);
	}
}