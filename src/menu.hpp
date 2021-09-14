/*
 * File: menu.hpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */
#pragma once

#include <GL/glut.h>
#include "2dpoly_to_3d/2dpoly_to_3d.hpp"
#include "2dpoly_to_3d/utils.hpp"
namespace openglHandler {

	//functions for popup menu in glut

	void menu(int item);
	void menuInit();
	void checkUpdateMenu();
	void connectPolyTo3D(sketch3D::poly_to_3D* p_to_3d);

	bool getSetModelFlag();
	void resetSetModelFlag();

	bool getShowNormal();
	bool getShow2DPoly();
	bool getShowModel();
	bool getExitFlag();
	void resetExitFlag();
}