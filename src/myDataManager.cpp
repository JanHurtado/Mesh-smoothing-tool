#include "myDataManager.h"


myDataManager::myDataManager()
{
}


myDataManager::~myDataManager()
{
	input_mesh.clear();
	output_mesh.clear();
	temporal_mesh.clear();
	input_mesh_shape.clear();
	output_mesh_shape.clear();
}


void myDataManager::loadInputMesh(string & fileName)
{
	input_mesh.clear();
	output_mesh.clear();
	temporal_mesh.clear();
	input_mesh_shape.clear();
	output_mesh_shape.clear();

	importMesh(input_mesh, fileName);
	input_mesh.request_face_normals();
	input_mesh.request_vertex_normals();
	input_mesh.update_normals();
	temporal_mesh = input_mesh;
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

void myDataManager::updateInputShape(vector<size_t> & indices)
{

}

void myDataManager::updateOutputShape()
{
	output_mesh_shape.clear();
	output_mesh_shape.loadMesh(output_mesh);
}

void myDataManager::updateOutputShape(vector<size_t> & indices)
{

}

void myDataManager::updateShapes()
{
	updateInputShape();
	updateOutputShape();
}