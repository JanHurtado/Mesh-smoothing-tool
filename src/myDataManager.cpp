#include "myDataManager.h"


myDataManager::myDataManager()
{
}


myDataManager::~myDataManager()
{
	input_mesh.clear();
	output_mesh.clear();
	input_mesh_shape.clear();
	output_mesh_shape.clear();
	selection.clear();
}


void myDataManager::loadInputMesh(string & fileName)
{
	input_mesh.clear();
	output_mesh.clear();
	input_mesh_shape.clear();
	output_mesh_shape.clear();

	importMesh(input_mesh, fileName);
	input_mesh.request_face_normals();
	input_mesh.request_vertex_normals();
	input_mesh.update_normals();

	output_mesh = input_mesh;
}

void myDataManager::saveOutputMesh(string & fileName)
{
	output_mesh.request_face_normals();
	output_mesh.request_vertex_normals();
	output_mesh.update_normals();
	exportMesh(output_mesh, fileName);
}

void myDataManager::updateInputShape()
{
	input_mesh_shape.clear();
	input_mesh_shape.loadMesh(input_mesh);
}

void myDataManager::updateOutputShape()
{
	output_mesh_shape.clear();
	output_mesh_shape.loadMesh(output_mesh);
}

void myDataManager::updateShapes()
{
	updateInputShape();
	updateOutputShape();
}

void myDataManager::updateOutputSelection(vector<size_t> & indices)
{
	selection.clear();
	selection.loadMeshVertexSelection(output_mesh, indices);
}

void myDataManager::setOutputAsInput()
{
	input_mesh = output_mesh;
	updateShapes();
}

void myDataManager::reinitialize()
{
	output_mesh = input_mesh;
	updateShapes();
}