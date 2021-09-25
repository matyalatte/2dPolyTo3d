/*
 * File: 2dpoly_to_3d.cpp
 * --------------------
 * 
 * See 2dpoly_to_3d.hpp for documentation of each member.
 * 
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#include "2dpoly_to_3d.hpp"

namespace sketch3D {
	poly_to_3D::poly_to_3D(graph::graph* graph) :
		directedGraph(graph),
		cdt(directedGraph),
		spineEstimator(directedGraph),
		modeler(directedGraph),
		point3DNum(0),
		points3D(),faces(),
		pointNormal(),
		points2D(), edges(),
		point2DNum(), edgeNum(),inputPointNum(0),
		spines(),spineNum()
	{
	}

	poly_to_3D::~poly_to_3D() {
		delete[] points2D[0];
		delete[] points2D[1];
		delete[] points2D[2];
		delete[] points2D[3];
		delete[] points2D[4];
		delete[] edges[0];
		delete[] edges[1];
		delete[] edges[2];
		delete[] edges[3];
		delete[] edges[4];
		delete[] spines[0];
		delete[] spines[1];

		delete[] points3D;
		delete[] pointNormal;
		delete[] faces;
	}

	void poly_to_3D::init() {
		directedGraph->init();
		cdt.init();
		spineEstimator.init();
		point3DNum = 0;
		inputPointNum = 0;
	}

	//generate 3D model from 2D polygon
	void poly_to_3D::genModel(double* point_coords, size_t pointNum) {
		if (pointNum < 5) {
			throw(graph::graphException("poly_to_3D", "genModel: The number of points should be 5 or more.", false));
		}
		init();
		inputPointNum = pointNum;
		delete[] points2D[0];
		points2D[0] = new double[inputPointNum * 2];
		memmove(points2D[0], point_coords, sizeof(double) * inputPointNum * 2);
		size_t* constraints = new size_t[pointNum * 2];
		for (size_t i = 0; i < pointNum; i++) {
			constraints[i * 2] = i;
			constraints[i * 2 + 1] = (i < pointNum - 1) ? i + 1 : 0;
		}
		cdt.defineProblem(point_coords, pointNum, constraints, pointNum);
		store2DPolyData(0);
		cdt.solve();
		//cdt.checkAllConstraintEdgeExist(cdt.getPointNum());
		store2DPolyData(1);
		delete[] constraints;
		spineEstimator.init();
		spineEstimator.deleteExternalEdge();
		store2DPolyData(2);
		spineEstimator.genSpineFromChordalAxis();
		storeSpineData(0);
		spineEstimator.cutSpine();
		storeSpineData(1);
		store2DPolyData(3);

		spineEstimator.splitFaceBySpine();
		//modeler.checkEdgePair();
		
		store2DPolyData(4);

		modeler.smoothing(pointNum);
		//store2DPolyData(4);

		modeler.graphTo3D(pointNum);
		//modeler.checkDuplicatePoint();
		//modeler.checkDuplicateEdge();
		storeModelData();
	}

	//get spine data
	void poly_to_3D::storeSpineData(size_t id) {
		delete[] spines[id];
		spineNum[id] = spineEstimator.getSpineNum();
		spines[id] = new double[spineNum[id] * 4];
		spineEstimator.getSpinesAsCoords(spines[id]);
	}

	size_t poly_to_3D::getSpineNum(size_t id) {
		return spineNum[id];
	}

	double* poly_to_3D::getPointerToSpines(size_t id) {
		return spines[id];
	}


	//get 2D polygon data

	void poly_to_3D::store2DPolyData(size_t id) {
		point2DNum[id] = directedGraph->getPointNum();
		edgeNum[id]= directedGraph->getEdgeNum();

		delete[] points2D[id];
		delete[] edges[id];

		points2D[id] = new double[point2DNum[id] * 2];
		edges[id] = new double[edgeNum[id] * 4];

		directedGraph->getPointsEdgesAsCoords(points2D[id], edges[id]);
	}

	double* poly_to_3D::getPointerToPoints2D(size_t id) {
		return points2D[id];
	}
	size_t poly_to_3D::getPoint2DNum(size_t id) {
		return point2DNum[id];
	}
	double* poly_to_3D::getPointerToEdges(size_t id) {
		return edges[id];
	}
	size_t poly_to_3D::getEdgeNum(size_t id) {
		return edgeNum[id];
	}

	//get 3D model data

	void poly_to_3D::storeModelData() {
		size_t faceNum = getFaceNum();
		point3DNum = directedGraph->getPointNum();
		delete[] points3D;
		delete[] pointNormal;
		delete[] faces;
		points3D = new double[point3DNum * 3];
		pointNormal = new double[point3DNum * 3];
		faces = new size_t[faceNum * 3];
		modeler.getPointsFacesNormals(points3D, faces, pointNormal);
	}

	double* poly_to_3D::getPointerToPoints3D() {
		return points3D;
	}
	size_t poly_to_3D::getPoint3DNum() {
		return point3DNum;
	}
	size_t* poly_to_3D::getPointerToFaces() {
		return faces;
	}
	size_t poly_to_3D::getFaceNum() {
		return modeler.getFaceNum();
	}

	double* poly_to_3D::getPointerToPointNormal() {
		return pointNormal;
	}

	//

	

	
	bool poly_to_3D::exportAsObj(WCHAR* FileName) {
		if (point3DNum == 0) {
			return false;
		}
		std::string str = "#vertices\n";
		for (size_t i = 0; i < point3DNum; i++) {
			str += "v " + std::to_string(points3D[i * 3]) + " ";
			str += std::to_string(points3D[i * 3 + 1]) + " ";
			str += std::to_string(points3D[i * 3 + 2]) + "\n";
		}
		str += "\n#faces\n";
		size_t faceNum = getFaceNum();
		for (size_t i = 0; i < faceNum; i++) {
			str += "f " + std::to_string(faces[i * 3]+1) + " ";
			str += std::to_string(faces[i * 3 + 1]+1) + " ";
			str += std::to_string(faces[i * 3 + 2]+1) + "\n";
		}

		return fileUtils::write_txt(FileName, str);
	}

	bool poly_to_3D::exportAsStroke(const WCHAR* file_path, const char* desc) {
		if (inputPointNum != 0) {
			std::string str(desc);
			str = "type:stroke\ndesc:" + str + "\n"
				+ stringUtils::array_to_string(points2D[0], inputPointNum * 2, 8);
			fileUtils::write_txt(file_path, str);
			return true;
		}
		return false;
	}
	bool poly_to_3D::exportAsStroke(const char* file_path, const char* desc) {
		if (inputPointNum != 0) {
			std::string str(desc);
			str = "type:stroke\ndesc:" + str + "\n" 
				+ stringUtils::array_to_string(points2D[0], inputPointNum*2, 8);
			fileUtils::write_txt(file_path, str);
			return true;
		}
		return false;
	}

	bool poly_to_3D::importStroke(const WCHAR* file_path) {
		
		std::string str = fileUtils::read_txt(file_path);
		if (str == "") {
			throw(graph::graphException("poly_to_3D","importStroke: Failed to read a file.",false));
			return false;
		}
		std::vector<std::string> vec = stringUtils::split(str, '\n');
		if (vec.size() < 3) {
			throw(graph::graphException("poly_to_3D", "importStroke: File format error detected.", false));
			return false;
		}

		std::vector<std::string> desc = stringUtils::split(vec[1], ':');
		if (vec[0] != "type:stroke" || desc[0] != "desc") {
			throw(graph::graphException("poly_to_3D", "importStroke: File format error detected.", false));
			return false;
		}

		std::string data;
		for (size_t i = 2; i < vec.size(); i++) {
			if (vec[i].substr(0,1) == "#") {
				continue;
			}
			data=data+vec[i];
		}
		size_t size = stringUtils::count(data, ',') + 1;
		if (size<10) {
			throw(graph::graphException("poly_to_3D", "importStroke: Stroke is too short.", false));
			return false;
		}
		if (size % 2 == 1) {
			throw(graph::graphException("poly_to_3D", "importStroke: The size of Data array should be even.", false));
			return false;
		}
		double* point_coords = new double[size];
		stringUtils::string_to_array(data, point_coords, size);
		//printf("%s\n",stringUtils::array_to_string(point_coords, size).c_str());
		
		genModel(point_coords, size/2);
		delete[] point_coords;
		return true;
	}

	void poly_to_3D::loadGraph(const char* file_path) {
		modeler.loadGraph(file_path);
	}

}