/*
 * File: edge.hpp
 * --------------------
 * This file contains the declaration of half edge.
 *
 * See edge.cpp for implementation of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include "point.hpp"
#include <cstddef>


namespace graph {

	class edge {

	/*half edge p1->p2
	 *
	 *        p3
	 *         *
	 *        /
	 *  prevE/
	 *      / this
	 *  p1 *------->* p2
	 *      <-------
	 *        pairE
	 */

	private:
		point* p1;
		point* p2;
		edge* pairE;
		edge* prevE;
		bool flag;

	public:
	   /*
		* Constructor: edge
		* Usage: edge::edge e(p1,p2);
		* ---------------------------------
		* Creates an half edge object.
		*/
		edge(point* p1_, point* p2_, edge* pairE_ = nullptr, edge* prevE_ = nullptr);

		~edge();

		double len();//gets the length of the edge
		double len_sq();//gets the squared length of the edge
		void setPoint(point* p1_, point* p2_);//sets the points of the edge
		void getPoint(point* p1_, point* p2_);//gets the points of the edge
		void getPointID(size_t* p1_id, size_t* p2_id);//gets point indices of the edge
		point* getP1();//get p1
		point* getP2();//get p2
		point* getP3();// get prevE.p1;
		void getCoord(double* coord_);//gets 2d coordinates of the edge.
		void setPairEdge(edge* e=nullptr);
		edge* getPairEdge();
		void setPrevEdge(edge* e);
		edge* getPrevEdge();
		void setFlag(bool f);
		bool getFlag();
		void disconnectPair();//sets NULL to pairE
		point* getMidPoint(bool calZ=false);//gets the mid point of the edge
		point* getCenterPoint(bool calZ=false);//gets the center point of the face with the edge
		void printIDs();
	};
	void makeEdgePair(edge* e1, edge* e2);

}