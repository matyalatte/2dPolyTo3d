/*
 * File: modeler.cpp
 * --------------------
 * modeler generates 3d model data from 2d graph
 *
 * See modeler.hpp for documentation of each member.
 *
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#include "modeler.hpp"

namespace sketch3D {

	modeler::modeler(graph::graph* graph)
		:baseGraphHandler(graph, "modeler"), 
		faceNum(), faces() {}


	size_t modeler::getFaceNum() {
		return faceNum;
	}

	graph::point* obalMid(graph::point* p1, graph::point* p2) {
		graph::point* pa, * pb, *dummy;
		pa = p1;
		pb = p2;
		if (p1->getZ() != 0) {
			dummy = pa;
			pa = pb;
			pb = dummy;
		}
		double pax = pa->getX();
		double pay = pa->getY();
		double pbx = pb->getX();
		double pby = pb->getY();
		double vecx = pax - pbx;
		double vecy = pay - pby;
		double b = pb->getZ();
		double sin_half_pi = sin(M_PI / 4);

		return new graph::point(pbx+vecx*sin_half_pi, pby+vecy*sin_half_pi, b*sin_half_pi);
	}

	void modeler::calZ(size_t outer_pnum) {
		graph::edge* e;
		graph::point* p1 = nullptr, * p2 = nullptr, * mid;
		size_t p1id, p2id;
		size_t edgeNum = getEdgeNum();
		size_t pointNum = getPointNum();
		size_t spinePointNum = pointNum - outer_pnum;
		double* z = new double[spinePointNum];
		int* count = new int[spinePointNum];
		for (size_t i = 0; i < spinePointNum; i++) {
			z[i] = 0.0;
			count[i] = 0;
		}

		for (size_t i = 0; i < edgeNum; i++) {
			e = getEdge(i);
			p1 = e->getP1();
			p2 = e->getP2();
			p1id = p1->getID();
			p2id = p2->getID();
			if (p1id >= outer_pnum && p2id < outer_pnum) {
				z[p1id - outer_pnum] += e->len();
				count[p1id - outer_pnum] += 1;
			}
		}
		for (size_t i = 0; i < spinePointNum; i++) {
			p1 = getPoint(i+outer_pnum);
			p1->setZ(z[i]/(double)count[i]);
		}

		delete[] z;
		delete[] count;
	}

	void modeler::smoothing(size_t outer_pnum) {
		calZ(outer_pnum);
		size_t point_size = getPointNum();
		graph::edge* e;
		graph::point* p1=nullptr, * p2= nullptr, * mid;
		size_t p1id, p2id;
		size_t edgeNum = getEdgeNum();
		for (size_t i = 0; i < edgeNum; i++) {
			e = getEdge(i);
			p1 = e->getP1();
			p2 = e->getP2();
			p1id = p1->getID();
			p2id = p2->getID();
			if (p1id >= outer_pnum && p2id < outer_pnum ) {
				mid = obalMid(p1,p2);
				addPointObject(mid);
				directedGraph->insertPoint2(mid, e);
			}
			
		}
	}

	void modeler::mirrorZ(size_t outer_pnum) {
		size_t point_size = getPointNum();
		for (size_t i = 0; i < point_size - outer_pnum; i++) {
			graph::point* p = getPoint(outer_pnum+i);
			addPointObject(new graph::point(p->getX(), p->getY(), -p->getZ(), point_size + i));
		}
		size_t p1,p2,p3;
		size_t i3;
		faces.resize(faceNum*6);
		for (size_t i = 0; i < faceNum;i++) {
			i3 = i * 3;
			p1 = faces[i3];
			p2 = faces[i3 + 1];
			p3 = faces[i3 + 2];

			if (p1 >= outer_pnum) {
				p1 += point_size - outer_pnum;
			}
			if (p2 >= outer_pnum) {
				p2 += point_size - outer_pnum;
			}
			if (p3 >= outer_pnum) {
				p3 += point_size - outer_pnum;
			}
			faces[faceNum * 3 + i3] = p2;
			faces[faceNum * 3 + i3+1] = p1;
			faces[faceNum * 3 + i3 + 2] = p3;
			

		}
		faceNum = faceNum * 2;
	
	}


	void modeler::getPointsFacesNormals(double* point_coords, size_t* face_IDs, double* normals) {
		size_t size = getPointNum();
		if (size < 3) return;

		getPoints(point_coords);

		for (size_t i = 0; i < size * 3; i++) {
			normals[i] = 0;
		}
		graph::point *p1, *p2, *p3;
		size_t p1id, p2id, p3id;
		double normal[3];

		for (size_t i = 0; i < faceNum * 3; i++) {
			face_IDs[i] = faces[i];
		}

		
		//face_IDs = &faces[0];
		size_t i3;
		for (size_t i = 0; i < faceNum; i++) {
			i3 = i * 3;
			p1id = faces[i3];
			p2id = faces[i3 + 1];
			p3id = faces[i3 + 2];
			

			p1 = getPoint(p1id);
			p2 = getPoint(p2id);
			p3 = getPoint(p3id);

			calNormal(p1, p2, p3, normal);
			normals[p1id * 3] += normal[0];
			normals[p1id * 3 + 1] += normal[1];
			normals[p1id * 3 + 2] += normal[2];
			normals[p2id * 3] += normal[0];
			normals[p2id * 3 + 1] += normal[1];
			normals[p2id * 3 + 2] += normal[2];
			normals[p3id * 3] += normal[0];
			normals[p3id * 3 + 1] += normal[1];
			normals[p3id * 3 + 2] += normal[2];

		}
		/*
		for (size_t i = 0; i < size/2; i++) {
			printf("(%f,%f,%f)\n", normals[i*3],normals[i*3+1],normals[i*3+2]);
		}
		*/
	}


	void modeler::graphTo3D(size_t outer_pnum) {
		faceNum = calFaceNum();
		size_t* face_IDs = new size_t[faceNum * 3];
		getFaces(face_IDs);
		faces.resize(faceNum * 3);
		for (size_t i = 0; i < faceNum * 3; i++) {
			faces[i] = face_IDs[i];
		}
		//faces = std::vector<size_t>(face_IDs, face_IDs+(int)(faceNum*3));
		delete[] face_IDs;

		mirrorZ(outer_pnum);
	}
}
