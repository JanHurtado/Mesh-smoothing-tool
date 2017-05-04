#ifndef MYGLWINDOW_H
#define MYGLWINDOW_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <QtWidgets/qapplication.h>
#include <GL/glew.h>
#include <QtWidgets/qopenglwidget.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include <QtGui/qmouseevent>
#include <QtGui/qkeyevent>
#include <QtGui/qwheelevent>

#include "visualization/myRenderer.h"


using namespace std;

class myGLWindow : public QOpenGLWidget
{
protected:

	myRenderer * renderer;

	void initializeGL();
	void paintGL();
	void resizeGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void wheelEvent(QWheelEvent *event);
	bool event(QEvent *event);

	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	string readShaderCode(const char * fileName);
	void sendDataToOpenGL();


public:

	ShapeData current_shape;

	void setVisualizationMode(int mode){ renderer->setShapeDrawMode(0,(myDrawFlags)mode); repaint(); }
	void setShape(ShapeData * _shape);
	void removeSelection();
	void setSelection(ShapeData * _selection);
	void updateMesh();
	void addShader(GLenum _shaderType, const string & _fileName);
	void clearAndDeleteShaders();
	void installShaders();
	glm::vec2 getCurrentMousePosition();
	glm::vec3 getRayDirection(glm::vec2 & pos){ return renderer->getRayDirection(pos); }
	myCamera * getCamera(){return &(renderer->camera);}
	glm::mat4 getModelToWorldMatrix(){ return renderer->modelToWorldMatrix; }


	myGLWindow::myGLWindow(QWidget *parent = 0);
	~myGLWindow();

};
#endif // MYGLWINDOW_H