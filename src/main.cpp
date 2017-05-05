
#include "myMainWindow.h"

#include <string>
#include <fstream>
#include <streambuf>



int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	myMainWindow main_window;
	main_window.show();
	/*myGLWindow myWindow;
	myWindow.show();*/
	printf("OpenGL version supported by this platform (%s): \n",
	glGetString(GL_VERSION));
	return app.exec();
}