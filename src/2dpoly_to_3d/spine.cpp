/*
 * File: spine.cpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#include "spine.hpp"
#include "exception.hpp"

namespace sketch3D {

	spine::spine(graph::edge* e1_, graph::edge* e2_, graph::point* p1_, graph::point* p2_)
		:s(), flag(false)
	{
		e1 = e1_;
		e2 = e2_;
		p1 = p1_;
		p2 = p2_;
		if (e2_ == nullptr) {
			spineType = SPINE_TYPE_TERMINAL;
		}
		else if (e1_ == nullptr) {
			spineType = SPINE_TYPE_JOINT;
		}
		else {
			spineType = SPINE_TYPE_NORMAL;
		}
		s[0] = nullptr;
		s[1] = nullptr;
		s[2] = nullptr;
	}

	void spine::setPoint(graph::point* p1_, graph::point* p2_) {
		p1 = p1_;
		p2 = p2_;
	}
	void spine::getPoint(graph::point* p1_, graph::point* p2_) {
		p1_ = p1;
		p2_ = p2;
	}

	graph::point* spine::getP1() {
		return p1;
	}
	graph::point* spine::getP2() {
		return p2;
	}
	void spine::getCoord(double* coord) {
		coord[0] = p1->getX();
		coord[1] = p1->getY();
		coord[2] = p2->getX();
		coord[3] = p2->getY();
	}

	void spine::setSpine(spine* s_, size_t index) {
		if (index==3) index = s[0] == nullptr ? 0 : 1;
		s[index] = s_;
	}

	template <class T>
	void swap(T* a, T* b) {
		T* c;
		c = a;
		a = b;
		b = c;
	}

	void spine::changeToJoint(graph::point* p1_, graph::edge* e2_, graph::point* p2_) {
		e1 = nullptr;
		e2 = e2_;
		p1 = p1_;
		p2 = p2_;
		spineType = SPINE_TYPE_JOINT;
	}

	int spine::getType() {
		return spineType;
	}
	graph::edge* spine::getE1() {
		return e1;
	}
	graph::edge* spine::getE2() {
		return e2;
	}
	spine* spine::getOppositeSpine(spine* s_) {
		return s[0] == s_ ? s[1] : s[0];
	}

	graph::edge* spine::getOppositeEdge(graph::edge* e_) {
		return e1 == e_ ? e2 : e1;
	}

	void spine::setFlag(bool f) {
		flag = f;
	}
	bool spine::getFlag() {
		return flag;
	}

	void connectSpine(spine* s1, spine* s2) {
		s2->setSpine(s1);
		s1->setSpine(s2);
	}
}