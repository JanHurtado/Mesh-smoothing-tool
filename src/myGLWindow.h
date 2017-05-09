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

/**
 * @brief The myGLWindow class - Qt Widget for OpenGL
 * Designed to support only one shape and one selection  at time.
 * So in the containers of the renderer there are two shapes at
 * position 0 and 1. The first one for the current shape and
 * the second one for the current selection. This object adapts
 * a renderer to a QT OpenGL Widget which manages a single OpenGL
 * context. Also, it simplifies the usage of the renderer for the
 * mesh smoothing application.
 */
class myGLWindow : public QOpenGLWidget
{
    /**
     * @brief renderer - renderer object for visualization in the widget
     */
	myRenderer * renderer;
protected:
    /**
     * @brief initializeGL - initialize the renderer in the current OpenGL context
     */
	void initializeGL();
    /**
     * @brief paintGL - call renderer draw functions
     */
	void paintGL();
    /**
     * @brief event - event manager for mouse interactions (in myGLWindow)
     * @param event - input event
     * @return success
     */
	bool event(QEvent *event);
public:
    /**
     * @brief myGLWindow - default constructor
     * @param parent parent widget pointer
     */
    myGLWindow(QWidget *parent = 0);
    /**
     * @brief ~myGLWindow - destructor
     */
	~myGLWindow();
    /**
     * @brief sendDataToOpenGL - call sendDataToOpenGL of the renderer, in the current context
     */
	void sendDataToOpenGL();
    /**
     * @brief setVisualizationMode - set rendering mode of the current shape
     * @param mode - input rendering mode
     */
	void setVisualizationMode(int mode);
    /**
     * @brief setShape - set a shape as current shape (removing previous shape)
     * @param _shape new shape
     */
	void setShape(ShapeData * _shape);
    /**
     * @brief setSelection - set a selection as current selection (removing previous selection)
     * @param _selection new selection
     */
	void setSelection(ShapeData * _selection);
    /**
     * @brief removeSelection - remove current selection
     */
	void removeSelection();
    /**
     * @brief updateMesh - update vertex data of the current shape (resend vertex data)
     */
	void updateMesh();
    /**
     * @brief addShader - add a shader to the renderer
     * @param _shaderType - shader type
     * @param _fileName - shader code file name
     */
	void addShader(GLenum _shaderType, const string & _fileName);
    /**
     * @brief clearAndDeleteShaders - clear, deattach and delete shaders of the renderer
     */
	void clearAndDeleteShaders();
    /**
     * @brief installShaders - call installShaders of the renderer, in the current context.
     */
	void installShaders();
    /**
     * @brief getCurrentMousePosition - get mouse position in the OpenGL Window (Qt Widget)
     * @return mouse position
     */
	glm::vec2 getCurrentMousePosition();
    /**
     * @brief getRayDirection - call getRayDirection of the renderer for a given position
     * @param pos - position
     * @return ray direction
     */
	glm::vec3 getRayDirection(glm::vec2 & pos);
    /**
     * @brief getCamera - get pointer to the camera of the renderer
     * @return camera pointer
     */
	myCamera * getCamera();
    /**
     * @brief getModelToWorldMatrix - get model to world matrix applied to all objects in the renderer
     * @return 4x4 model to world matrix
     */
	glm::mat4 getModelToWorldMatrix();
};
#endif // MYGLWINDOW_H
