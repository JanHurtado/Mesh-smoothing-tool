#ifndef MYDATAMANAGER_H
#define MYDATAMANAGER_H

#include "mesh/iomesh.h"
#include "visualization/myShape.h"

class myDataManager
{
public:
	TriMesh input_mesh;
	TriMesh output_mesh;
	TriMesh temporal_mesh;

	ShapeData input_mesh_shape;
	ShapeData output_mesh_shape;

	ShapeData selection;

	myDataManager();
	~myDataManager();

	void loadInputMesh(string & fileName);
	void saveOutputMesh(string & fileName);
	void updateInputShape();
	void updateInputShape(vector<size_t> & indices);
	void updateOutputShape();
	void updateOutputShape(vector<size_t> & indices);
	void updateShapes();
	void updateOutputSelection(vector<size_t> & indices);
};
#endif // MYDATAMANAGER_H
