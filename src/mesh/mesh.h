#ifndef MESH_H
#define MESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <iostream>
#include <queue>

using namespace std;

/** @addtogroup mesh_processing
  * @brief Mesh processing half-edge data structure definition (OpenMesh library).
  *
  * @{
  */

/**
 * @brief The MyTraits struct - OpenMesh custom traits
 */
struct MyTraits : OpenMesh::DefaultTraits
{
  // Let Point and Normal be a vector of doubles
  typedef OpenMesh::Vec3f Point;
  typedef OpenMesh::Vec3f Normal;

  // The default 1D texture coordinate type is float.
  typedef double  TexCoord1D;
  // The default 2D texture coordinate type is OpenMesh::Vec2f.
  typedef OpenMesh::Vec2f  TexCoord2D;
  // The default 3D texture coordinate type is OpenMesh::Vec3f.
  typedef OpenMesh::Vec3f  TexCoord3D;

    //enable standart properties
    VertexAttributes   (OpenMesh::Attributes::Status|OpenMesh::Attributes::Normal|OpenMesh::Attributes::Color);
    HalfedgeAttributes (OpenMesh::Attributes::Status|OpenMesh::Attributes::PrevHalfedge);
    FaceAttributes     (OpenMesh::Attributes::Status|OpenMesh::Attributes::Normal|OpenMesh::Attributes::Color);
    EdgeAttributes     (OpenMesh::Attributes::Status|OpenMesh::Attributes::Color);
};

/**
 * @brief TriMesh - triangular mesh definition
 */
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> TriMesh;

/**
 * @brief num_t - type definition for numbers used in mesh processing module
 */
typedef float num_t;


/** @} */

#endif // MESH_H

