/*
 * File: spineEstimator.hpp
 * --------------------
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#pragma once

#include "baseGraphHandler.hpp"
#include "spine.hpp"
//#include "exception.h"
//#include "point.hpp"
//#include "edge.hpp"
//#include "graph.hpp"

namespace sketch3D {

	class spineEstimator : public baseGraphHandler {
	private:
		std::vector<spine*> spines;
		size_t pointNum;

		//*spine functions*
		void addSpine(graph::edge* e1=nullptr,graph::edge* e2=nullptr,graph::point* p1=nullptr, graph::point*p2=nullptr);
		void resetSpineFlag();
		void deleteSpineFlagIsTrue();

		//generate spines
		void genSpineFromChordalAxisRec(graph::edge* e, graph::point* p = nullptr, spine* s = nullptr);
		
		//generate fan triangles
		void genFanTriangle(graph::point* p1, graph::point* p2, graph::point* center, size_t pnum, graph::edge* se_prev, graph::edge* se_next);

		//recursive function to generate fan triangles
		void genFanTriangleRec(graph::edge* e, std::vector<graph::point*>& pvec, spine* s, spine* preS=nullptr, bool end=false);

		//find terminal spine to generate fan triangles
		void searchSpineToGenFan();

		//delete unused points
		void deleteUnusedPoint();

		
	public:
		spineEstimator(graph::graph* graph);
		~spineEstimator();

		void init() override;

		//delete external edges
		void deleteExternalEdge();

		void genSpineFromChordalAxis();

		//estimate spines and split faces by spines
		void cutSpine();
		
		//split faces by spine
		void splitFaceBySpine();

		//get the coordinates of spines
		void getSpinesAsCoords(double* spine_coords);

		//get the number of spines
		size_t getSpineNum();

		//get spines[spineNum-1]
		spine* getLastSpine();

		//get spines[i].type;
		int getSpineType(size_t i);

		
	};


}