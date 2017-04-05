#ifndef IOMESH_H
#define IOMESH_H

#include "mesh.h"

bool importMesh(TriMesh &mesh, string filename);

bool exportMesh(TriMesh &mesh, string filename);

#endif // IOMESH_H
