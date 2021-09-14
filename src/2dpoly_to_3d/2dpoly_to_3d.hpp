/*
 * File: 2dpoly_to_3d.hpp
 * --------------------
 * 
 * See 2dpoly_to_3d.cpp for implementation of each member.
 * 
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include "cdt.hpp"
#include "spineEstimator.hpp"
#include "modeler.hpp"

namespace sketch3D {
	class poly_to_3D {
	private:

		graph::graph* directedGraph;
		CDTsolver cdt;
		spineEstimator spineEstimator;
		modeler modeler;

		size_t inputPointNum;// the number of vertices in 2D polygon

		double* points2D;
		double* edges;// edge coordinates of 2D polygon

		size_t point2DNum;//the number of vertices in 2D polygon
		size_t edgeNum;//the number of edges in 2D polygon

		double* points3D;//vertex coordinates of 3D model
		size_t* faces;//face indices of 3D model
		double* pointNormal;//vertex normal

		size_t point3DNum;//the number of vertices in 3D model

		//store 2d graph data
		void store2DPolyData();

		//store 3D model data
		void storeModelData();

	public:
		poly_to_3D(graph::graph* graph);
		~poly_to_3D();
		void init();

		//generate 3D model from 2D polygon
		void genModel(double* point_coords, size_t pointNum);

		//get spine data
		size_t getSpineNum();//get the number of spines
		void getSpinesAsCoords(double* spine_coords);// get spine coordinates
		int getSpineType(size_t i);

		//gets 2D polygon data
		double* getPointerToPoints2D();
		size_t getPoint2DNum();
		double* getPointerToEdges();
		size_t getEdgeNum();

		//gets model datas
		double* getPointerToPoints3D();
		size_t getPoint3DNum();
		size_t* getPointerToFaces();
		size_t getFaceNum();
		double* getPointerToPointNormal();
		
		//export 3D model data as .obj
		bool exportAsObj(WCHAR* FileName);

		//export input polygon data
		bool exportAsStroke(const WCHAR* file_path, const char* desc);
		bool exportAsStroke(const char* file_path, const char* desc);

		//import input polygon and generate 3D model
		bool importStroke(const WCHAR* file_path);

		void loadGraph(const char* file_path);

	};
}