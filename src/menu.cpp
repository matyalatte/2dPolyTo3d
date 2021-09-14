/*
 * File: menu.cpp
 * --------------------
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */
#include "menu.hpp"
namespace openglHandler {
	
	enum class MENU_TYPE
	{
		EXPORT_AS_OBJ,
		EXPORT_AS_STROKE,
		IMPORT_STROKE,
		SHOW_NORMAL,
		SHOW_2DPOLY,
		SHOW_MODEL,
		EXIT
	};

	bool show_normal;//show normal vector or not
	bool getShowNormal() {
		return show_normal;
	}

	bool show_2dpoly;//show 2d polygon or not
	bool getShow2DPoly() {
		return show_2dpoly;
	}

	bool show_model;//show 3d model or not
	bool getShowModel() {
		return show_model;
	}

	//update popup menu
	void updateMenu() {
		if (show_normal) {
			glutChangeToMenuEntry((int)MENU_TYPE::SHOW_NORMAL+1, "Hide Normal", (int)MENU_TYPE::SHOW_NORMAL);
		}
		else {
			glutChangeToMenuEntry((int)MENU_TYPE::SHOW_NORMAL+1, "Show Normal", (int)MENU_TYPE::SHOW_NORMAL);
		}
		if (show_2dpoly) {
			glutChangeToMenuEntry((int)MENU_TYPE::SHOW_2DPOLY + 1, "Hide 2D Polygon", (int)MENU_TYPE::SHOW_2DPOLY);
		}
		else {
			glutChangeToMenuEntry((int)MENU_TYPE::SHOW_2DPOLY + 1, "Show 2D Polygon", (int)MENU_TYPE::SHOW_2DPOLY);
		}
		if (show_model) {
			glutChangeToMenuEntry((int)MENU_TYPE::SHOW_MODEL + 1, "Hide Model", (int)MENU_TYPE::SHOW_MODEL);
		}
		else {
			glutChangeToMenuEntry((int)MENU_TYPE::SHOW_MODEL + 1, "Show Model", (int)MENU_TYPE::SHOW_MODEL);
		}
	}

	bool updateMenuFlag;
	void checkUpdateMenu() {
		if (updateMenuFlag) {
			updateMenu();
			updateMenuFlag = !updateMenuFlag;
		}
	}

	bool setModelFlag;
	bool getSetModelFlag() {
		return setModelFlag;
	}
	void resetSetModelFlag() {
		setModelFlag = false;
	}

	sketch3D::poly_to_3D* polyTo3D;
	void connectPolyTo3D(sketch3D::poly_to_3D* p_to_3d) {
		polyTo3D = p_to_3d;
	}

	bool exitFlag=false;//if true then show close dialog
	bool getExitFlag() {
		return exitFlag;
	}
	void resetExitFlag() {
		exitFlag = false;
	}

	WCHAR FileName[MAX_PATH];

	void menu(int item) {

		switch (item)
		{
		case (int)MENU_TYPE::EXPORT_AS_OBJ:
			//export 3D model as .obj
			if (polyTo3D->getPoint3DNum() != 0) {
				if (fileUtils::selectSaveFileInExplorer(FileName,
					L"Object Files(.obj)\0*.obj\0All Files(.)\0*.*\0",
					L"obj", 
					L"Export Obj")) {
					wprintf(L"save as %s\n", FileName);
					if (!(polyTo3D->exportAsObj(FileName))) {
						printf("save canceled");
					}
				}
			}
			else {
				printf("Draw before exporting.");
			}
			break;

		case (int)MENU_TYPE::EXPORT_AS_STROKE:
			//export input polygon
			if (polyTo3D->getPoint3DNum() != 0) {
				if (fileUtils::selectSaveFileInExplorer(FileName,
					L"Stroke Log(.stroke)\0 * .stroke\0All Files(.)\0 * .*\0",
					L"stroke",
					L"Export Stroke",
					L"stroke")) {
					wprintf(L"save as %s\n", FileName);
					if (!(polyTo3D->exportAsStroke(FileName, "stroke log"))) {
						printf("save canceled");
					}
				}
			}
			else {
				printf("Draw before exporting.");
			}
			break;

		case (int)MENU_TYPE::IMPORT_STROKE:
			//import 2d polygon and generate 3d model
			if (fileUtils::selectOpenFileInExplorer(
				FileName,
				L"Stroke Log(.stroke)\0 * .stroke\0All Files(.)\0 * .*\0",
				L"stroke",
				L"Import stroke",
				L"stroke"))
			{
				wprintf(L"import %s\n", FileName);
				try {
					polyTo3D->importStroke(FileName);
					setModelFlag = true;
				}
				catch(graph::graphException e){
					e.print();
					printf("import canceled\n");
				}
				catch (std::exception e) {
					printf(e.what());
					printf("import canceled\n");
				}
				catch (...) {
					printf("unexpected exception\n");
					printf("import canceled\n");
				}
				
			}
			break;

		case (int)MENU_TYPE::SHOW_NORMAL:
			show_normal = !show_normal;
			break;

		case (int)MENU_TYPE::SHOW_2DPOLY:
			show_2dpoly = !show_2dpoly;
			break;

		case (int)MENU_TYPE::SHOW_MODEL:
			show_model = !show_model;
			break;

		case (int)MENU_TYPE::EXIT:
			exitFlag = true;
			break;
		}
		updateMenuFlag = true;
		return;
	}

	void menuInit() {
		show_normal = false;
		show_2dpoly = false;
		show_model = true;

		// Create a menu
		glutCreateMenu(menu);

		// Add menu items
		glutAddMenuEntry("Export as .obj", (int)MENU_TYPE::EXPORT_AS_OBJ);
		glutAddMenuEntry("Export as .stroke", (int)MENU_TYPE::EXPORT_AS_STROKE);
		glutAddMenuEntry("Import .stroke", (int)MENU_TYPE::IMPORT_STROKE);
		glutAddMenuEntry("Show Normal", (int)MENU_TYPE::SHOW_NORMAL);
		glutAddMenuEntry("Show 2D Polygon", (int)MENU_TYPE::SHOW_2DPOLY);
		glutAddMenuEntry("Hide Model", (int)MENU_TYPE::SHOW_MODEL);
		glutAddMenuEntry("EXIT", (int)MENU_TYPE::EXIT);

		// Associate a mouse button with menu
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

	
}
