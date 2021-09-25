/*
 * File: main.cpp
 * --------------------
 * 
 * This program is a demo application of 2Dpoly_to_3D
 * You can generate a 3D model by drawing a 2D polygon
 * 
 * Author: Matyalatte
 * Last updated: 2021/09/25
 */

#include "openglHandler.hpp"
#include "mouseLogger.hpp"
#include "2dpoly_to_3d/2dpoly_to_3d.hpp"
#include "menu.hpp"

//setting----------
const char* VERSION = "0.9.0";

const int WINDOW_W = 860;//initial value of window width
const int WINDOW_H = 860;//initial value of window height

const int LOGGER_POINT_NUM = 300;//maximum number of vertices
const double MIN_DISTANCE = 0.07;//minimum distance of vertices

const float ROT_SPEED = 1;//rotation speed

const bool LOG_STROKE = true;
const char* LOG_DIR = "stroke";
//-----------------

openglHandler::openglHandler GL_handler= openglHandler::openglHandler();

mouseLogger::mouseLogger Mlogger(LOGGER_POINT_NUM, MIN_DISTANCE);

graph::graph directedGraph;

//3D model generator
sketch3D::poly_to_3D polyTo3D(&directedGraph);

//redisplay flag
GLboolean isRedisp = GL_FALSE;

//0:initial state, 1:drawing, 2:show 3D model
int writeMode = 0;

//if true then you can rotate 3D model
bool rotateMode = false;

//window size
int width = WINDOW_W;
int height = WINDOW_H;

//mouse parameters
int mouseX = 0;
int mouseY = 0;
int mouseDx=0, mouseDy=0;
int moveTimer = 0; // >0 then display mouse cursor

//update mouse parameters
void moveMouse(int x, int y) {
	//printf("move");
	if (mouseX == x && mouseY == y) return;
	mouseDx = x - mouseX;
	mouseDy = y - mouseY;

	mouseX = x;
	mouseY = y;
	isRedisp = GL_TRUE;
	moveTimer = 100;
}

void rejectMessage() {
	printf("Input rejected.\n\n");
	writeMode = 0;
}


double* spines;
size_t spineNum;

//send model data to opengl handler and get spine data
void setModel() {
	GL_handler.setModel(
		polyTo3D.getPointerToPoints3D(),
		polyTo3D.getPointerToFaces(),
		polyTo3D.getPointerToPointNormal(),
		polyTo3D.getPoint3DNum(),
		polyTo3D.getFaceNum());
	GL_handler.setModelColor((float)1.0, (float)1.0, (float)1.0);
	GL_handler.setLightPos((float)0.1, 0, (float)-2.0);
	
	GL_handler.resetModelRotation();
	writeMode = 2;
	rotateMode = false;
}

//when push mouse button
void mouse(int button, int state, int x, int y) {
	//printf("mouses");
	isRedisp = GL_TRUE;
	moveMouse(x, y);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {//LMB is pushed
			//start logging
			Mlogger.start(x * 2.0 / width - 1, 1 - y * 2.0 / height);
			if (writeMode != 1) writeMode = 1;
		}
		else {//LMB is released
			if (writeMode == 1) {

				//check stroke length
				bool res = Mlogger.finishStroke();
				if (!res) {
					printf("Stroke is too long.\n");
					rejectMessage();
					return;
				}
				size_t Mlog_pnum = Mlogger.getPnum();
				if (Mlog_pnum < 5) {
					printf("Stroke is too short.\n");
					rejectMessage();
					return;
				}

				//initialize generator
				polyTo3D.init();
				try {

					//generate a 3D model from a 2D polygon
					polyTo3D.genModel(Mlogger.getPoints(), Mlogger.getPnum());

					if (LOG_STROKE) {
						//write stroke data to a file
						std::string file = std::string(LOG_DIR) + "/autolog.stroke";
						polyTo3D.exportAsStroke(file.c_str(), "stroke log");
					}

					//send model data to opengl handler
					setModel();

				}
				catch (graph::graphException e) {
					if (e.getUnexpected()) {
						e.print();
						std::string file = std::string(LOG_DIR) + "/" + stringUtils::getTime() + "_error.stroke";
						polyTo3D.exportAsStroke(file.c_str(), e.getErrorMsg().c_str());
					}
					else {
						printf("Crossed lines detected.\n");
					}
					rejectMessage();
				}
				catch (std::exception e) {
					printf(e.what());
					std::string file = std::string(LOG_DIR) + "/" + stringUtils::getTime() + "_error.stroke";
					polyTo3D.exportAsStroke(file.c_str(), e.what());
					rejectMessage();
				}
				catch (...) {
					printf("unexpected exception\n");
					std::string file = std::string(LOG_DIR) + "/" + stringUtils::getTime() + "_error.stroke";
					polyTo3D.exportAsStroke(file.c_str(), "unexpected error");
					rejectMessage();
				}
			}
		}
		return;
	}
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN && writeMode == 2) {
			rotateMode = true;
		}
		else {
			rotateMode = false;
		}
	}
}

