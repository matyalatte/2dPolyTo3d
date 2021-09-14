/*
 * File: baseGraphHandler.hpp
 * --------------------
 * This file contains the declaration of graph handler.
 * 
 * See baseGraphHandler.cpp for implementation of each member.
 * 
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once
#include "graph.hpp"

namespace sketch3D {
	class baseGraphHandler {
	protected:
		//*parameters*

		graph::graph* directedGraph;// a graph object for CDT

		const char* className;

		//*private methods*

		//add point p to graph
		void addPointObject(graph::point* p);

		//gets directedGraph.points[i]
		graph::point* getPoint(size_t index);

		//gets directedGraph.edges[i]
		graph::edge* getEdge(size_t index);

		//delete edge flag is true
		void deleteEdgeFlagIsTrue();

		//set flag=true to e, e.prevE and e.prevE.prevE
		void setFlagToFace(graph::edge* e);
		size_t calFaceNum();


	public:
		baseGraphHandler(graph::graph* graph, const char* classname);

		virtual void init();

		//get coordinates of points and edges
		void getPointsEdgesAsCoords(double* point_coords, double* edge_coords);

		size_t getPointNum();// directedGraph.points.size()
		size_t getEdgeNum();// directedGraph.edges.size()

		//check graph data to avoid errors
		void checkAllConstraintEdgeExist(size_t pointNum, bool checkPair=false);
		void checkDuplicatePoint();
		void checkDuplicateEdge();
		void checkEdgePair();

		//load graph from text file
		void loadGraph(const char* file_path);

		//get coordinates of points
		void getPoints(double* point_coords);

		//get faces as point indices
		void getFaces(size_t* face_IDs);

		//get graph data as string
		std::string graphToString();

		//print graphToString();
		void printGraph();
	};
}