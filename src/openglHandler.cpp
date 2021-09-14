/*
 * File: openglHandler.cpp
 * --------------------
 * This file contains the implementation of some utils for OpenGL.
 *
 * See openglHandler.hpp for documentation of each member.
 *
 * @author Matyalatte
 * @version 2021/09/14
 * - initial commit
 */

#include "openglHandler.hpp"

namespace openglHandler {
	openglHandler::openglHandler() :
		vao(),
		vertexBuffer(), normalBuffer(),
		indexBuffer(), 
		modelProgramID(),
		colorID(), lightPosID(), modelRotID(),
		faceNum(), vertexNum(),

		normalLineBuffer(),
		normalProgramID(),
		normalModelRotID(), normalColorID(),

		modelRot(){}



	openglHandler::~openglHandler() {
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteBuffers(1, &normalLineBuffer);
		glDeleteVertexArrays(1, &vao);

		glDeleteProgram(modelProgramID);
		glDeleteProgram(normalProgramID);
	}

	void openglHandler::exit() {
		glutDestroyWindow(glutGetWindow());
	}

	//Initializes glutand glew, generates buffer, and links shader programs.
	void openglHandler::init(int argc, char** argv, char* window_name, int width, int height) {

		//initializes glut
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GL_DEPTH_BUFFER_BIT);
		glutInitWindowSize(width, height);

		glutCreateWindow(window_name);

		//initializes glew
		GLenum glewStatus = glewInit();
		if (glewStatus != GLEW_OK)
		{
			printf("Error!");
			std::exit(1);
		}
		printf("OpenGL version:%s\n", glGetString(GL_VERSION));

