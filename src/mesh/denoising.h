#ifndef DENOISING_H
#define DENOISING_H

#include "neighborhood.h"
#include "util.h"



/** @addtogroup mesh_processing
  * @brief Mesh denoising functions
  *
  * @{
  */

/**
 * @brief The FaceNeighborType enum - determines the face neighborhood features
 */
enum FaceNeighborType{
    kVertexBased, /**< vertex based face neighborhood */
    kEdgeBased, /**< edge based face neighborhood */
    kRadiusBased /**< radius based face neighborhood */
};

/**
 * @brief updateVertexPositions - computes new vertex positions adapted to an input normal field.
 * @param mesh input mesh.
 * @param filtered_normals input normal field.
 * @param iteration_number number of iterations of the optimization algorithm (gradient descent approach).
 * @param fixed_boundary determines if a boundary vertex will be updated
 */
void updateVertexPositions(TriMesh &mesh, vector<TriMesh::Normal> &filtered_normals, int iteration_number, bool fixed_boundary);

/**
 * @brief getSigmaC - sigma_c computation based on the average of face centroid distances (only between adjacent faces), and multiplied by a scalar
 * @param mesh input mesh
 * @param face_centroids precomputed face centroids
 * @param sigma_c_scalar scalar
 * @return value of sigma_c
 */
num_t getSigmaC(TriMesh &mesh, vector<TriMesh::Point> &face_centroids, num_t sigma_c_scalar);

/**
 * @brief getRadius - computation of radius for radius-based face neighborhood, based on the average of face centroid distances (only between adjacent faces) and multiplied by a scalar
 * @param mesh input mesh
 * @param scalar
 * @return radius value
 */
num_t getRadius(TriMesh &mesh, num_t scalar);

/////////////////////////////////////////////////
/// Uniform Laplacian smoothing
/////////////////////////////////////////////////

/**
 * @brief uniformLaplacian - denoise the input mesh using Uniform Laplacian Smoothing Algorithm
 * @param _mesh input mesh
 * @param iteration_number number of iterations
 * @param scale displacement influence for each iteration
 * @return denoised mesh
 */
TriMesh uniformLaplacian(TriMesh & _mesh, int iteration_number, num_t scale);

/**
 * @brief uniformLaplacian - denoise a subset of vertices of an input mesh using Uniform Laplacian Smoothing Algorithm
 * @param _mesh input mesh
 * @param iteration_number number of iterations
 * @param scale displacement influence for each iteration
 * @param vertex_ids subset of vertex IDs to be denoised
 * @return denoised mesh (taking into account only the subset of IDs)
 */
TriMesh uniformLaplacian(TriMesh & _mesh, int iteration_number, num_t scale, vector<size_t> & vertex_ids);

/////////////////////////////////////////////////
/// HC Laplacian smoothing (Vollmer et al.)
/////////////////////////////////////////////////

/**
 * @brief HCLaplacian - denoise the input mesh using HC Laplacian Smoothing Algorithm
 * @param _mesh input mesh
 * @param iteration_number number of iterations
 * @param alpha alpha defined in HC Laplacian Smoothing paper
 * @param beta beta defined in HC Laplacian Smoothing paper
 * @return denoised mesh
 */
TriMesh HCLaplacian(TriMesh & _mesh,int iteration_number,num_t alpha,num_t beta);

/**
 * @brief HCLaplacian - denoise a subset of vertices of an input mesh using HC Laplacian Smoothing Algorithm
 * @param _mesh input mesh
 * @param iteration_number number of iterations
 * @param alpha alpha defined in HC Laplacian Smoothing paper
 * @param beta beta defined in HC Laplacian Smoothing paper
 * @param vertex_ids subset of vertex IDs to be denoised
 * @return denoised mesh (taking into account only the subset of IDs)
 */
TriMesh HCLaplacian(TriMesh & _mesh, int iteration_number, num_t alpha, num_t beta, vector<size_t> & vertex_ids);

/////////////////////////////////////////////////
/// Bilateral normal filtering for mesh denoising (Zheng et al.)
/////////////////////////////////////////////////

/**
 * @brief updateFilteredNormals - bilateral filtering of the normal field (face based) of a mesh
 * @param mesh input mesh
 * @param normal_iteration_number number of iterations
 * @param sigma_c_scalar bilateral filtering influence regarding a \f$ sigma_c \f$ based on average edge length (spatial distance influence)
 * @param sigma_s bilateral filtering parameter \f$ sigma_s \f$
 * @param filtered_normals output vector of filtered face normals
 */
void updateFilteredNormals(TriMesh &mesh, int normal_iteration_number, num_t sigma_c_scalar, num_t sigma_s, vector<TriMesh::Normal> &filtered_normals);

/**
 * @brief bilateralNormal - Denoise the input mesh using Bilateral Normal Filtering Algorithm
 * @param _mesh input mesh
 * @param normal_iteration_number number of iterations for normal field filtering
 * @param vertex_iteration_number number of iterations for vertex updating
 * @param sigma_c_scalar bilateral normal field filtering influence regarding a \f$ sigma_c \f$ based on average edge length (spatial distance influence)
 * @param sigma_s bilateral normal field filtering parameter \f$ sigma_s \f$
 * @return denoised mesh
 */
