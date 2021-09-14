/*
 * File: spine.hpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include "edge.hpp"

namespace sketch3D {

	const int SPINE_TYPE_NORMAL = 0;
	const int SPINE_TYPE_JOINT = 1;
	const int SPINE_TYPE_TERMINAL = 2;

	//spine between e1-e2
	//joint->...->p1->p2->...->terminal
	class spine {
	private:
		graph::edge* e1, *e2;//e1=null then joint, e2=null then terminal
		graph::point* p1, * p2;//p1:mid point of e1, p2:mid point of e2
		spine* s[3];//connected spines ...->s1->p1->this->p2->s2->... s3 is for joint spines.

		int spineType; //type0:normal spine, 1:joint spine, 2:terminal spine
		bool flag;
	public:
		spine(graph::edge* e1_, graph::edge* e2_=nullptr, graph::point* p1_=nullptr, graph::point* p2_ = nullptr);
		
		//set points
		void setPoint(graph::point* p1_, graph::point* p2_ = nullptr);

		//get points
		void getPoint(graph::point* p1_, graph::point* p2_ = nullptr);

		//get p1
		graph::point* getP1();

		//get p2
		graph::point* getP2();

		//get coordinates of spine
		void getCoord(double* coord_);

		//set connected spine
		void setSpine(spine* s_, size_t index=3);

		//change this spine to joint type spine
		void changeToJoint(graph::point* p1_, graph::edge* e2_, graph::point* p2_);

		//get type
		int getType();

		//get e1
		graph::edge* getE1();

		//get e2
		graph::edge* getE2();

		//get the spine opposite s_
		spine* getOppositeSpine(spine* s_);

		//get the spine opposite e_
		graph::edge* getOppositeEdge(graph::edge* e_);

		void setFlag(bool f=false);
		bool getFlag();
	};

	void connectSpine(spine* s1, spine* s2);
}