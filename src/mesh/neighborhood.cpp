#include "neighborhood.h"

/////////////////////////////////////////////////
/// Vertex neighborhood
/////////////////////////////////////////////////

void getVertexNeighbors(TriMesh &mesh, TriMesh::VertexHandle vh, int k,
                       vector<TriMesh::VertexHandle> &vertex_neighbors)
{
    vertex_neighbors.clear();
    vector<bool> mark(mesh.n_vertices(),false);
    queue<TriMesh::VertexHandle> queue_vertex_handle;
    queue<int> queue_depth_handle;
    mark[vh.idx()] = true;
    queue_vertex_handle.push(vh);
    queue_depth_handle.push(0);
    TriMesh::Point ci = mesh.point(vh);
    while(!queue_vertex_handle.empty())
    {
        TriMesh::VertexHandle vh = queue_vertex_handle.front();
        int current_depth = queue_depth_handle.front();
        vertex_neighbors.push_back(vh);
        queue_vertex_handle.pop();
        queue_depth_handle.pop();
        for(TriMesh::VertexVertexIter vv_it = mesh.vv_iter(vh); vv_it.is_valid(); ++vv_it)
        {
            TriMesh::VertexHandle vh_neighbor = *vv_it;
            if(mark[vh_neighbor.idx()] == false)
            {
                int new_depth = current_depth + 1;
                if(new_depth <= k)
                {
                    queue_vertex_handle.push(vh_neighbor);
                    queue_depth_handle.push(new_depth);
                }
                mark[vh_neighbor.idx()] = true;
            }
        }
    }
}

void getAdaptiveVertexNeighbors(TriMesh &mesh, TriMesh::VertexHandle vh, num_t radius,
                                                       vector<TriMesh::VertexHandle> &vertex_neighbors)
{
    vertex_neighbors.clear();
    vector<bool> mark(mesh.n_vertices(),false);
    queue<TriMesh::VertexHandle> queue_vertex_handle;
    mark[vh.idx()] = true;
    queue_vertex_handle.push(vh);
    TriMesh::Point ci = mesh.point(vh);

    while(!queue_vertex_handle.empty())
    {
        TriMesh::VertexHandle vh = queue_vertex_handle.front();
        vertex_neighbors.push_back(vh);
        queue_vertex_handle.pop();
        for(TriMesh::VertexVertexIter vv_it = mesh.vv_iter(vh); vv_it.is_valid(); ++vv_it)
        {
            TriMesh::VertexHandle vh_neighbor = *vv_it;
            if(mark[vh_neighbor.idx()] == false)
            {
                TriMesh::Point cj = mesh.point(vh_neighbor);
                num_t length = (cj - ci).length();
                if(length <= radius)
                    queue_vertex_handle.push(vh_neighbor);
                mark[vh_neighbor.idx()] = true;
            }
        }
    }
}

void getAllVertexNeighbors(TriMesh &mesh, int k,vector<vector<TriMesh::VertexHandle> > &all_vertex_neighbors)
{
    all_vertex_neighbors.resize(mesh.n_vertices());
    for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
    {
        vector<TriMesh::VertexHandle> vertex_neighbor;
        getVertexNeighbors(mesh, *v_it, k, vertex_neighbor);
        all_vertex_neighbors[v_it->idx()] = vertex_neighbor;
    }
}

void getAllAdaptiveVertexNeighbors(TriMesh &mesh, num_t radius,
                               vector<vector<TriMesh::VertexHandle> > &all_vertex_neighbors)
{
    all_vertex_neighbors.resize(mesh.n_vertices());
    for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
    {
        vector<TriMesh::VertexHandle> vertex_neighbor;
        getAdaptiveVertexNeighbors(mesh, *v_it, radius, vertex_neighbor);
        all_vertex_neighbors[v_it->idx()] = vertex_neighbor;
    }
}