//when move mouse with mouse buttons pushed
void motion(int x, int y) {
	moveMouse(x, y);

	if (writeMode == 1) Mlogger.log(x * 2.0 / width - 1, 1 - y * 2.0 / height);
	if (writeMode == 2 && rotateMode) {
		GL_handler.rotateModel((float)(mouseDy/300.0*ROT_SPEED),(float)(mouseDx/300.0 * ROT_SPEED));
	}
}

//when move mouse without mouse buttons pushed
void passivemotion(int x, int y) {
	moveMouse(x, y);
}

void exit() {
	openglHandler::resetExitFlag();
	int yesno = MessageBox(NULL, L"Are you sure you want to exit?", L"Exit?", MB_YESNO);
	if (yesno == IDNO) {
		return;
	}

	//close window
	GL_handler.exit();
}

size_t show2DMode=4;
bool showSpine = true;
bool showConst = false;
bool darkMode = true;
void redisp(int value) {
	moveTimer = (moveTimer - 1)*(int)(moveTimer>0);
	if (isRedisp || writeMode==2){
		bool pushW = GetKeyState('W') & 0x8000;
		bool pushA = GetKeyState('A') & 0x8000;
		bool pushS = GetKeyState('S') & 0x8000;
		bool pushD = GetKeyState('D') & 0x8000;
		bool pushQ = GetKeyState('Q') & 0x8000;
		bool pushE = GetKeyState('E') & 0x8000;
		if (pushW || pushA || pushS || pushD || pushQ || pushE) {
			float x = (float)((0.0+pushS - pushW) / 20.0 * ROT_SPEED);
			float y = (float)((0.0+pushD - pushA) / 20.0 * ROT_SPEED);
			float z = (float)((0.0+pushE - pushQ) / 20.0 * ROT_SPEED);
			GL_handler.rotateModel(x,y,z);
		}

		if (GetKeyState('R') & 0x8000) {//push R
			GL_handler.resetModelRotation();
		}
		if (GetAsyncKeyState('C') & 1) {
			darkMode = !darkMode;
		}
		if (GetAsyncKeyState('T') & 1) {
			GL_handler.takeScreenShot();
		}
		if (writeMode == 2) {
			if (GetAsyncKeyState('Z') & 1) {
				show2DMode = (show2DMode + 1) % 5;
			}
			if (GetAsyncKeyState('X') & 1) {
				showSpine = !showSpine;
			}
			if (GetAsyncKeyState('V') & 1) {
				showConst = !showConst;
			}
			
		}


		glutPostRedisplay();
		isRedisp = GL_FALSE;
	};
	if (GetKeyState(0x1B) & 0x8000) {//push esc key
		exit();
	}
	glutTimerFunc(10, redisp, 0);
}

