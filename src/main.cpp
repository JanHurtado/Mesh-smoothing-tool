
#include "myMainWindow.h"
#include "test.h"

#include <string>
#include <fstream>
#include <streambuf>


/** @defgroup mesh_processing Mesh Processing
*   Module containing mesh processing tools useful for the application
*/

/** @defgroup visualization Visualization based on OpenGL
*   Module containing OpenGL abstractions for visualization of triangular meshes
*/

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	myMainWindow main_window;
	main_window.show();
	/*myGLWindow myWindow;
	myWindow.show();*/
	printf("OpenGL version supported by this platform (%s): \n",glGetString(GL_VERSION));
	initializeTestCases();
	runTests();
	return app.exec();
}