/////////////////////////////////////////////////
/// Face neighborhood
/////////////////////////////////////////////////

void getFaceNeighbors_EdgeBased(TriMesh &mesh, TriMesh::FaceHandle fh, vector<TriMesh::FaceHandle> &face_neighbors)
{
    face_neighbors.clear();
    for(TriMesh::FaceFaceIter ff_it = mesh.ff_iter(fh); ff_it.is_valid(); ff_it++)
        face_neighbors.push_back(*ff_it);
}

void getFaceNeighbors_VertexBased(TriMesh &mesh, TriMesh::FaceHandle fh, vector<TriMesh::FaceHandle> &face_neighbors)
{
    face_neighbors.clear();
    set<int> neighbor_face_index; neighbor_face_index.clear();
    for(TriMesh::FaceVertexIter fv_it = mesh.fv_begin(fh); fv_it.is_valid(); fv_it++)
    {
        for(TriMesh::VertexFaceIter vf_it = mesh.vf_iter(*fv_it); vf_it.is_valid(); vf_it++)
        {
            if((*vf_it) != fh)
                neighbor_face_index.insert(vf_it->idx());
        }
    }
    for(set<int>::iterator iter = neighbor_face_index.begin(); iter != neighbor_face_index.end(); ++ iter)
    {
        face_neighbors.push_back(TriMesh::FaceHandle(*iter));
    }
}

void getFaceNeighbors_RadiusBased(TriMesh &mesh, TriMesh::FaceHandle fh, num_t radius, vector<TriMesh::FaceHandle> &face_neighbors)
{
    TriMesh::Point ci = mesh.calc_face_centroid(fh);
    vector<bool> flag((int)mesh.n_faces(), false);

    face_neighbors.clear();
    flag[fh.idx()] = true;
    queue<TriMesh::FaceHandle> queue_face_handle;
    queue_face_handle.push(fh);

    vector<TriMesh::FaceHandle> temp_face_neighbors;
    while(!queue_face_handle.empty())
    {
        TriMesh::FaceHandle temp_face_handle_queue = queue_face_handle.front();
        if(temp_face_handle_queue != fh)
            face_neighbors.push_back(temp_face_handle_queue);
        queue_face_handle.pop();
        getFaceNeighbors_VertexBased(mesh, temp_face_handle_queue, temp_face_neighbors);
        for(int i = 0; i < (int)temp_face_neighbors.size(); i++)
        {
            TriMesh::FaceHandle temp_face_handle = temp_face_neighbors[i];
            if(!flag[temp_face_handle.idx()])
            {
                TriMesh::Point cj = mesh.calc_face_centroid(temp_face_handle);
                num_t distance = (ci - cj).length();
                if(distance <= radius)
                    queue_face_handle.push(temp_face_handle);
                flag[temp_face_handle.idx()] = true;
            }
        }
    }
}

void getAllFaceNeighbors_EdgeBased(TriMesh &mesh, bool include_target_face, vector<vector<TriMesh::FaceHandle> > &all_face_neighbors)
{
    all_face_neighbors.resize(mesh.n_faces());
    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        vector<TriMesh::FaceHandle> face_neighbors;
        getFaceNeighbors_EdgeBased(mesh, *f_it, face_neighbors);
        if(include_target_face) face_neighbors.push_back(*f_it); //include target face
        all_face_neighbors[f_it->idx()] = face_neighbors;
    }
}

void getAllFaceNeighbors_VertexBased(TriMesh &mesh, bool include_target_face, vector<vector<TriMesh::FaceHandle> > &all_face_neighbors)
{
    all_face_neighbors.resize(mesh.n_faces());
    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        vector<TriMesh::FaceHandle> face_neighbors;
        getFaceNeighbors_VertexBased(mesh, *f_it, face_neighbors);
        if(include_target_face) face_neighbors.push_back(*f_it); //include target face
        all_face_neighbors[f_it->idx()] = face_neighbors;
    }
}