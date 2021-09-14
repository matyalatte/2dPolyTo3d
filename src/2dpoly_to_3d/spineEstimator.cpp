/*
 * File: spineEstimator.cpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#include "spineEstimator.hpp"

namespace sketch3D {

	spineEstimator::spineEstimator(graph::graph* graph) 
		:baseGraphHandler(graph, "spineEstimator"),
		 spines(),pointNum(0){}

	spineEstimator::~spineEstimator() {
		for (size_t i = 0; i < getSpineNum(); i++) {
			delete spines[i];
		}
	}

	void spineEstimator::addSpine(graph::edge* e1, graph::edge* e2, graph::point* p1, graph::point* p2) {
		spines.push_back(new spine(e1,e2,p1,p2));
	}

	void spineEstimator::init() {
		spines.resize(0);
		spines.shrink_to_fit();
		pointNum = getPointNum();
	}

	/*In the following case, this method will not count the edges crossing correctly.
	* @:point, -:edge
	*
	*               |
	*               |
	* @-----@=======@----@
	*       |
	*       |
	* 
	*/
	void spineEstimator::deleteExternalEdge() {
		double maxX = directedGraph->getMaxX();
		size_t point_size = getPointNum();
		size_t edge_size = getEdgeNum();
		graph::edge* e;
		graph::point* p1;
		graph::point* p2;
		graph::point* p3;
		graph::point* p4;
		size_t p1_id;
		size_t p2_id;

		int count;

		directedGraph->resetFlag();
		for (size_t i = 0; i < edge_size; i++) {
			e = getEdge(i);
			if (e->getFlag()) continue;

			p1 = e->getP1();
			p2 = e->getP2();
			e->getPointID(&p1_id, &p2_id);

			//if edge e is an outer edge then continue
			if ((p1_id + 1) % point_size == p2_id || (p2_id + 1) % point_size == p1_id) continue;

			graph::point* mid = graph::mid(p1, p2);
			graph::point* end = new graph::point(directedGraph->getMaxX() + 1.0, mid->getY());

			//check edge e is an external edge or not.
			count = 0;
			for (size_t j = 0; j < point_size; j++) {
				p3 = getPoint(j);
				p4 = getPoint((j + 1) * (int)(j + 1 < point_size));
				if (graph::isCross(mid, end, p3, p4)) count += 1;

				double p3x = p3->getX();
				double p3y = p3->getY();
				if (p3y == mid->getY() && mid->getX() <= p3x && p3x <= end->getX()) {
					double y=getPoint(((int)j - 1 + point_size) % point_size)->getY();
					if ((p4->getY()-p3y)*(y-p3y)<0) count += 1;
				}
			}
			if (count % 2 == 0) setFlagToFace(e);//edge e is external!
		}

		//delete external edges
		deleteEdgeFlagIsTrue();

		//if constraint edges are deleted then throw exception
		checkAllConstraintEdgeExist(point_size, true);
	}

	//generate spines
	void spineEstimator::genSpineFromChordalAxis(graph::edge* e, graph::point* p,spine* s) {
		graph::edge* prev=e->getPrevEdge();
		graph::edge* next=prev->getPrevEdge();
		graph::edge* pair = e->getPairEdge();

		if (pair == nullptr) {//outer edge
			if (p == nullptr) genSpineFromChordalAxis(prev);
			return;
		}

		graph::point* mid;

		if (p == nullptr){
			mid = e->getMidPoint(true);
			directedGraph->addPointObject(mid);		
		}
		else{
			mid = p;
		}
		graph::point* p3;
		spine* newS;

		bool prevPairIsNull = prev->getPairEdge() == nullptr;
		bool nextPairIsNull = next->getPairEdge() == nullptr;
		if (prevPairIsNull && nextPairIsNull) {
			//gen terminal spine
			p3 = e->getP3();
			addSpine(e, nullptr, mid, p3);
			newS = getLastSpine();
			if (s != nullptr) connectSpine(s, newS);
		}
		else if (!prevPairIsNull && !nextPairIsNull) {
			graph::point* mid2, * mid3;
			spine* s2, * s3;

			//gen joint spine
			p3 = e->getCenterPoint(true);
			addPointObject(p3);
			addSpine(nullptr, e, p3, mid);
			newS = getLastSpine();
			if (s != nullptr) connectSpine(s, newS);

			mid2 = prev->getMidPoint(true);
			addPointObject(mid2);
			addSpine(nullptr, prev, p3, mid2);
			s2 = getLastSpine();

			mid3 = next->getMidPoint(true);
			addPointObject(mid3);
			addSpine(nullptr, next, p3, mid3);
			s3 = getLastSpine();

			s2->setSpine(newS);
			s2->setSpine(s3, 2);
			s3->setSpine(newS);
			s3->setSpine(s2, 2);

			genSpineFromChordalAxis(prev->getPairEdge(), mid2, s2);
			genSpineFromChordalAxis(next->getPairEdge(), mid3, s3);

			directedGraph->insertPoint(p3, e);
		}
		else {
			//gen normal spine
			graph::edge* e2;
			if (prevPairIsNull) {
				e2 = next;
			}
			else {
				e2 = prev;
			}
			p3 = e2->getMidPoint(true);
			addPointObject(p3);
			addSpine(e, e2, mid, p3);
			newS = getLastSpine();
			if (s != nullptr) connectSpine(s, newS);
			genSpineFromChordalAxis(e2->getPairEdge(), p3, newS);
		}
		if (p == nullptr) {
			genSpineFromChordalAxis(pair, mid, newS);
		}
	}

	void spineEstimator::genFanTriangle(graph::point* p1, graph::point* p2, graph::point* center, size_t pnum, graph::edge* se_prev, graph::edge* se_next) {
		
		graph::point* start, *next;
		size_t centerID = center->getID();
		if ((p1->getID() + pnum) % pointNum == p2->getID()) { start = p1;}
		else if ((p2->getID() + pnum) % pointNum == p1->getID()) { start = p2;}
		else { throw graph::graphException("spineEstimator", "genFanTriangleGen: unexpected parameters detected"); }

		size_t edgesize = getEdgeNum();
		next = getPoint((start->getID() + 1) % pointNum);
		bool direction = graph::crossprod2D(start, next, center)>0;

		for (size_t i = 0; i < pnum; i++) {
			directedGraph->addFace((start->getID() + i+!direction) % pointNum, (start->getID() + i + direction) % pointNum, centerID);
		}
		for (size_t i = 0; i < pnum-1; i++) {
			directedGraph->makePair(edgesize + 1 + i * 3+ !direction, edgesize + 5 + i * 3-!direction);
		}

		if (se_prev != nullptr) {
			graph::edge* e, *pe, * ne;
			if (direction) {
				pe = se_prev;
				ne = se_next;
			}
			else {
				pe = se_next;
				ne = se_prev;
			}
			e = getEdge(edgesize + 2 - !direction);
			e->setPairEdge(pe);
			pe->setPairEdge(e);

			e = getEdge(edgesize + 1 + (pnum - 1) * 3 + !direction);
			e->setPairEdge(ne);
			ne->setPairEdge(e);
		}
	}

	void spineEstimator::genFanTriangleRec(graph::edge* e, std::vector<graph::point*>& pvec, spine* s, spine* preS) {

		//if s is joint then generate fan triangle
		if (s->getType() == SPINE_TYPE_JOINT) {
			s->setFlag(true);

			graph::edge* s_e = s->getOppositeEdge(nullptr);
			if (s_e == nullptr) throw graph::graphException("spineEstimator", "genFanTriangleRec: unexpected edge detected");
			setFlagToFace(s_e);
			graph::edge* prev = s_e->getPrevEdge();
			graph::edge* next = prev->getPrevEdge();
			
			genFanTriangle(s_e->getP1(), s_e->getP2(), s->getP1(), pvec.size() + 1, prev->getPairEdge(), next->getPairEdge());

			prev->setPairEdge(nullptr);
			next->setPairEdge(nullptr);

			return;
		}

		
		graph::point* p1 = e->getP1();
		graph::point* p2 = e->getP2();
		graph::edge* pair = e->getPairEdge();

		bool flag = false;


		graph::point* p3 = e->getP3();
		pvec.push_back(p3);

		graph::point* p;
		for (size_t i = 0; i < pvec.size(); i++) {
			p = pvec[i];
			if (!isInCircle(p, p1, p2)) {
				flag = true;
				break;
			}
		}
		if (flag) {
			if (s->getType() == SPINE_TYPE_TERMINAL) {
				s->setFlag(true);
				return;
			}
			graph::edge* e2;
			graph::point* s_p2;
			if (s->getE1() == e) { e2 = s->getE2(); p = s->getP2(); s_p2 = s->getP1(); }
			else {
				if (s->getE2() == nullptr) throw graph::graphException("spineEstimator", "genFanTriangleRec: unexpected edge detected");
				p = s->getP1();
				e2 = s->getE1();
				s_p2 = s->getP2();
			}
			
			p1 = e2->getP1();
			p2 = e2->getP2();
			pair = e2->getPairEdge();
			if (p->getID() < pointNum) throw graph::graphException("spineEstimator", "genFanTriangleRec: unexpected point index detected");

			pair->setPairEdge(nullptr);
			e2->setPairEdge(nullptr);

			directedGraph->insertPoint2(p, e2);
			size_t edgeNum = getEdgeNum();

			genFanTriangle(p1, p2, p, pvec.size(),e2,getEdge(edgeNum-2));
			
			s->changeToJoint(p,e, s_p2);
			return;
		}
		s->setFlag(true);

		setFlagToFace(e);
		spine* s2 = s->getOppositeSpine(preS);
		graph::edge* e2 = s2->getOppositeEdge(pair);
		genFanTriangleRec(e2, pvec, s2, s);
	}

	void spineEstimator::searchSpineToGenFan() {
		directedGraph->resetFlag();
		resetSpineFlag();

		spine* s;
		graph::edge* e;
		std::vector<graph::point*> pvec;

		//find terminal edges
		for (size_t i = 0; i < getSpineNum(); i++) {
			s = spines[i];
			if (s->getType() == SPINE_TYPE_TERMINAL) {
				pvec.resize(0);
				pvec.shrink_to_fit();
				e = s->getE1();
				//generate fan triangles
				genFanTriangleRec(e, pvec, s);
			}
		}

		//delete unused edges and spines
		deleteEdgeFlagIsTrue();
		deleteSpineFlagIsTrue();
	}

	void spineEstimator::resetSpineFlag() {
		for (size_t i = 0; i < getSpineNum(); i++) {
			spines[i]->setFlag();
		}
	}

	void spineEstimator::deleteSpineFlagIsTrue() {
		size_t spine_size = getSpineNum();
		spine* s;
		for (int i = (int)spine_size - 1; i >= 0; i--) {
			s = spines[i];
			if (s->getFlag()) {
				delete s;
				spines.erase(spines.begin() + i);
			}
		}
	}

	void spineEstimator::deleteUnusedPoint() {
		size_t point_size = getPointNum();
		size_t spine_size = getSpineNum();

		spine* s;
		size_t p1_id;
		size_t p2_id;
		bool* flag=new bool[point_size - pointNum];
		for (size_t i = 0; i < point_size - pointNum; i++) {
			flag[i] = true;
		}

		for (size_t i = 0; i < spine_size; i++) {
			s = spines[i];
			p1_id = s->getP1()->getID();
			p2_id=s->getP2()->getID();
			flag[p1_id - pointNum] = false;
			flag[p2_id - pointNum] = false;
		}

		for (int i = (int)point_size -1; i >= pointNum; i--) {
			if (flag[(size_t)i-pointNum]) {
				directedGraph->deletePoint(i);
			}
		}
		directedGraph->resetPointID();
		delete[] flag;
	}
	
	void spineEstimator::splitFaceBySpine() {
		size_t point_size = getPointNum();
		size_t spine_size = getSpineNum();

		spine* s;
		size_t p1_id;
		size_t p2_id;

		bool* flag = new bool[point_size - pointNum];
		for (size_t i = 0; i < point_size - pointNum; i++) {
			flag[i] = false;
		}

		for (size_t i = 0; i < spine_size; i++) {
			s = spines[i];
			
			p1_id = s->getP1()->getID();
			p2_id = s->getP2()->getID();

			if (!flag[p2_id - pointNum]) {
				directedGraph->insertPoint2(s->getP2(), s->getE2());
				flag[p2_id-pointNum] = true;
			}
			if (s->getType() == SPINE_TYPE_NORMAL && !flag[p1_id - pointNum]) {
				directedGraph->insertPoint2(s->getP1(), s->getE1());
				flag[p1_id - pointNum] = true;
			}
		}

	}

	void spineEstimator::estimateSpine() {
		init();
		deleteExternalEdge();
		genSpineFromChordalAxis(getEdge(0));
		searchSpineToGenFan();
		deleteUnusedPoint();
		splitFaceBySpine();
	}

	void spineEstimator::getSpinesAsCoords(double* spine_coords) {
		size_t size = spines.size();
		if (size < 1) return;

		double* coords = new double[4];
		for (size_t i = 0; i < size; i++) {
			spines[i]->getCoord(coords);
			memmove(&spine_coords[i*4], coords, sizeof(double)*4);
		}
		delete[] coords;
	}

	size_t spineEstimator::getSpineNum() {
		return spines.size();
	}

	spine* spineEstimator::getLastSpine() {
		return spines[getSpineNum()-1];
	}

	int spineEstimator::getSpineType(size_t i) {
		return spines[i]->getType();
	}
	
}