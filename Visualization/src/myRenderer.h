#ifndef MYRENDERER_H
#define MYRENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "myShader.h"
#include "myShape.h"
#include "myCamera.h"

enum flags {e_draw_faces,e_draw_wireframe,e_draw_points};

class myRenderer
{
public:
	GLuint m_width;
	GLuint m_height;
	GLfloat m_fov;
	GLfloat m_near;
	GLfloat m_far;
	glm::mat4 modelToWorldMatrix;

	myCamera camera;

	glm::vec2 oldMousePosition;

	int currentDrawFlag;
	glm::vec3 sceneCentralPoint;

	myRenderer();
	virtual ~myRenderer();
	void addShape(ShapeData * _sd);
	void addShader(GLenum _shaderType, const string & _fileName);
	void createProgram();
	bool installShaders();
	void sendDataSingleBuffer();
	void draw();
	void initialize();
	void initializeInteractor();
	void computeCentralPoint();
	void rotate(const glm::vec2& newMousePosition);

	GLuint getProgramID(){ return m_programID; }
	GLsizeiptr getIndexOffsetAt(size_t pos){ return m_elementOffsets[pos]; }
	GLuint getVertexArrayObjectIDAt(size_t pos){ return m_vertexArrayObjectIDs[pos]; }
private:
	vector<myShader*> m_shaders;
	GLuint m_programID;


	vector<ShapeData*> m_shapes;
	vector<GLsizeiptr> m_elementOffsets;
	vector<GLsizeiptr> m_vertexOffsets;
	vector<GLuint> m_vertexArrayObjectIDs;
	GLuint m_vertexBufferID;
	GLuint m_elementBufferID;
};

#endif // MYRENDERER_H