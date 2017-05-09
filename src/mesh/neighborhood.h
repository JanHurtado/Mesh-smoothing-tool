#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "mesh.h"
#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/** @addtogroup mesh_processing
  * @brief Mesh processing neighborhood functions.
  *
  * @{
  */

/////////////////////////////////////////////////
/// Vertex neighborhood
/////////////////////////////////////////////////

/**
 * @brief getVertexNeighbors: get all neighboring vertices of a given vertex regarding a depth k (k-ring)
 * @param mesh: input mesh
 * @param vh: evaluated vertex
 * @param k: depth
 * @param vertex_neighbors: vector containing neighbors (output)
 */
void getVertexNeighbors(TriMesh &mesh, TriMesh::VertexHandle vh, int k, vector<TriMesh::VertexHandle> &vertex_neighbors);

/**
 * @brief getAdaptiveVertexNeighbors: get all neighboring vertices of a given vertex regarding a radius
 * @param mesh: input mesh
 * @param vh: evaluated vertex
 * @param radius: radius
 * @param vertex_neighbors: vector containing neighbors (output)
 */
void getAdaptiveVertexNeighbors(TriMesh &mesh, TriMesh::VertexHandle vh, num_t radius, vector<TriMesh::VertexHandle> &vertex_neighbors);

/**
 * @brief getAllVertexNeighbors: get all neighboring vertices of all vertices regarding a depth k (k-ring)
 * @param mesh: input mesh
 * @param k: depth
 * @param all_vertex_neighbors: vector containing all neighbor vectors (output)
 */
void getAllVertexNeighbors(TriMesh &mesh, int k,vector<vector<TriMesh::VertexHandle> > &all_vertex_neighbors);

/**
 * @brief getAllAdaptiveVertexNeighbors: get all neighboring vertices of all vertices regarding a radius
 * @param mesh: input mesh
 * @param radius: radius
 * @param all_vertex_neighbors: vector containing all neighbor vectors (output)
 */
void getAllAdaptiveVertexNeighbors(TriMesh &mesh, num_t radius, vector<vector<TriMesh::VertexHandle> > &all_vertex_neighbors);


/////////////////////////////////////////////////
/// Face neighborhood
/////////////////////////////////////////////////

/**
 * @brief getFaceNeighbors_EdgeBased: get neighboring faces based on face edges
 * @param mesh: input mesh
 * @param fh: evaluated face
 * @param face_neighbors: vector containing neighbors (output)
 */
void getFaceNeighbors_EdgeBased(TriMesh &mesh, TriMesh::FaceHandle fh, vector<TriMesh::FaceHandle> &face_neighbors);

/**
 * @brief getFaceNeighbors_VertexBased: get neighboring faces based on face vertices
 * @param mesh: input mesh
 * @param fh: evaluated face
 * @param face_neighbors: vector containing neighbors (output)
 */
void getFaceNeighbors_VertexBased(TriMesh &mesh, TriMesh::FaceHandle fh, vector<TriMesh::FaceHandle> &face_neighbors);

/**
 * @brief getFaceNeighbors_RadiusBased: get neighboring faces regarding a radius
 * @param mesh: input mesh
 * @param fh: evaluated face
 * @param radius: radius
 * @param face_neighbors: vector containing neighbors (output)
 */
void getFaceNeighbors_RadiusBased(TriMesh &mesh, TriMesh::FaceHandle fh, num_t radius, vector<TriMesh::FaceHandle> &face_neighbors);

/**
 * @brief getAllFaceNeighbors_EdgeBased: get all neighboring faces of all faces (edge based)
 * @param mesh: input mesh
 * @param include_target_face: include fh as neighbor
 * @param all_face_neighbors: vector containing neighbors (output)
 */
void getAllFaceNeighbors_EdgeBased(TriMesh &mesh, bool include_target_face, vector<vector<TriMesh::FaceHandle> > &all_face_neighbors);

/**
 * @brief getAllFaceNeighbors_VertexBased: get all neighboring faces of all faces (vertex based)
 * @param mesh: input mesh
 * @param include_target_face: include fh as neighbor
 * @param all_face_neighbors: vector containing neighbors (output)
 */
void getAllFaceNeighbors_VertexBased(TriMesh &mesh, bool include_target_face, vector<vector<TriMesh::FaceHandle> > &all_face_neighbors);

/** @} */

#endif // NEIGHBORHOOD_H
