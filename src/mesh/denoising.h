#ifndef DENOISING_H
#define DENOISING_H

#include "neighborhood.h"
#include "util.h"

/**
 * @brief The FaceNeighborType enum
 * kVertexBased : vertex based face neighborhood
 * kEdgeBased : edge based face neighborhood
 * kRadiusBased : radius based face neighborhood
 */
enum FaceNeighborType{kVertexBased, kEdgeBased, kRadiusBased};

/** @defgroup groupAuxiliarFunctions Auxiliar functions group
 *  This group contains auxiliar functions for mesh denoising algorithms
 *  @{
 */

/**
 * @brief updateVertexPositions: this function compute new vertex positions adapted to an input normal field.
 * @param mesh: input mesh.
 * @param filtered_normals: input normal field.
 * @param iteration_number: number of iterations of the optimization algorithm (gradient descent approach).
 * @param fixed_boundary: determines if a boundary vertex will be updated
 */
void updateVertexPositions(TriMesh &mesh, vector<TriMesh::Normal> &filtered_normals, int iteration_number, bool fixed_boundary);

/**
 * @brief getSigmaC: sigma_c computation based on the average of face centroid distances (only between adjacent faces), and multiplied by a scalar
 * @param mesh: input mesh
 * @param face_centroids: precomputed face centroids
 * @param sigma_c_scalar: scalar
 * @return value of sigma_c
 */
num_t getSigmaC(TriMesh &mesh, vector<TriMesh::Point> &face_centroids, num_t sigma_c_scalar);

/**
 * @brief getRadius: computation of radius for radius-based face neighborhood, based on the average of face centroid distances (only between adjacent faces) and multiplied by a scalar
 * @param mesh: input mesh
 * @param scalar
 * @return radius value
 */
num_t getRadius(TriMesh &mesh, num_t scalar);

/** @} */ // end of group1



/** @defgroup isotropicMeshSmoothingAlgorithms Isotropic mesh denoising algorithms
 *  This group contains isotropic mesh denoising algorithms
 *  @{
 */

/////////////////////////////////////////////////
/// Uniform Laplacian smoothing
/////////////////////////////////////////////////

TriMesh uniformLaplacian(TriMesh & _mesh, int iteration_number, num_t scale);

TriMesh uniformLaplacian(TriMesh & _mesh, int iteration_number, num_t scale, vector<size_t> & vertex_ids);


/////////////////////////////////////////////////
/// HC Laplacian smoothing (Vollmer et al.)
/////////////////////////////////////////////////

TriMesh HCLaplacian(TriMesh & _mesh,int iteration_number,num_t alpha,num_t beta);

TriMesh HCLaplacian(TriMesh & _mesh, int iteration_number, num_t alpha, num_t beta, vector<size_t> & vertex_ids);

/** @} */


/** @defgroup anisotropicMeshSmoothingAlgorithms Anisotropic mesh denoising algorithms
 *  This group contains anisotropic mesh denoising algorithms
 *  @{
 */


/////////////////////////////////////////////////
/// Bilateral normal filtering for mesh denoising (Zheng et al.)
/////////////////////////////////////////////////

void updateFilteredNormals(TriMesh &mesh, int normal_iteration_number, num_t sigma_c_scalar, num_t sigma_s, vector<TriMesh::Normal> &filtered_normals);

TriMesh bilateralNormal(TriMesh &_mesh, int normal_iteration_number, int vertex_iteration_number, num_t sigma_c_scalar,  num_t sigma_s );

/////////////////////////////////////////////////
/// Guided mesh normal filtering (Zhang et al.)
/////////////////////////////////////////////////

void getVertexBasedFaceNeighbor(TriMesh &mesh, TriMesh::FaceHandle fh, vector<TriMesh::FaceHandle> &face_neighbor);

void getAllFaceNeighborGMNF(TriMesh &mesh, FaceNeighborType face_neighbor_type, num_t radius, bool include_central_face,
	vector<vector<TriMesh::FaceHandle> > &all_face_neighbor);

void getAllGuidedNeighborGMNF(TriMesh &mesh, vector<vector<TriMesh::FaceHandle> > &all_guided_neighbor);

void getFaceNeighborInnerEdge(TriMesh &mesh, vector<TriMesh::FaceHandle> &face_neighbor, vector<TriMesh::EdgeHandle> &inner_edge);

void getRangeAndMeanNormal(TriMesh &mesh, vector<vector<TriMesh::FaceHandle> > &all_guided_neighbor,
	vector<num_t> &face_areas, vector<TriMesh::Normal> &normals,
	vector<pair<num_t, TriMesh::Normal> > &range_and_mean_normal);

void getGuidedNormals(TriMesh &mesh, vector<vector<TriMesh::FaceHandle> > &all_guided_neighbor,
	vector<num_t> &face_areas, vector<TriMesh::Normal> &normals,
	vector<pair<num_t, TriMesh::Normal> > range_and_mean_normal,
	vector<TriMesh::Normal> &guided_normals);


void updateFilteredNormalsGuided(TriMesh &mesh, vector<TriMesh::Normal> &filtered_normals, num_t radius_scalar,
	num_t sigma_c_scalar, int normal_iteration_number, num_t sigma_s, int vertex_iteration_number);

TriMesh guided(TriMesh _mesh, int normal_iteration_number, int vertex_iteration_number, num_t sigma_c_scalar, num_t sigma_s, num_t radius_scalar);


/** @} */


#endif // DENOISING_H
