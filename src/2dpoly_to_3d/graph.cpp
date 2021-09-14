/*
 * File: graph.cpp
 * --------------------
 * This file contains the implementation of directed graph.
 *
 * See graph.hpp for declaration of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#include "graph.hpp"
#include <cmath>



namespace graph {

	graph::graph()
		:points(), edges(), maxX(0), minX(0), maxY(0), minY(0) {}

	graph::~graph() {
		size_t pointNum = getPointNum();
		for (size_t i = 0; i < pointNum; i++) {
			delete getPoint(i);
		}
		size_t edgeNum = getEdgeNum();
		for (size_t i = 0; i <edgeNum; i++) {
			delete getEdge(i);
		}
		init();
	}

	void graph::init() {
		points.resize(0);
		points.shrink_to_fit();
		edges.resize(0);
		edges.shrink_to_fit();
	}

	double graph::getMaxX() {
		return maxX;
	}

	double graph::getMinX() {
		return minX;
	}
	double graph::getMaxY() {
		return maxY;
	}
	double graph::getMinY() {
		return minY;
	}

	//*point functions*

	size_t graph::getPointNum() {
		return points.size();
	}

	void graph::addPoint(double px, double py, double pz) {
		size_t size = points.size();
		points.resize(size + 1);
		points[size] = new point(px, py, pz, size);
	}

	void graph::addPointObject(point* p) {
		size_t size = points.size();
		points.resize(size + 1);
		p->setID(size);
		points[size] = p;
	}

	point* graph::getPoint(size_t index) {
		return points[index];
	}

	void graph::addPoints(double* point_coords, size_t pointNum) {
		if (pointNum < 3) throw graphException("graph", "addPoints : 'pointNum' should be more equal than 3");
		//init points vector
		points.resize(pointNum);

		double x, y;
		maxX = DBL_MIN, maxY = DBL_MIN, minX = DBL_MAX, minY = DBL_MAX;

		//add points
		for (size_t i = 0; i < pointNum; i++) {
			x = point_coords[i * 2];
			y = point_coords[i * 2 + 1];
			maxX = max(x, maxX);
			maxY = max(y, maxY);
			minX = min(x, minX);
			minY = min(y, minY);
			points[i] = new point(x, y, 0, i);
		}
	}

	void graph::resetPointID() {
		for (size_t i = 0; i < getPointNum(); i++) {
			points[i]->setID(i);
		}
	}

	void graph::deletePoint(size_t index) {
		delete getPoint(index);
		points.erase(points.begin() + index);
	}

	void graph::deleteLastPoint(size_t deleteNum) {
		size_t size = points.size();
		for (size_t i = 0; i < deleteNum; i++) {
			delete points[size - i - 1];
		}
		points.erase(points.begin() + size - deleteNum, points.begin() + size);
	}

	//*edge functions*

	size_t graph::getEdgeNum() {
		return edges.size();
	}

	void graph::addEdge(size_t tailPointID, size_t headPointID) {
		size_t size = edges.size();
		edges.resize(size + 1);
		edges[size] = new edge(points[tailPointID], points[headPointID]);
	}

	void graph::addFace(size_t p1ID, size_t p2ID, size_t p3ID) {
		size_t size = edges.size();
		edges.resize(size + 3);

		point* p1 = points[p1ID];
		point* p2 = points[p2ID];
		point* p3 = points[p3ID];
		edge* e1 = new edge(p1, p2);
		edge* e2 = new edge(p2, p3);
		edge* e3 = new edge(p3, p1);
		edges[size] = e1;
		edges[size+1] = e2;
		edges[size+2] = e3;
		e1->setPrevEdge(e3);
		e2->setPrevEdge(e1);
		e3->setPrevEdge(e2);
	}
	
	edge* graph::getEdge(size_t index) {
		return edges[index];
	}

	void graph::deleteEdge(size_t index) {
		edge* e = getEdge(index);
		delete e;
		edges.erase(edges.begin() + index);
	}

	void graph::setPairEdge(size_t edgeID, size_t pairEdgeID) {
		edges[edgeID]->setPairEdge(edges[pairEdgeID]);
	}

	void graph::setPrevEdge(size_t edgeID, size_t prevEdgeID) {
		edges[edgeID]->setPrevEdge(edges[prevEdgeID]);
	}

	void graph::resetFlag() {
		for (size_t i = 0; i < edges.size(); i++) {
			edges[i]->setFlag(false);
		}
	}

	void graph::makePair(size_t edgeID1, size_t edgeID2) {
		size_t size = edges.size();
		if (size <= edgeID1 || size <= edgeID2) {
			throw graphException("graph","setPair : edgeID is larger than last index");
		}
		makeEdgePair(edges[edgeID1],edges[edgeID2]);
	}

	//splits the face has edge e and inserts point p in the face.
	void graph::insertPoint(point* p, edge* e, std::stack<edge*>* edgeStack) {

		point* p1 = e->getP1();
		point* p2 = e->getP2();
		point* p3 = e->getP3();

		edge* prev = e->getPrevEdge();
		edge* next = prev->getPrevEdge();

		edge* p_p1, * p2_p, * p_p2;
		edge* p3_p, * p_p3, * p1_p;

		p_p1 = new edge(p, p1);
		p2_p = new edge(p2, p);
		p_p1->setPrevEdge(p2_p);
		p2_p->setPrevEdge(e);
		e->setPrevEdge(p_p1);

		p_p2 = new edge(p, p2);
		p3_p = new edge(p3, p);
		p_p2->setPrevEdge(p3_p);
		p3_p->setPrevEdge(next);
		next->setPrevEdge(p_p2);

		p_p3 = new edge(p, p3);
		p1_p = new edge(p1, p);
		p_p3->setPrevEdge(p1_p);
		p1_p->setPrevEdge(prev);
		prev->setPrevEdge(p_p3);

		makeEdgePair(p_p1, p1_p);
		makeEdgePair(p_p2, p2_p);
		makeEdgePair(p_p3, p3_p);

		size_t size = edges.size();
		edges.resize(size + 6);
		edges[size] = p_p1;
		edges[size + 1] = p2_p;
		edges[size + 2] = p_p2;
		edges[size + 3] = p3_p;
		edges[size + 4] = p_p3;
		edges[size + 5] = p1_p;
		
		if (edgeStack == nullptr) return;
		edgeStack->push(e);
		edgeStack->push(prev);
		edgeStack->push(next);
	}

	//splits edge e and inserts point p
	void graph::insertPoint2(point* p, edge* e, std::stack<edge*>* edgeStack, edge* newpair1, edge* newpair2) {
		point* p1 = e->getP1();
		point* p2 = e->getP2();
		point* p3 = e->getP3();

		edge* prev = e->getPrevEdge();
		edge* next = prev->getPrevEdge();

		edge* p_p3, * p_p2, * p3_p;

		e->setPoint(p1, p);
		p_p3 = new edge(p, p3);
		p_p3->setPrevEdge(e);
		prev->setPrevEdge(p_p3);

		p_p2 = new edge(p, p2);
		p3_p = new edge(p3, p);
		p_p2->setPrevEdge(p3_p);
		p3_p->setPrevEdge(next);
		next->setPrevEdge(p_p2);

		makeEdgePair(p_p3, p3_p);

		size_t size = edges.size();
		edges.resize(size + 3);
		edges[size] = p_p3;
		edges[size + 1] = p_p2;
		edges[size + 2] = p3_p;

		if (edgeStack != nullptr) {
			edgeStack->push(prev);
			edgeStack->push(next);
		}

		if (newpair1 == nullptr) {//splits edge "pair" by point p
			edge* pair = e->getPairEdge();

			if (pair != nullptr) {
				insertPoint2(p, pair, edgeStack, edges[size + 1],e);
			}
		}
		else {//sets pair edge to splited edge
			makeEdgePair(e, newpair1);
			makeEdgePair(p_p2, newpair2);
		}
	}

	//finds a face include point p and inserts the point
	bool graph::findFaceIncludePoint(point* p, edge* e, std::stack<edge*>* edgeStack) {

		//printf("findFaceIncludePoint\n");
		bool inserted = false;

		//check
		point* p1 = e->getP1();
		point* p2 = e->getP2();
		point* p3 = e->getP3();

		if (p1 == nullptr) throw(graphException("graph","findFaceIncludePoint : p1 is nullptr"));
		if (p2 == nullptr) throw(graphException("graph","findFaceIncludePoint : p2 is nullptr"));

		//CP>0: inside, CP=0: online, CP<0: outside
		double CPp1p2 = crossprod2D(p, p1, p2);
		double CPp2p3 = crossprod2D(p, p2, p3);
		double CPp3p1 = crossprod2D(p, p3, p1);

		//fix rounding error
		if (abs(CPp1p2) < 1e-15) { CPp1p2 = 0; }

		if (CPp1p2 >= 0 && CPp2p3 >= 0 && CPp3p1 >= 0) {
			//found the face!
			//inserts point p
			if (CPp1p2 > 0 && CPp2p3 > 0 && CPp3p1 > 0) {
				insertPoint(p, e, edgeStack);
			}
			else if (CPp1p2 == 0) {
				insertPoint2(p, e, edgeStack);
			}
			else if (CPp2p3 == 0) {
				insertPoint2(p, e->getPrevEdge()->getPrevEdge(), edgeStack);
			}
			else {
				insertPoint2(p, e->getPrevEdge(), edgeStack);
			}
			return true;
		}

		//search other faces
		edge* next = e;
		if (CPp2p3 < 0) {
			next = e->getPrevEdge()->getPrevEdge();
		}
		else if (CPp3p1 < 0) {
			next = e->getPrevEdge();
		}
		edge* nextPair = next->getPairEdge();
		if (nextPair == nullptr) return false;

		inserted = inserted || findFaceIncludePoint(p, nextPair, edgeStack);
		return inserted;
	}

	void graph::flipEdge(edge* e, edge* pair, point* p3, point* p4, std::stack<edge*>* edgeStack) {
		edge* prev = e->getPrevEdge();
		edge* next = prev->getPrevEdge();
		edge* pairPrev = pair->getPrevEdge();
		edge* pairNext = pairPrev->getPrevEdge();

		e->setPoint(p3, p4);
		e->setPrevEdge(next);
		next->setPrevEdge(pairPrev);
		pairPrev->setPrevEdge(e);

		pair->setPoint(p4, p3);
		pair->setPrevEdge(pairNext);
		pairNext->setPrevEdge(prev);
		prev->setPrevEdge(pair);

		if (edgeStack == nullptr) return;
		edgeStack->push(pairPrev);
		edgeStack->push(pairNext);
	}

	void graph::getPointsEdgesAsCoords(double* point_coords, double* edge_coords) {
		size_t size = getPointNum();
		if (size < 3) return;

		point* p;
		for (size_t i = 0; i < size; i++) {
			p = points[i];
			point_coords[i * 2] = p->getX();
			point_coords[i * 2 + 1] = p->getY();
		}

		size = getEdgeNum();
		double* coords = new double[4];
		for (size_t i = 0; i < size; i++) {
			edges[i]->getCoord(coords);
			memmove(&edge_coords[i * 4], coords, sizeof(double) * 4);
		}
		delete[] coords;

	}

	void graph::getPoints(double* point_coords) {
		size_t size = getPointNum();
		point* p;

		size_t i3;
		for (size_t i = 0; i < size; i++) {
			p = points[i];
			i3 = i * 3;
			point_coords[i3] = p->getX();
			point_coords[i3 + 1] = p->getY();
			point_coords[i3 + 2] = p->getZ();
		}
	}

}