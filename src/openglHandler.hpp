/*
 * File: openglHandler.hpp
 * --------------------
 * This file contains the declaration of some utils for OpenGL.
 *
 * See openglHandler.cpp for implementation of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#pragma once

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "2dpoly_to_3d/utils.hpp"

namespace openglHandler {
	class openglHandler {
	private:

		//*parameters*
		
		//3D model data
		GLuint vao; //ID of vertex array object
		GLuint vertexBuffer; //ID of vertex buffer object
		GLuint normalBuffer; //ID of color buffer object
		GLuint indexBuffer; //ID of index buffer object
		GLuint modelProgramID; //ID of program for 3D model
		GLuint colorID; //ID of color parameter for 3D model
		GLuint modelRotID; //ID of quaternion parameter for 3D model
		GLuint lightPosID;//ID of light position parameter for 3D model
		
		GLuint faceNum; //the number of faces
		GLuint vertexNum;//the number of vertices

		GLuint normalLineBuffer;//ID of buffer object for normal vectors
		GLuint normalProgramID; //ID of program for normal vectors
		GLuint normalColorID; //ID of color parameter for normal vectors
		GLuint normalModelRotID;//ID of quaternion parameter for normal vectors

		glm::mat4 modelRot;//model rotation matrix

		//*private methods*

		//load shader script
		void prepare_shader(GLenum shaderType, GLuint* shaderID, const char* file_path);

		//link a shader program
		GLuint link_program(const char* vertex_file_path, const char* fragment_file_path);

		//send parameters to shader
		void setUniformVec(GLuint ID, int dim, float* vec);

		//send model rotation matrix to shader
		void setModelRotation();

	public:
		/*
		* Constructor: openglHandler
		* Usage: openglHander GL_handler();
		* ---------------------------------
		* Creates an openGL hander object.
		*/
		openglHandler();
		~openglHandler();

		void exit();

		/*
		* Method: init
		* Usage: GL_handler.init(argc, argv, window_name, width, height, vertex_file_path, fragment_file_path);
		* ---------------------------------
		* Initializes glut and glew, generates buffer, and links shader programs. 
		* argc : argc of main method.
		* argv : argv of main method.
		* window_name : window name.
		* width, height : window size.
		*/
		void init(int argc, char** argv, char* window_name, int width, int height);

		/*
		* Method: setModel
		* Usage: GL_handler.setModel(vertices, indices, vnum, fnum);
		* ---------------------------------
		* Sets a model data.
		* vertices : 3d coordinates of vertices. vertices={v1_x, v1_y, v1_z, v2_x, v2_y, v2_z , v3_x, ...}
		* indices : Indices of vertices representing faces. indices={f1_v1, f1_v2, f1_v3, f2_v1, f2_v2, f2_v3, f3_v1, ...}
		* vnum : the number of vertices. vnum = len(vertices) / 3
		* fnum : the number of faces. fnum = len(indices) / 3
		*/
		void setModel(double* vertices, size_t* indices, double* normals, size_t vnum, size_t fnum);
		
		/*
		* Method: draw
		* Usage: GL_handler.draw();
		* ---------------------------------
		* Draws the model.
		*/
		void draw();

		/*
		* Method: draw_normalLines
		* Usage: GL_handler.draw_normalLines(r,g,b);
		* ---------------------------------
		* Draws normal vectors.
		* r,g,b: the color of the normal vectors.
		*/
		void draw_normalLines(float r, float g, float b);

		/*
		* Method: draw_by_array
		* Usage: GL_handler.draw_by_array(mode, vertex, vnum, r, g, b);
		* ---------------------------------
		* Draw objects without buffers.
		* mode : Symbolic constant representing  the primitives that will be drawn
		* vertices : 3d coordinates of vertices. vertices={v1_x, v1_y, v1_z, v2_x, v2_y, v2_z , v3_x, ...}
		* vnum : the number of the vertices. vnum = len(vertices) / 3
		* r,g,b: the color of the primitives.
		*/
		void draw_by_array(GLenum mode, double* vertices, size_t vnum, float r, float g, float b);

		
		/*
		* Method: draw_point
		* Usage: GL_handler.draw_point(x, y, size, r, g, b);
		* ---------------------------------
		* Draw a point
		* x, y : 2d coordinates of the point.
		* size : the size of the point.
		* r,g,b: the color of the point.
		*/
		void draw_point(double x, double y, float size, float r, float g, float b);

		//draw string
		void drawString(const char* string, float x, float y, float r=1.0, float g=1.0, float b=1.0);

		//reset model rotation matrix to identity
		void resetModelRotation();

		//rotate model
		void rotateModel(float x, float y, float z=0);

		//set model color
		void setModelColor(float r, float g, float b);

		//set light position
		void setLightPos(float x, float y, float z);
	};
}