TriMesh bilateralNormal(TriMesh &_mesh, int normal_iteration_number, int vertex_iteration_number, num_t sigma_c_scalar,  num_t sigma_s );

/////////////////////////////////////////////////
/// Guided mesh normal filtering (Zhang et al.)
/////////////////////////////////////////////////

/**
 * @brief getAllFaceNeighborsGMNF - neighborhood computation for all mesh faces (for bilateral filtering)
 * @param mesh input mesh
 * @param face_neighbor_type type of neighborhood to be considered
 * @param radius radius for radius based face neighborhood
 * @param include_central_face if the same face is considered as a neighbor
 * @param all_face_neighbors output vector containing neighboring faces IDs for each face of the mesh
 */
void getAllFaceNeighborsGMNF(TriMesh &mesh, FaceNeighborType face_neighbor_type, num_t radius, bool include_central_face,
    vector<vector<TriMesh::FaceHandle> > &all_face_neighbors);

/**
 * @brief getAllGuidedNeighborsGMNF - neighborhood computation for guidance signal
 * @param mesh input mesh
 * @param all_guided_neighbors output vector containing neighboring faces IDs for each face of the mesh
 */
void getAllGuidedNeighborsGMNF(TriMesh &mesh, vector<vector<TriMesh::FaceHandle> > &all_guided_neighbors);

/**
 * @brief getFaceNeighborsInnerEdges - get all inner edges of a face neighborhood
 * @param mesh input mesh
 * @param face_neighbors face neighborhood
 * @param inner_edges output vector containing all inner edges
 */
void getFaceNeighborsInnerEdges(TriMesh &mesh, vector<TriMesh::FaceHandle> &face_neighbors, vector<TriMesh::EdgeHandle> &inner_edges);

/**
 * @brief getConsistenciesAndMeanNormals - consistency and mean normal computation for all patches
 * @param mesh input mesh
 * @param all_guided_neighbors all neighborhoods defining the patches
 * @param face_areas all face areas
 * @param normals all face normals
 * @param consistencies_and_mean_normals output vector containing the consistency and mean normal of all patches
 */
void getConsistenciesAndMeanNormals(TriMesh &mesh, vector<vector<TriMesh::FaceHandle> > &all_guided_neighbors,
    vector<num_t> &face_areas, vector<TriMesh::Normal> &normals,
    vector<pair<num_t, TriMesh::Normal> > &consistencies_and_mean_normals);

/**
 * @brief getGuidedNormals - guided normal computation for each face of the mesh
 * @param mesh input mesh
 * @param all_guided_neighbors all neighborhoods defining the patches
 * @param face_areas all face areas
 * @param normals all face normals
 * @param consistencies_and_mean_normals consistencies and mean normals of all patches
 * @param guided_normals output vector containing guided normals for all faces
 */
void getGuidedNormals(TriMesh &mesh, vector<vector<TriMesh::FaceHandle> > &all_guided_neighbors,
    vector<num_t> &face_areas, vector<TriMesh::Normal> &normals,
    vector<pair<num_t, TriMesh::Normal> > consistencies_and_mean_normals,
	vector<TriMesh::Normal> &guided_normals);

/**
 * @brief updateFilteredNormalsGuided - guided bilateral filtering of a normal field (face based) of a mesh
 * @param mesh input mesh
 * @param filtered_normals guided normals for each face
 * @param radius_scalar average radius ratio for face neighborhood computation
 * @param sigma_c_scalar bilateral filtering influence regarding a \f$ sigma_c \f$ based on average edge length (spatial distance influence)
 * @param normal_iteration_number number of iterations for normal field filtering
 * @param sigma_s bilateral filtering parameter \f$ sigma_s \f$
 * @param vertex_iteration_number number of iterations for vertex updating
 */
void updateFilteredNormalsGuided(TriMesh &mesh, vector<TriMesh::Normal> &filtered_normals, num_t radius_scalar,
	num_t sigma_c_scalar, int normal_iteration_number, num_t sigma_s, int vertex_iteration_number);

/**
 * @brief guided - Denoise the input mesh using Guided Mesh Normal Filtering Algorithm
 * @param _mesh input mesh
 * @param normal_iteration_number number of iterations for normal field filtering
 * @param vertex_iteration_number number of iterations for vertex updating
 * @param sigma_c_scalar bilateral normal field filtering influence regarding a \f$ sigma_c \f$ based on average edge length (spatial distance influence)
 * @param sigma_s bilateral normal field filtering parameter \f$ sigma_s \f$
 * @param radius_scalar average radius ratio for face neighborhood computation
 * @return denoised mesh
 */
TriMesh guided(TriMesh _mesh, int normal_iteration_number, int vertex_iteration_number, num_t sigma_c_scalar, num_t sigma_s, num_t radius_scalar);

/** @} */


#endif // DENOISING_H
