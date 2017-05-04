#include "util.h"

num_t getArea (TriMesh & mesh)
{
    vector<num_t> areas;
    getAllFaceAreas(mesh,areas);
    num_t sum = 0;
    for (size_t i=0;i<areas.size();i++)
        sum+=areas[i];
    return sum;
}

num_t getVolume ( TriMesh & mesh ) {

    typedef TriMesh::HalfedgeHandle hh_t;
    typedef TriMesh::VertexHandle vh_t;
    typedef TriMesh::Point p_t;

    num_t volume = 0.0f;
    num_t three_inv = 1.0f / 3.0f;
    TriMesh::FaceIter f_it;
    for (f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
    {
        hh_t hh = mesh.halfedge_handle(*f_it);
        hh_t hhn = mesh.next_halfedge_handle(hh);

        vh_t v0(mesh.from_vertex_handle(hh));
        vh_t v1(mesh.to_vertex_handle(hh));
        vh_t v2(mesh.to_vertex_handle(hhn));

        p_t p0(mesh.point(v0));
        p_t p1(mesh.point(v1));
        p_t p2(mesh.point(v2));

        p_t g = (p0 + p1 + p2) * three_inv;
        p_t n = (p1 - p0) % (p2 - p0);
        volume += (g|n);
    }
    volume *= 1.0f / 6.0f;
    return volume;
}

num_t getAverageEdgeLength(TriMesh &mesh)
{
    num_t average_edge_length = 0.0;
    for(TriMesh::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); e_it++)
        average_edge_length += mesh.calc_edge_length(*e_it);
    num_t edgeNum = (num_t)mesh.n_edges();
    average_edge_length /= edgeNum;

    return average_edge_length;
}

void getAllFaceAreas(TriMesh &mesh, vector<num_t> &areas)
{
    areas.resize(mesh.n_faces());

    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        vector<TriMesh::Point> point;
        point.resize(3); int index = 0;
        for(TriMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
        {
            point[index] = mesh.point(*fv_it);
            index++;
        }
        TriMesh::Point edge1 = point[1] - point[0];
        TriMesh::Point edge2 = point[1] - point[2];
        num_t S = 0.5f * (edge1 % edge2).length();
        areas[(*f_it).idx()] = S;
    }
}

void getAllFaceCentroids(TriMesh &mesh, vector<TriMesh::Point> &centroids)
{
    centroids.resize(mesh.n_faces(), TriMesh::Point(0.0, 0.0, 0.0));
    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        TriMesh::Point pt = mesh.calc_face_centroid(*f_it);
        centroids[(*f_it).idx()] = pt;
    }
}

void getAllFaceNormals(TriMesh &mesh, vector<TriMesh::Normal> &normals)
{
    mesh.request_face_normals();
    mesh.update_face_normals();

    normals.resize(mesh.n_faces());
    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        TriMesh::Normal n = mesh.normal(*f_it);
        normals[f_it->idx()] = n;
    }
}

void getFaceVertexAngle(TriMesh &mesh,TriMesh::FaceHandle fh,TriMesh::VertexHandle vh, num_t & angle)
{
    TriMesh::FaceVertexIter verts = mesh.fv_iter(fh);
    TriMesh::VertexHandle vh1,vh2;
    if (vh == *verts)
    {
        vh1 = *(++verts);
        vh2 = *(++verts);
    }
    else
    {
        vh1 = *verts;
        vh2 = (vh == *(++verts)) ? *(++verts) : *verts;
    }
    TriMesh::Point v1 = mesh.point(vh) - mesh.point(vh1);
    TriMesh::Point v2 = mesh.point(vh) - mesh.point(vh2);
    TriMesh::Point v3 = (mesh.point(vh1) - mesh.point(vh2));
    v1.normalize_cond(); v2.normalize_cond();
    angle = acos(v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

num_t getVertexArea(TriMesh & mesh,TriMesh::VertexHandle vh,vector<num_t> & areas)
{
    num_t area_sum = 0.0f;
    for(TriMesh::VertexFaceIter vf_iter = mesh.vf_iter(vh);vf_iter.is_valid();vf_iter++)
        area_sum+=areas[vf_iter->idx()];
    return area_sum*(1.0f/3.0f);
}

void getAllVertexAreas(TriMesh & mesh,vector<num_t> & areas)
{
    areas.resize(mesh.n_vertices());
    vector<num_t> face_areas;
    getAllFaceAreas(mesh,face_areas);
    for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
        areas[v_it->idx()]=getVertexArea(mesh,*v_it,face_areas);
}

void getAllPoints(TriMesh &mesh, vector<TriMesh::Point> & points)
{
    points.resize(mesh.n_vertices());
    for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
    {
        TriMesh::Point p = mesh.point(*v_it);
        points[v_it->idx()] = p;
    }
}
