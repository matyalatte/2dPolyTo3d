/*
 * File: graph.hpp
 * --------------------
 * This file contains the declaration of directed graph.
 *
 * See graph.cpp for implementation of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include "exception.hpp"
#include "edge.hpp"
#include "utils.hpp"
//#include <vector>
#include <stack>
 //#include "point.h"

namespace graph {

	class graph {

	private:
		//*parameters*
		std::vector<point*> points; // points
		std::vector<edge*> edges; // edges
		double maxX, minX, maxY, minY; // graph area


	public:
		graph();
		~graph();

		void init();

		//get parameters
		double getMaxX();
		double getMinX();
		double getMaxY();
		double getMinY();

		//*point functions*-----

		//get points.size()
		size_t getPointNum();

		//add a point (px,py,pz) to graph
		void addPoint(double px, double py, double pz = 0);

		//add point p to graph
		void addPointObject(point* p);

		//add points to graph
		void addPoints(double* point_coords, size_t pointNum);

		//get points[index]
		point* getPoint(size_t index);

		//points[i]->setID(i) (i=0,1,2,...)
		void resetPointID();
		
		//delete edges[index]
		void deletePoint(size_t index);

		//delete last 'deleteNum' points
		void deleteLastPoint(size_t deleteNum);

		//*edge functions*-----

		//get edges.size()
		size_t getEdgeNum();

		//add an edge (points[tailID]->points[headID])
		void addEdge(size_t tailPointID, size_t headPointID);

		//get edges[index]
		edge* getEdge(size_t index);

		//delete edges[index]
		void deleteEdge(size_t index);

		//add 3 edges (p1->2, p2->p3, p3->1)
		void addFace(size_t p1ID, size_t p2ID, size_t p3ID);

		//edges[edgeID]->setPrevEdge(edges[prevEdgeID])
		void setPrevEdge(size_t edgeID, size_t prevEdgeID);

		//edges[edgeID]->setPairEdge(edges[pairEdgeID])
		void setPairEdge(size_t edgeID, size_t pairEdgeID);
		
		//resets flags of edges
		void resetFlag();

		//makes a pair of edges.
		void makePair(size_t edgeID1, size_t edgeID2);

		//splits the face has edge e and inserts point p in the face.
		void insertPoint(point* p, edge* e, std::stack<edge*>* edgeStack = nullptr);

		//splits edge e and inserts point p
		void insertPoint2(point* p, edge* e, std::stack<edge*>* edgeStack = nullptr, edge* newpair1 = nullptr, edge* newpair2 = nullptr);

		//finds a face includes point p
		bool findFaceIncludePoint(point* p, edge* e, std::stack<edge*>* edgeStack);

		//flip edge e (e=(p1,p2), pair=(p2,p1), e.prevE=(p3,p1), pair.prevE=(p4,p2))
		void flipEdge(edge* e, edge* pair, point* p3, point* p4, std::stack<edge*>* edgeStack = nullptr);

		//get graph data
		void getPointsEdgesAsCoords(double* point_coords, double* edge_coords);
		
		//get points coordinates
		void getPoints(double* point_coords);
	};
}