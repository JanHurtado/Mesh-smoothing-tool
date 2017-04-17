#ifndef MYDATAMANAGER_H
#define MYDATAMANAGER_H

#include "mesh/iomesh.h"

class myDataManager
{
public:
	TriMesh input_mesh;
	TriMesh output_mesh;
	TriMesh temporal_mesh;

	myDataManager();
	~myDataManager();
};
#endif // MYDATAMANAGER_H
