#ifndef MYGLWINDOW_H
#define MYGLWINDOW_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qopenglwidget.h>
#include <QtGui/qmouseevent>
#include <QtGui/qkeyevent>
#include <QtGui/qwheelevent>

#include "visualization/myRenderer.h"

using namespace std;

class myGLWindow : public QOpenGLWidget
{

	myRenderer * renderer;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL();
	bool event(QEvent *event);
public:
	myGLWindow::myGLWindow(QWidget *parent = 0);
	~myGLWindow();

	void sendDataToOpenGL();
	void setVisualizationMode(int mode);
	void setShape(ShapeData * _shape);
	void setSelection(ShapeData * _selection);
	void removeSelection();
	void updateMesh();
	void addShader(GLenum _shaderType, const string & _fileName);
	void clearAndDeleteShaders();
	void installShaders();
	glm::vec2 getCurrentMousePosition();
	glm::vec3 getRayDirection(glm::vec2 & pos);
	myCamera * getCamera();
	glm::mat4 getModelToWorldMatrix();
};
#endif // MYGLWINDOW_H