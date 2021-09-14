/*
 * File: cdt.cpp
 * --------------------
 * This file contains the implementation of CDT(constrained delaunay triangulation) solver,
 *
 * See cdt.hpp for documentation of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#include "cdt.hpp"
#include <stdio.h>

namespace sketch3D {
	

	//constructor
	CDTsolver::CDTsolver(graph::graph* graph)
		: baseGraphHandler(graph, "CDTsolver"), constEdges(), step(0), count(0) {}

	//private

	bool CDTsolver::isConstraint(graph::point* p1, graph::point* p2) {
		graph::point* constP;
		graph::point* constP2;
		bool flag = false;
		for (size_t i = 0; i < constEdges.size()/2; i++) {
			constP = constEdges[i * 2];
			constP2 = constEdges[i * 2 + 1];

			if ((p1 == constP && p2 == constP2) || (p2 == constP && p1 == constP2)) {
				flag = true;
				break;
			}
		}
		return flag;
	}

	void CDTsolver::checkEdgeFlip(std::stack<graph::edge*>* edgeStack) {
		graph::edge* e;
		graph::edge* pair;
		graph::point* p1;
		graph::point* p2;
		graph::point* p3;
		graph::point* p4;

		while (!edgeStack->empty()) {
			e = edgeStack->top();
			edgeStack->pop();
			pair = e->getPairEdge();
			if (pair == nullptr) continue;

			p1 = e->getP1();
			p2 = e->getP2();

			if (isConstraint(p1, p2)) continue;

			p3 = e->getP3();
			p4 = pair->getP3();

			if (isConstraint(p3, p4) && isConvexSquare(p1, p2, p3, p4)) {
				directedGraph->flipEdge(e, pair, p3, p4, edgeStack);
				continue;
			}

			//if det>0 then p4 is in a circumscribed circle
			double det = crossprod2D(p1, p2, p4) * squaredDistance(p3, p4)
				+ crossprod2D(p3, p1, p4) * squaredDistance(p2, p4)
				+ crossprod2D(p2, p3, p4) * squaredDistance(p1, p4);
			if (det > 0) directedGraph->flipEdge(e, pair, p3, p4, edgeStack);
		}
	}

	void CDTsolver::addSuperTriangle() {
		double maxX = directedGraph->getMaxX();
		double minX = directedGraph->getMinX();
		double maxY = directedGraph->getMaxY();
		double minY = directedGraph->getMinY();

		double w = maxX - minX;
		double h = maxY - minY;
		size_t pnum = getPointNum();

		directedGraph->addPoint(minX - w * 0.1, minY + h * 2.1);
		directedGraph->addPoint(minX - w * 0.1, minY - h * 0.1);
		directedGraph->addPoint(minX + w * 2.1, minY - h * 0.1);

		directedGraph->addEdge(pnum, pnum + 1);
		directedGraph->addEdge(pnum + 1, pnum + 2);
		directedGraph->addEdge(pnum + 2, pnum);
		directedGraph->setPrevEdge(0, 2);
		directedGraph->setPrevEdge(1, 0);
		directedGraph->setPrevEdge(2, 1);
	}


	void CDTsolver::deleteSuperTriangle() {

		std::stack<size_t> deteleEdgeID;
		bool delete_flag;
		size_t p1_id = 0, p2_id = 0;
		size_t edge_size = getEdgeNum();
		size_t point_size = getPointNum();
		graph::edge* e;

		directedGraph->resetFlag();
		for (size_t i = 0; i < edge_size; i++) {
			e = getEdge(i);
			delete_flag = e->getFlag();
			if (delete_flag) continue;
			e->getPointID(&p1_id, &p2_id);
			if (p1_id >= point_size - 3 || p2_id >= point_size - 3) {
				setFlagToFace(e);
			}
		}

		deleteEdgeFlagIsTrue();

		directedGraph->deleteLastPoint(3);
	}

	void CDTsolver::checkConstraint(graph::point* p1, graph::point* p2, std::queue<graph::edge*>* edgeQueue) {
		graph::point* p3;
		graph::point* p4;
		graph::edge* e;

		for (size_t i = 0; i < getEdgeNum(); i++) {
			e = getEdge(i);
			p3 = e->getP1();
			p4 = e->getP2();
			if (p3->getID() > p4->getID()) continue;

			if (isCross(p1,p2,p3,p4)) edgeQueue->push(e);
		}
	}
	
	void CDTsolver::restoreConstraint(graph::point* constP1, graph::point* constP2, graph::edge* e, std::queue<graph::edge*>* edgeQueue, std::stack<graph::edge*>* edgeStack) {
		graph::edge* pair = e->getPairEdge();
		if (pair == nullptr) return;

		graph::point* p1 = e->getP1();
		graph::point* p2 = e->getP2();

		if (isConstraint(p1, p2)) return;

		graph::point* p3 = e->getP3();
		graph::point* p4 = pair->getP3();

		if (isConvexSquare(p1, p2, p3, p4)) {
			directedGraph->flipEdge(e, pair, p3, p4);
			if (isCross(constP1, constP2, p3, p4)) {
				edgeQueue->push(e);
			}
			else { edgeStack->push(e); count = 0; }
		}
		else {
			edgeQueue->push(e);
		}
	}

	//public
	void CDTsolver::defineProblem(double* point_coords, size_t pointNum, size_t* constraints, size_t constNum) {
		if (step != 1) throw graph::graphException("CDTsolver", "defineProblem : run 'init()' before defining a problem");
		step = 2;

		directedGraph->addPoints(point_coords, pointNum);

		if (constraints == nullptr) return;

		//checks constraints
		graph::point* p1, * p2, * p3, * p4;
		for (size_t i = 0; i < constNum - 1; i++) {
			p1 = getPoint(constraints[i * 2]);
			p2 = getPoint(constraints[i * 2 + 1]);
			for (size_t j = i + 1; j < constNum; j++) {
				p3 = getPoint(constraints[j * 2]);
				p4 = getPoint(constraints[j * 2 + 1]);
				if (isCross(p1, p2, p3, p4)) {
					step = 1;
					throw(graph::graphException("CDTsolver", "defineProblem : invalid constraints", false));
				}
			}
		}

		//defines constraints
		constEdges.resize(constNum * 2);
		for (size_t i = 0; i < constNum * 2; i++) {
			constEdges[i] = getPoint(constraints[i]);
		}

	}

	void CDTsolver::solve() {

		if (step != 2) throw graph::graphException("CDTsolver","solve : define a problem with 'defineProblem()' before solving");
		step = 3;

		addSuperTriangle();

		bool inserted;
		std::stack<graph::edge*>* edgeStack = new std::stack<graph::edge*>();
		graph::edge* top;

		//insert points
		for (size_t i = 0; i < getPointNum()-3; i++) {
			//insert a point
			inserted = directedGraph->findFaceIncludePoint(getPoint(i), getEdge(getEdgeNum() - 1), edgeStack = edgeStack);
			if (!inserted) throw graph::graphException("CDTsolver","solve: failed with point insertion");

			//flip edges to meet delaunay properties
			checkEdgeFlip(edgeStack);					
		}

		std::queue<graph::edge*>* edgeQueue = new std::queue<graph::edge*>();
		graph::point* p1, * p2;

		//flip edges to meet constraints
		for (size_t i = 0; i < constEdges.size() / 2; i++) {
			p1 = constEdges[i * 2];
			p2 = constEdges[i * 2 + 1];

			//get edges cross to edge p1->p2
			checkConstraint(p1, p2, edgeQueue);

			//flip edges to restore edge p1->p2
			while (!edgeQueue->empty()) {
				top = edgeQueue->front();
				edgeQueue->pop();
				restoreConstraint(p1,p2,top, edgeQueue, edgeStack);
			}

			//flip edges to meet delaunay properties
			checkEdgeFlip(edgeStack);
		}
		delete edgeStack;
		delete edgeQueue;

		deleteSuperTriangle();
	}

	void CDTsolver::init() {
		constEdges.resize(0);
		constEdges.shrink_to_fit();
		step = 1;
		count = 0;
	}
}

