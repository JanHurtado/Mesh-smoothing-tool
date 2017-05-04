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

enum myDrawFlags {e_draw_faces,e_draw_wireframe,e_draw_points,e_draw_selection};

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
	glm::vec4 light;
	glm::vec3 lightPosition;

	glm::vec2 oldMousePosition;

	glm::vec3 sceneCentralPoint;
	float boundingSphereRadius;

	myRenderer();
	virtual ~myRenderer();
	void addShape(ShapeData * _sd);
	void addShape(ShapeData * _sd, myDrawFlags _draw_mode);
	void setShapeDrawMode(size_t _shape_index, myDrawFlags _mode);
	void removeShape(size_t index);
	void addShader(GLenum _shaderType, const string & _fileName);
	void addShader(myShader * _shader);
	void clearShapes();
	void clearAndDeleteShapes();
	void clearShaders();
	void clearAndDeleteShaders();
	void createProgram();
	bool installShaders();
	void sendDataSingleBuffer();
	void updateVertexBuffer(size_t index);
	void resendDataSingleBuffer();



	void draw();
	void initialize();
	void initializeInteractor();
	void setDefaultValues();
	void computeCentralPoint();
	void computeBoundingSphereRadius();
	void rotateObjects(const glm::vec2& newMousePosition);
	void translateCamera(const glm::vec2& newMousePosition);
	void zoom(float delta);

	GLuint getProgramID(){ return m_programID; }
	GLsizeiptr getIndexOffsetAt(size_t pos){ return m_elementOffsets[pos]; }
	GLuint getVertexArrayObjectIDAt(size_t pos){ return m_vertexArrayObjectIDs[pos]; }
	size_t getNumberOfShapes(){ return m_shapes.size(); }
	glm::vec3 getRayDirection(glm::vec2 & pos);


	void clearData();
private:
	vector<myShader*> m_shaders;
	GLuint m_programID;

	vector<ShapeData*> m_shapes;
	vector<myDrawFlags> m_draw_modes;
	vector<GLsizeiptr> m_elementOffsets;
	vector<GLsizeiptr> m_vertexOffsets;
	vector<GLuint> m_vertexArrayObjectIDs;
	GLuint m_vertexBufferID;
	GLuint m_elementBufferID;
};

#endif // MYRENDERER_H