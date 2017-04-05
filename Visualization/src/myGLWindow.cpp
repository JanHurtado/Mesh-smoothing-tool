#include "myGLWindow.h"

myGLWindow::myGLWindow(QWidget *parent)
: QOpenGLWidget(parent)
{
	sd.loadFromFile("test.off");
	sd2.loadFromFile("test3.off");
}


void myGLWindow::sendDataToOpenGL()
{
}

void myGLWindow::initializeGL()
{
	setMinimumSize(1000, 800);
	renderer = new myRenderer;
	//renderer->addShape(&sd);
	renderer->addShape(&sd2);
	renderer->addShader(GL_VERTEX_SHADER, "VertexShaderCode2.glsl");
	renderer->addShader(GL_FRAGMENT_SHADER, "FragmentShaderCode.glsl");
	renderer->initialize();
}

void myGLWindow::paintGL()
{
	renderer->m_width = width();
	renderer->m_height = height();
	renderer->draw();
}

void myGLWindow::mouseMoveEvent(QMouseEvent* e)
{
	//setFocus();
	renderer->rotate(glm::vec2(e->x(), e->y()));
	repaint();
}

void myGLWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		renderer->camera.moveForward();
		break;
	case Qt::Key::Key_S:
		renderer->camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		renderer->camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		renderer->camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		renderer->camera.moveUp();
		break;
	case Qt::Key::Key_F:
		renderer->camera.moveDown();
		break;
	case Qt::Key::Key_J:
		renderer->currentDrawFlag = e_draw_faces;
		break;
	case Qt::Key::Key_K:
		renderer->currentDrawFlag = e_draw_wireframe;
		break;
	case Qt::Key::Key_L:
		renderer->currentDrawFlag = e_draw_points;
		break;
	}
	repaint();
}

myGLWindow::~myGLWindow()
{
	//glUseProgram(0);
	//glDeleteProgram(mainProgramID);
	delete renderer;
}