//draw window
void disp(void) {
	openglHandler::checkUpdateMenu();
	if (openglHandler::getSetModelFlag()) {
		setModel();
		openglHandler::resetSetModelFlag();
	}
	
	//get window size
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	//clear buffer
	glClearColor(!darkMode, !darkMode, !darkMode, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (writeMode == 1) {
		//draw points
		glLineWidth(2);
		GL_handler.draw_by_array(GL_LINE_STRIP, 
			Mlogger.getPoints(),
			(GLsizei)Mlogger.getPnum(),
			0, 0, 1);
	}

	if (writeMode == 2) {
		bool showModel = openglHandler::getShowModel();
		bool show2DPoly = openglHandler::getShow2DPoly();
		bool showNormal = openglHandler::getShowNormal();
		if (showModel) {
			//draw model
			GL_handler.draw();
		}

		if (show2DPoly) {
			

			//draw edges
			glLineWidth(2);
			GL_handler.draw_by_array(GL_LINES, 
				polyTo3D.getPointerToEdges(show2DMode),
				polyTo3D.getEdgeNum(show2DMode) * 2,
				0, 1, 0);

			if (showConst) {
				GL_handler.draw_by_array(GL_LINES,
					polyTo3D.getPointerToEdges(0),
					polyTo3D.getEdgeNum(0) * 2,
					1, 0, 0);
			}

			//draw spines
			if (show2DMode > 1 && showSpine) {
				glLineWidth(3);
				GL_handler.draw_by_array(GL_LINES,
					polyTo3D.getPointerToSpines(show2DMode - 2 - (show2DMode==4)),
					polyTo3D.getSpineNum(show2DMode - 2 - (show2DMode==4)) * 2,
					1, 0, 0);
			}

			//draw points
			glPointSize(8);
			GL_handler.draw_by_array(GL_POINTS,
				polyTo3D.getPointerToPoints2D(show2DMode),
				polyTo3D.getPoint2DNum(show2DMode*(show2DMode!=3)),
				0, 0, darkMode);
		}

		if (showNormal) {
			//draw normals
			GL_handler.draw_normalLines(1, 0, 0);
		}

		if (!showModel && !show2DPoly && !showNormal) {
			GL_handler.drawString("Don't you want to see anything...?", (float)-0.3, (float)0);
		}
	}
	GL_handler.drawString("LMB:draw", (float)-0.98, (float)0.95);
	GL_handler.drawString("RMB:menu", (float)-0.98, (float)0.90);
	GL_handler.drawString("MMB:rotate", (float)-0.98, (float)0.85);

	//draw mouse cursor
	if (moveTimer > 0) {
		GL_handler.draw_point((mouseX - width / 2.0) / width * 2, -(mouseY - height / 2.0) / height * 2, 3, 0, 1, 0);
	};

	//update window
	glutSwapBuffers();

	if (openglHandler::getExitFlag()) {
		exit();
	}
}


sketch3D::CDTsolver cdt(&directedGraph);
double test_p[49*2];
size_t test_p_num=49;
size_t constraint[4] = { 8,26,31,36 };
size_t constNum = 2;
bool useconst = true;
void test() {
	std::srand(0);

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			test_p[(x * 7 + y)*2] = (x - 3.0 + (std::rand() % 100 - 50.0) * 0.01) * 0.25;
			test_p[(x * 7 + y)*2+1] = (y - 3.0 + (std::rand() % 100 - 50.0) * 0.01) * 0.25;
		}
	}
	cdt.init();
	if (useconst) {
		cdt.defineProblem(test_p, test_p_num, constraint, constNum);
	}
	else {
		cdt.defineProblem(test_p, test_p_num, nullptr, 0);
	}
	cdt.solve();
}

void testdisp() {
	//clear buffer
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	size_t edgeNum = cdt.getEdgeNum();
	double* edges = new double[edgeNum*4];
	cdt.getPointsEdgesAsCoords(test_p, edges);
	glLineWidth(2);
	/*
	GL_handler.draw_by_array(GL_LINES,
		edges,
		edgeNum * 2,
		0, 1, 0);*/
	if (useconst) {
		glLineWidth(3);
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		graph::point* p;
		for (size_t i = 0; i < constNum*2; i++) {
			p = directedGraph.getPoint(constraint[i]);
			glVertex2d(p->getX(), p->getY());
		}
		glEnd();
	}
	glPointSize(8);
	GL_handler.draw_by_array(GL_POINTS, test_p, test_p_num, 0,0,0);
	glFinish();
	delete[] edges;
}

int main(int argc, char** argv) {
	//make log directory
	if (fileUtils::mkdir(LOG_DIR)<0) { printf("ERROR: mkdir fail (%s)\n", LOG_DIR); };
	
	//initialize
	printf("2Dpoly_to_3D version:%s\n", VERSION);
	char window_name[] = "2Dpoly_to_3D";

	if (false) {
		glutInit(&argc, argv);
		glutInitWindowPosition(100, 50);
		glutInitWindowSize(width, height);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

		glutCreateWindow(window_name);
		
		test();
		glutDisplayFunc(testdisp);
	}
	else{
		GL_handler.init(argc, argv, window_name, width, height);
		openglHandler::connectPolyTo3D(&polyTo3D);
		glutDisplayFunc(disp);
		glutMouseFunc(mouse);
		glutPassiveMotionFunc(passivemotion);
		glutMotionFunc(motion);
		glutTimerFunc(10, redisp, 0);
		openglHandler::menuInit();
	}
	//main loop
	glutMainLoop();

	return 0;
}