		//generates buffer
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &normalBuffer);
		glGenBuffers(1, &indexBuffer);
		glGenBuffers(1, &normalLineBuffer);

		//links shader
		modelProgramID = link_program("shader/model.vert", "shader/model.frag");
		normalProgramID = link_program("shader/basic.vert", "shader/basic.frag");

		modelRotID = glGetUniformLocation(modelProgramID, "modelRot");
		colorID = glGetUniformLocation(modelProgramID, "color");
		lightPosID = glGetUniformLocation(modelProgramID, "lightPos");
		normalModelRotID = glGetUniformLocation(normalProgramID, "modelRot");
		normalColorID = glGetUniformLocation(normalProgramID, "color");

	};

	// creates a shader
	void openglHandler::prepare_shader(GLenum shaderType, GLuint *shaderID, const char* file_path) {
		*shaderID = glCreateShader(shaderType);
		std::string ShaderCode;
		const char* type_str;
		if (shaderType == GL_VERTEX_SHADER) {
			type_str = "vertex";
		}
		else {
			type_str = "fragment";
		}

		//gets code from a file
		ShaderCode = fileUtils::read_txt(file_path);
		//printf("%s shader : %s\n", type_str, file_path);


		//compiles
		char const* SourcePointer = ShaderCode.c_str();
		glShaderSource(*shaderID, 1, &SourcePointer, NULL);
		glCompileShader(*shaderID);

		// checks compiling result
		GLint Result = GL_FALSE;
		int InfoLogLength;
		glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(*shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (Result == FALSE) {
			std::vector<char> ShaderErrorMessage(InfoLogLength);
			glGetShaderInfoLog(*shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
			fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
		}
	}

	// links a shader program
	GLuint openglHandler::link_program(const char* vertex_file_path, const char* fragment_file_path) {
		GLuint vshaderID;
		GLuint fshaderID;
		prepare_shader(GL_VERTEX_SHADER, &vshaderID, vertex_file_path);
		prepare_shader(GL_FRAGMENT_SHADER, &fshaderID, fragment_file_path);

		GLint Result = GL_FALSE;
		int InfoLogLength;

		//links program
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vshaderID);
		glAttachShader(programID, fshaderID);
		glDeleteShader(vshaderID);
		glDeleteShader(fshaderID);
		glLinkProgram(programID);

		//checks program
		glGetProgramiv(programID, GL_LINK_STATUS, &Result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage((std::max)(InfoLogLength, int(1)));
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		return programID;
	}

	//Sets a model data.
	void openglHandler::setModel(double* vertices, size_t* indices, double* normals, size_t vnum, size_t fnum) {
		GLuint* indices_ = new GLuint[fnum * 3];
		for (size_t i = 0; i < fnum * 3; i++) {
			indices_[i] = (GLuint)indices[i];
		}

		GLuint vnum_ = (GLuint) vnum;
		GLuint fnum_ = (GLuint) fnum;

		//set model data to buffer
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vnum_ * 3, vertices, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vnum_ * 3, normals, GL_STREAM_DRAW);
		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * fnum_, indices_, GL_STREAM_DRAW);

		glBindVertexArray(0);

		//generate line primitives to draw normal vectors
		GLdouble *normalLines = new GLdouble[vnum * 6];

		double px, py, pz, nx, ny, nz, nd;

		size_t i3, i6;
		for (size_t i = 0; i < vnum_; i++) {
			i3 = i * 3;
			nx = normals[i3];
			ny = normals[i3 + 1];
			nz = normals[i3 + 2];
			nd = sqrt(nx * nx + ny * ny + nz * nz) * 20;
			px = vertices[i3];
			py = vertices[i3 + 1];
			pz = vertices[i3 + 2];
			i6 = i * 6;

			normalLines[i6] = px;
			normalLines[i6 + 1] = py;
			normalLines[i6 + 2] = pz;
			normalLines[i6 + 3] = px + nx / nd;
			normalLines[i6 + 4] = py + ny / nd;
			normalLines[i6 + 5] = pz + nz / nd;
		}

		glBindBuffer(GL_ARRAY_BUFFER, normalLineBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vnum_ * 6, normalLines, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
		delete[] indices_;
		delete[] normalLines;

		//reset model rotation matrix to identity
		resetModelRotation();

		faceNum = fnum_;
		vertexNum = vnum_;
		printf("v:%d, f:%d\n", (int)vnum, (int)fnum);
	}

	//Draws the model.
	void openglHandler::draw() {
		glUseProgram(modelProgramID);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, faceNum * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void openglHandler::draw_normalLines(float r, float g, float b) {
		glUseProgram(normalProgramID);

		GLfloat c[3] = { r,g,b };
		glUniform3fv(normalColorID, 1, c);
		glDrawArrays(GL_LINES, 0, vertexNum * 2);
		glUseProgram(0);
	}

	//Draw objects without buffers.
	void openglHandler::draw_by_array(GLenum mode, double* vertices, size_t vnum, float r, float g, float b) {
		glColor3f(r, g, b);
		glBegin(mode);
		for (size_t i = 0; i < vnum; i++) {
			glVertex2d(vertices[i * 2], vertices[i * 2 + 1]);
		}
		glEnd();
	}

	//Draw a point
	void openglHandler::draw_point(double x, double y, float size, float r, float g, float b) {
		glColor3f(r, g, b);
		glPointSize(size);
		glBegin(GL_POINTS);
		glVertex2d(x, y);
		glEnd();
	}

	void openglHandler::drawString(const char* string, float x, float y, float r, float g, float b)
	{
		glColor3f(r, g, b);
		glRasterPos2f(x, y);
		int len, i;
		len = (int)strlen(string);
		for (i = 0; i < len; i++) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
		}
	}
	glm::mat4 rotationX(float angle) {
		return glm::mat4(
			1.0, 0, 0, 0,
			0, cos(angle), -sin(angle), 0,
			0, sin(angle), cos(angle), 0,
			0, 0, 0, 1);
	}

	glm::mat4 rotationY(float angle) {
		return glm::mat4(
			cos(angle), 0, sin(angle), 0,
			0, 1.0, 0, 0,
			-sin(angle), 0, cos(angle), 0,
			0, 0, 0, 1);
	}

	glm::mat4 rotationZ(float angle) {
		return glm::mat4(
			cos(angle), -sin(angle), 0, 0,
			sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	void openglHandler::setModelRotation() {
		glUseProgram(modelProgramID);
		glUniformMatrix4fv(modelRotID, 1, GL_FALSE, &modelRot[0][0]);
		glUseProgram(0);
		glUseProgram(normalProgramID);
		glUniformMatrix4fv(normalModelRotID, 1, GL_FALSE, &modelRot[0][0]);
		glUseProgram(0);
	}

	void openglHandler::resetModelRotation() {
		modelRot = glm::mat4(1.0);
		setModelRotation();
	}

	void openglHandler::rotateModel(float x, float y, float z) {
		modelRot = rotationX(x)*rotationY(y)*rotationZ(z)*modelRot;
		setModelRotation();
	}

	void openglHandler::setUniformVec(GLuint ID, int dim, float* vec) {
		glUseProgram(modelProgramID);
		glUniform3fv(ID, 1, vec);
		glUseProgram(0);
	}
	void openglHandler::setModelColor(float r, float g, float b) {
		GLfloat c[3] = { r,g,b };
		setUniformVec(colorID, 3, c);
	}
	void openglHandler::setLightPos(float x, float y, float z) {
		GLfloat pos[3] = { x,y,z };
		setUniformVec(lightPosID, 3, pos);
	}
}