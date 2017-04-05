
#include "myGLWindow.h"
#include "myShader.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	myGLWindow myWindow;
	myWindow.show();
	printf("OpenGL version supported by this platform (%s): \n",
	glGetString(GL_VERSION));
	return app.exec();
}