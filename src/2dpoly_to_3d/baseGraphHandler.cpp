/*
 * File: baseGraphHandler.cpp
 * --------------------
 * This file contains the implementation of graph handler
 * 
 * See baseGraphHandler.hpp for documentation of each member.
 * 
 * Author: Matyalatte
 * Last updated: 2021/09/14
 */

#include "baseGraphHandler.hpp"
#include <typeinfo>

namespace sketch3D {


	//constructor
	baseGraphHandler::baseGraphHandler(graph::graph* graph, const char* classname){
		directedGraph = graph;
		className = classname;
	}

	//private
	void baseGraphHandler::addPointObject(graph::point* p) {
		directedGraph->addPointObject(p);
	}

	graph::point* baseGraphHandler::getPoint(size_t index) {
		return directedGraph->getPoint(index);
	}

	graph::edge* baseGraphHandler::getEdge(size_t index) {
		return directedGraph->getEdge(index);
	}

	void baseGraphHandler::deleteEdgeFlagIsTrue() {
		size_t edge_size = getEdgeNum();
		graph::edge* e;
		for (int i = (int)edge_size - 1; i >= 0; i--) {
			e = getEdge(i);
			if (e->getFlag()) {
				directedGraph->deleteEdge(i);
			}
		}
	}

	void baseGraphHandler::setFlagToFace(graph::edge* e) {
		e->setFlag(true);
		graph::edge* prev = e->getPrevEdge();
		graph::edge* next = prev->getPrevEdge();
		prev->setFlag(true);
		next->setFlag(true);
	}

	void baseGraphHandler::init() {
		directedGraph->init();
	}

	void baseGraphHandler::getPointsEdgesAsCoords(double* point_coords, double* edge_coords) {
		directedGraph->getPointsEdgesAsCoords(point_coords, edge_coords);
	}

	size_t baseGraphHandler::getPointNum() {
		return directedGraph->getPointNum();
	}

	size_t baseGraphHandler::getEdgeNum() {
		return directedGraph->getEdgeNum();
	}

	void baseGraphHandler::checkAllConstraintEdgeExist(size_t pointNum, bool checkPair) {
		bool* exist= new bool[pointNum];
		for (size_t i = 0; i < pointNum; i++) {
			exist[i] = false;
		}
		size_t p1;
		size_t p2;
		graph::edge* e;
		for (size_t i = 0; i < getEdgeNum(); i++) {
			e = getEdge(i);
			e->getPointID(&p1,&p2);
			if ((p1 + 1) % pointNum == p2) {
				exist[p1] = true;
				if (checkPair && e->getPairEdge() != nullptr) {
					throw (graph::graphException(className, "checkAllConstraintEdgeExist: unexpected pair edge detected"));
				}
			}
			if ((p2 + 1) % pointNum == p1) {
				exist[p2] = true;
				if (checkPair && e->getPairEdge() != nullptr) {
					throw (graph::graphException(className, "checkAllConstraintEdgeExist: unexpected pair edge detected"));
				}
			}
		}
		for (size_t i = 0; i < pointNum; i++) {
			if (exist[i] == false) {
				throw (graph::graphException(className, "checkAllConstraintEdgeExist: unexpected edge deletion detected"));
			}
		}
		delete[] exist;
	}

	void baseGraphHandler::checkDuplicatePoint() {
		size_t size = getPointNum();
		graph::point* p1, * p2;
		for (size_t i = 0; i < size-1; i++) {
			p1 = getPoint(i);
			if (p1->getID() != i) printf("point Error!!!");
			for (size_t j = i+1; i < size; i++) {
				p2 = getPoint(j);
				if (p1->getX() == p2->getX() && p1->getY() == p2->getY()) {
					throw graph::graphException(className,"checkDuplicatePoint: duplicated point detected");
				}
			}
		}
	}
	void baseGraphHandler::checkDuplicateEdge() {
		size_t size = getEdgeNum();
		graph::edge* e1, *e2;
		size_t e1_p1, e1_p2, e2_p1, e2_p2;
		for (size_t i = 0; i < size - 1; i++) {
			e1 = getEdge(i);
			e1_p1 = e1->getP1()->getID();
			e1_p2 = e1->getP2()->getID();
			for (size_t j = i + 1; i < size; i++) {
				e2 = getEdge(j);
				e2_p1 = e2->getP1()->getID();
				e2_p2 = e2->getP2()->getID();
				if (e1_p1 == e2_p1 && e1_p2 == e2_p2) {
					throw graph::graphException(className, "checkDuplicateEdge: duplicated edge detected");
				}
			}
		}
	}

