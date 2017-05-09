#ifndef IOMESH_H
#define IOMESH_H

#include "mesh.h"

/** @addtogroup mesh_processing
  * @brief Mesh processing I/O functions (OpenMesh library).
  *
  * @{
  */

/**
 * @brief importMesh - read mesh file (.obj, .off, .ply, .stl)
 * @param mesh - output mesh
 * @param filename - input file name
 * @return success
 */
bool importMesh(TriMesh &mesh, string filename);

/**
 * @brief exportMesh - write mesh file (.obj, .off, .ply, .stl)
 * @param mesh - input mesh
 * @param filename - output file name
 * @return success
 */
bool exportMesh(TriMesh &mesh, string filename);

/** @} */

#endif // IOMESH_H
