#ifndef UTIL_H
#define UTIL_H

#include "mesh.h"
#include <vector>
#include <queue>

using namespace std;

#define PI 3.14159265359

/** @addtogroup mesh_processing
  * @brief Mesh processing auxiliar functions.
  *
  * @{
  */

/**
 * @brief getArea - area computation of a mesh (sum of triangle areas)
 * @param mesh input mesh
 * @return total area
 */
num_t getArea (TriMesh & mesh);

/**
 * @brief getVolume - volume computation of a mesh
 * @param mesh input mesh
 * @return total area
 */
num_t getVolume ( TriMesh & mesh );

/**
 * @brief getAverageEdgeLength - average edge length computation of a mesh
 * @param mesh input mesh
 * @return average edge length
 */
num_t getAverageEdgeLength(TriMesh &mesh);

/**
 * @brief getAllFaceAreas - area computation for all faces
 * @param mesh input mesh
 * @param areas output vector containing areas for all faces
 */
void getAllFaceAreas(TriMesh &mesh, vector<num_t> &areas);

/**
 * @brief getAllFaceCentroids - centroid computation for all faces
 * @param mesh input mesh
 * @param centroids output vector containing centroids for all faces
 */
void getAllFaceCentroids(TriMesh &mesh, vector<TriMesh::Point> &centroids);

/**
 * @brief getAllFaceNormals - normal computation for all faces
 * @param mesh input mesh
 * @param normals vector containing normals for all faces
 */
void getAllFaceNormals(TriMesh &mesh, vector<TriMesh::Normal> &normals);

/**
 * @brief getFaceVertexAngle - angle computation for a given vertex included in a face
 * @param mesh input mesh
 * @param fh corresponding face handle
 * @param vh corresponding vertex handle
 * @param angle angle
 */
void getFaceVertexAngle(TriMesh &mesh,TriMesh::FaceHandle fh,TriMesh::VertexHandle vh, num_t & angle);

/**
 * @brief getVertexArea - vertex area computation (barycentric area)
 * @param mesh input mesh
 * @param vh input vertex (vertex handle)
 * @param areas all precomputed triangle areas
 * @return vertex area
 */
num_t getVertexArea(TriMesh & mesh,TriMesh::VertexHandle vh,vector<num_t> & areas);

/**
 * @brief getAllVertexAreas - vertex area computation for all vertices (barycentric areas)
 * @param mesh input mesh
 * @param areas output vector containing all vertex areas
 */
void getAllVertexAreas(TriMesh & mesh, vector<num_t> & areas);

/**
 * @brief getAllPoints - get all vertex coordinates
 * @param mesh input mesh
 * @param points all vertex coordinates
 */
void getAllPoints(TriMesh &mesh, vector<TriMesh::Point> & points);

/**
 * @brief GaussianWeight - gaussian function computation used for bilateral filtering
 * @param distance input distance
 * @param sigma input sigma
 * @return function value for inputs (distance and sigma)
 */
inline num_t GaussianWeight(num_t distance, num_t sigma)
{
    return static_cast<num_t>(exp( -0.5 * distance * distance / (sigma * sigma)));
}

/**
 * @brief NormalDistance - computation of normal distance: \f$ |n1-n2| \f$
 * @param n1 - input normal 1
 * @param n2 - input normal 2
 * @return normal distance
 */
inline num_t NormalDistance(const TriMesh::Normal &n1, const TriMesh::Normal &n2)
{
    return (n1 - n2).length();
}

/** @} */

#endif // UTIL_H
