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

#include "myCamera.h"
#include "myShape.h"
#include "myRenderer.h"


using namespace std;

class myGLWindow : public QOpenGLWidget
{
protected:
	myCamera mainCamera;
	GLuint mainProgramID;
	ShapeData sd;
	ShapeData sd2;
	myRenderer * renderer;


	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);

	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	string readShaderCode(const char * fileName);
	void installShaders();
	void sendDataToOpenGL();

public:
	myGLWindow::myGLWindow(QWidget *parent = 0);
	~myGLWindow();

};
#endif // MYGLWINDOW_H