	void baseGraphHandler::checkEdgePair() {
		size_t size = getEdgeNum();
		graph::edge* e1, * e2;
		size_t e1_p1, e1_p2, e2_p1, e2_p2;
		for (size_t i = 0; i < size; i++) {
			e1 = getEdge(i);
			e2 = e1->getPairEdge();
			if (e2 != nullptr) {
				e1_p1 = e1->getP1()->getID();
				e1_p2 = e1->getP2()->getID();
				e2_p1 = e2->getP1()->getID();
				e2_p2 = e2->getP2()->getID();
				if (e1_p2 != e2_p1 || e1_p1 != e2_p2) {
					printf("(%d,%d)<->(%d,%d)\n", (int)e1_p1,(int)e1_p2, (int)e2_p1, (int)e2_p2);
					throw graph::graphException(className, "checkEdgePair: unexpected pair edge detected");
				}
			}
		}
	}

	void baseGraphHandler::loadGraph(const char* file_path) {
		init();

		std::string str = fileUtils::read_txt(file_path);
		std::vector<std::string> vec = stringUtils::split(str, '\n');
		std::vector<std::string> typevec = stringUtils::split(vec[0], ':');
		std::vector<std::string> discvec = stringUtils::split(vec[1], ':');
		std::vector<std::string> pointvec = stringUtils::split(vec[2], ':');
		std::vector<std::string> facevec = stringUtils::split(vec[3], ':');
		if (typevec[0] != "type" or typevec[1] != "graph mesh" or discvec[0] != "disc" or pointvec[0] != "point" or facevec[0] != "face") {
			throw("loadGraph: format error");
		}
		printf("loading graph...\nfile name: %s\n", file_path);
		printf("disc: %s\n",discvec[1].c_str());

		size_t size = stringUtils::count(pointvec[1], ',')+1;
		double* point_coords=new double[size];
		stringUtils::string_to_array(pointvec[1], point_coords, size);
		printf("pointNum:%d\n", (int)size/3);
		for (size_t i = 0; i < size / 3; i++) {
			directedGraph->addPoint(point_coords[i*3], point_coords[i*3+1], point_coords[i*3+2]);
		}

		size = stringUtils::count(facevec[1], ',') + 1;
		size_t* face_index = new size_t[size];
		stringUtils::string_to_array(facevec[1], face_index, size);
		printf("faceNum:%d\n", (int)size / 3);
		for (size_t i = 0; i < size / 3; i++) {
			directedGraph->addFace(face_index[i * 3], face_index[i * 3 + 1], face_index[i * 3 + 2]);
		}
		printf("done!");
	}

	void baseGraphHandler::getPoints(double* point_coords) {
		directedGraph->getPoints(point_coords);
	}

	void baseGraphHandler::getFaces(size_t* face_IDs) {
		size_t size = getPointNum();
		if (size < 3) return;
		directedGraph->resetFlag();
		size = getEdgeNum();
		graph::edge* e;
		size_t count = 0;
		size_t count3;
		for (size_t i = 0; i < size; i++) {
			e = getEdge(i);
			if (!e->getFlag()) {
				count3 = count * 3;
				face_IDs[count3] = e->getP1()->getID();
				face_IDs[count3 + 1] = e->getP2()->getID();
				face_IDs[count3 + 2] = e->getP3()->getID();

				count += 1;
				setFlagToFace(e);
			}
		}
	}

	size_t baseGraphHandler::calFaceNum() {
		directedGraph->resetFlag();
		graph::edge* e;
		size_t count = 0;
		for (size_t i = 0; i < getEdgeNum(); i++) {
			e = getEdge(i);
			if (!e->getFlag()) {
				count += 1;
				e->setFlag(true);
				e->getPrevEdge()->setFlag(true);
				e->getPrevEdge()->getPrevEdge()->setFlag(true);
			}
		}
		return count;
	}

	std::string baseGraphHandler::graphToString() {
		size_t pointNum = getPointNum();
		size_t faceNum = calFaceNum();
		double* point_coords = new double[pointNum * 3];
		size_t* faceIDs = new size_t[faceNum * 3];
		getPoints(point_coords);
		getFaces(faceIDs);
		std::string point_str = stringUtils::array_to_string(point_coords, pointNum * 3);
		std::string face_str = stringUtils::array_to_string(faceIDs, faceNum*3);
		delete[] point_coords;
		delete[] faceIDs;
		return "point:" + point_str + "\nface:" + face_str + "\n";
	}

	void baseGraphHandler::printGraph() {
		printf("%s\n", graphToString().c_str());
	}

}