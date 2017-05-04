#ifndef UTIL_H
#define UTIL_H

#include "mesh.h"
#include <vector>
#include <queue>

using namespace std;

#define PI 3.14159265359

num_t getArea (TriMesh & mesh);
num_t getVolume ( TriMesh & mesh );
num_t getAverageEdgeLength(TriMesh &mesh);
void getAllFaceAreas(TriMesh &mesh, vector<num_t> &areas);
void getAllFaceCentroids(TriMesh &mesh, vector<TriMesh::Point> &centroids);
void getAllFaceNormals(TriMesh &mesh, vector<TriMesh::Normal> &normals);
void getFaceVertexAngle(TriMesh &mesh,TriMesh::FaceHandle fh,TriMesh::VertexHandle vh, num_t & angle);
num_t getVertexArea(TriMesh & mesh,TriMesh::VertexHandle vh,vector<num_t> & areas);
void getAllVertexAreas(TriMesh & mesh, vector<num_t> & areas);
void getAllPoints(TriMesh &mesh, vector<TriMesh::Point> & points);
inline num_t GaussianWeight(num_t distance, num_t sigma)
{
    return static_cast<num_t>(exp( -0.5 * distance * distance / (sigma * sigma)));
}

inline num_t NormalDistance(const TriMesh::Normal &n1, const TriMesh::Normal &n2)
{
    return (n1 - n2).length();
}

#endif // UTIL_H
