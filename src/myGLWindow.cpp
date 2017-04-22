#include "myGLWindow.h"

myGLWindow::myGLWindow(QWidget *parent)
: QOpenGLWidget(parent)
{
	//current_shape.loadFromFile("test.off");
	//sd2.loadFromFile("test3.off");
}


void myGLWindow::sendDataToOpenGL()
{
	renderer->sendDataSingleBuffer();
}

void myGLWindow::initializeGL()
{
	//setMinimumSize(1000, 800);
	renderer = new myRenderer;
	/*ShapeData * sd = new ShapeData;
	sd->loadFromFile("test2.off");
	renderer->addShape(sd);
	ShapeData * sd2 = new ShapeData;
	sd2->loadFromFile("test3.off");
	renderer->addShape(sd2);*/

	//renderer->addShape(&current_shape);
	//renderer->addShape(&sd2);
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
	if (e->buttons() == Qt::LeftButton)
		renderer->rotateObjects(glm::vec2(e->x(), e->y()));
	if (e->buttons() == Qt::RightButton)
		renderer->translateCamera(glm::vec2(e->x(), e->y()));
	repaint();
}

void myGLWindow::wheelEvent(QWheelEvent *event)
{
	renderer->zoom(static_cast<float>(event->delta()));
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

void myGLWindow::setShape(ShapeData * _shape)
{
	renderer->clearShapes();
	renderer->addShape(_shape);
	makeCurrent();
	renderer->resendDataSingleBuffer();
	doneCurrent();
	renderer->setDefaultValues();
	repaint();
}

void myGLWindow::updateMesh()
{
	makeCurrent();
	renderer->updateVertexBuffer(0);
	doneCurrent();
	repaint();
}

myGLWindow::~myGLWindow()
{
	//glUseProgram(0);
	//glDeleteProgram(mainProgramID);
	delete renderer;
}
