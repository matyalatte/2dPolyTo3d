/*
 * File: cdt.hpp
 * --------------------
 * This file contains the declaration of CDT(constrained delaunay triangulation) solver,
 *
 * See cdt.cpp for implementation of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include "baseGraphHandler.hpp"
#include <queue>
//#include "exception.h"
//#include "point.h"
//#include "edge.h"
//#include "graph.hpp"

namespace sketch3D {
	class CDTsolver : public baseGraphHandler {
	private:
	  //*parameters*

		//graph::graph* directedGraph;// a graph object for CDT
		std::vector<graph::point*> constEdges; // constraints as an array of points {e1_p1, e1_p2, e2_p1, ...}
		int step;// 0:initialized, 1:defined a problem, 2:solved
		int count;

	  //*private methods*

		//gets directedGraph.points[i]
		//graph::point* getPoint(size_t index);

		//gets directedGraph.edges[i]
		//graph::edge* getEdge(size_t index);

		//edge p1->p2 is a constraint edge or not
		bool isConstraint(graph::point* p1, graph::point* p2);

		//checks delaunay constraints
		void checkEdgeFlip(std::stack<graph::edge*>* edgeStack);
		
		//creates a super triangle to graph
		void addSuperTriangle();

		//deletes the super triangle from graph
		void deleteSuperTriangle();

		void checkConstraint(graph::point* p1, graph::point* p2, std::queue<graph::edge*>* edgeQueue);

		void restoreConstraint(graph::point* constP1, graph::point* constP2, graph::edge* e, std::queue<graph::edge*>* edgeQueue, std::stack<graph::edge*>* edgeStack);

	public:
		/*
		* Constructor: CDTsolver
		* Usage: CDTsolver cdt(graph);
		* ---------------------------------
		* Creates an empty CDT solver object.
		* graph: a directed graph object for CDT
		*/
		CDTsolver(graph::graph* graph);

		/*
		* Method: defineProblem
		* Usage: cdt.defineProblem(point_coords, pnum, constraints, constNum);
		* ---------------------------------
		* Defines points by an array of 2D coordinates.
		* And defines constraints by an array of point indices.
		* point_coords = {p1_x, p1_y, p2_x, p2_y, ...}
		* pnum : the number of points. (pnum=len(point_coords)/2)
		* constraints = {e1_p1, e1_p2, e2_p1, e2_p2, ...}
		* constNum: the number of constraints (constNum=len(constraints)/2)
		*/
		void defineProblem(double* point_coords, size_t pointNum, size_t* constraints = NULL, size_t constNum = 0);

		/*
		* Method: solve
		* Usage: cdt.solve();
		* ---------------------------------
		* Solves CDT problem.
		*/
		void solve();

		/*
		* Method: init
		* Usage: cdt.init();
		* ---------------------------------
		* Initializes parameters.
		* All infomation (about points and edges) will be deleted.
		*/
		void init() override;

		/*
		* Method: getPointsEdgesAsCoords
		* Usage: cdt.getPointsEdgesAsCoords(point_coords, edge_coords);
		* ---------------------------------
		* Gets coordinates of points as an array.
		* And gets coordinates of edges as an array.
		* point_coords = {p1_x, p1_y, p2_x, p2_y, ...}
		* edge_coords = {e1_x1, e1_y1, e1_x2, e1_y2, e2_x1, ...}
		*/
		//void getPointsEdgesAsCoords(double* point_coords, double* edge_coords);

		//size_t getPointNum();// directedGraph.points.size()
		//size_t getEdgeNum();// directedGraph.edges.size()
	};
}