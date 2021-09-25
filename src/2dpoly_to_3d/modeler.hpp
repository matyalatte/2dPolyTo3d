/*
 * File: modeler.hpp
 * --------------------
 * modeler generates 3d model data from 2d graph
 *
 * See modeler.cpp for implementation of each member.
 *
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#pragma once

#include "baseGraphHandler.hpp"
//#include "exception.h"
//#include "point.hpp"
//#include "edge.hpp"
//#include "graph.hpp"
#define _USE_MATH_DEFINES
#include <math.h>


namespace sketch3D {

	class modeler : public baseGraphHandler {
	private:
		size_t faceNum;
		std::vector<size_t> faces;
		void calZ(size_t outer_pnum);
		void mirrorZ(size_t outer_pnum);

	public:
		modeler(graph::graph* graph);
		void smoothing(size_t outer_pnum);

		void graphTo3D(size_t outer_pnum);
		void getPointsFacesNormals(double* point_coords, size_t* face_IDs, double* normals);
		size_t getFaceNum();

	};


}