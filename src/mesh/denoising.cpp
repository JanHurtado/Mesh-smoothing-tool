#include "denoising.h"

/////////////////////////////////////////////////
/// Auxiliar functions
/////////////////////////////////////////////////

bool hasIND(TriMesh::Point & p)
{
	if (p[0] != p[0] || p[1] != p[1] || p[2] != p[2])
		return true;
	else return false;
}

void updateVertexPositions(TriMesh &mesh, vector<TriMesh::Normal> &filtered_normals, int iteration_number, bool fixed_boundary)
{
    vector<TriMesh::Point> new_points(mesh.n_vertices());
    vector<TriMesh::Point> centroids;

    for(int iter = 0; iter < iteration_number; iter++)
    {
        getAllFaceCentroids(mesh, centroids);
        for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
        {
            TriMesh::Point p = mesh.point(*v_it);
            if(fixed_boundary && mesh.is_boundary(*v_it))
            {
                new_points.at(v_it->idx()) = p;
            }
            else
            {
                num_t face_num = 0.0f;
                TriMesh::Point temp_point(0.0f, 0.0f, 0.0f);
                for(TriMesh::VertexFaceIter vf_it = mesh.vf_iter(*v_it); vf_it.is_valid(); vf_it++)
                {
                    TriMesh::Normal temp_normal = filtered_normals[vf_it->idx()];
                    TriMesh::Point temp_centroid = centroids[vf_it->idx()];
                    temp_point += temp_normal * (temp_normal | (temp_centroid - p));
                    face_num++;
                }
                p += temp_point / face_num;
                if (!hasIND(p))
                {
                    new_points.at(v_it->idx()) = p;
                }
                else new_points.at(v_it->idx()) = p;
            }
        }
        for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
            mesh.set_point(*v_it, new_points[v_it->idx()]);
    }
}

num_t getSigmaC(TriMesh &mesh, vector<TriMesh::Point> &face_centroids, num_t sigma_c_scalar)
{
    num_t sigma_c = 0.0f;
    num_t num = 0.0f;
    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        TriMesh::Point ci = face_centroids[f_it->idx()];
        for(TriMesh::FaceFaceIter ff_it = mesh.ff_iter(*f_it); ff_it.is_valid(); ff_it++)
        {
            TriMesh::Point cj = face_centroids[ff_it->idx()];
            sigma_c += (ci - cj).length();
            num++;
        }
    }
    sigma_c *= sigma_c_scalar / num;
    return sigma_c;
}

num_t getRadius(TriMesh &mesh , num_t scalar )
{
    vector<TriMesh::Point> centroids;
    getAllFaceCentroids(mesh, centroids);

    num_t radius = 0.0f;
    num_t num = 0.0f;
    for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
    {
        TriMesh::Point fi = centroids[f_it->idx()];
        for(TriMesh::FaceFaceIter ff_it = mesh.ff_iter(*f_it); ff_it.is_valid(); ff_it++)
        {
            TriMesh::Point fj = centroids[ff_it->idx()];
            radius += (fj - fi).length();
            num++;
        }
    }
    return radius * scalar / num;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
/// Isotropic mesh denoising algorithms
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Uniform Laplacian smoothing
/////////////////////////////////////////////////

TriMesh uniformLaplacian(TriMesh & _mesh, int iteration_number, num_t scale)
{
	TriMesh mesh = _mesh;
	vector<TriMesh::Point> displacement_points;
	displacement_points.resize(mesh.n_vertices());
	for (int iter = 0; iter < iteration_number; iter++)
	{
		for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
		{
			vector<TriMesh::VertexHandle> vertex_neighbors;
			getVertexNeighbors(_mesh, *v_it, 1, vertex_neighbors);
			num_t weight = 1.0f / ((num_t)vertex_neighbors.size());
			TriMesh::Point sum(0.0f, 0.0f, 0.0f);
			for (int i = 0; i<(int)(vertex_neighbors.size()); i++)
				sum = sum + ((mesh.point(vertex_neighbors[i]) - mesh.point(*v_it))*weight);
			displacement_points[v_it->idx()] = sum;
		}
		for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
			mesh.set_point(*v_it, displacement_points[v_it->idx()] * scale + mesh.point(*v_it));
	}
	mesh.request_face_normals();
	mesh.request_vertex_normals();
	mesh.update_normals();
	return mesh;
}

TriMesh uniformLaplacian(TriMesh & _mesh, int iteration_number, num_t scale, vector<size_t> & vertex_ids)
{
	TriMesh mesh = _mesh;
	vector<TriMesh::Point> displacement_points;
	displacement_points.resize(mesh.n_vertices());
	for (int iter = 0; iter < iteration_number; iter++)
	{
		for (size_t i = 0; i < vertex_ids.size(); i++)
		{
			TriMesh::VertexHandle vh((int)vertex_ids[i]);
			vector<TriMesh::VertexHandle> vertex_neighbors;
			getVertexNeighbors(_mesh, vh, 1, vertex_neighbors);
			num_t weight = 1.0f / ((num_t)vertex_neighbors.size());
			TriMesh::Point sum(0.0f, 0.0f, 0.0f);
			for (int i = 0; i<(int)(vertex_neighbors.size()); i++)
				sum = sum + ((mesh.point(vertex_neighbors[i]) - mesh.point(vh))*weight);
			displacement_points[vh.idx()] = sum;
		}
		for (size_t i = 0; i < vertex_ids.size(); i++)
		{
			TriMesh::VertexHandle vh((int)vertex_ids[i]);
			mesh.set_point(vh, displacement_points[vh.idx()] * scale + mesh.point(vh));
		}
	}
	mesh.request_face_normals();
	mesh.request_vertex_normals();
	mesh.update_normals();
	return mesh;
}

/////////////////////////////////////////////////
/// HC Laplacian smoothing (Vollmer et al.)
/////////////////////////////////////////////////
TriMesh HCLaplacian(TriMesh & _mesh,int iteration_number,num_t alpha,num_t beta)
{
    TriMesh mesh = _mesh;
    vector<TriMesh::Point> original_points;
    getAllPoints(mesh,original_points);
    TriMesh temp_mesh_p;
    vector<TriMesh::Point> temp_points_p(original_points);
	vector<TriMesh::Point> uniform_displacement_points;
	uniform_displacement_points.resize(mesh.n_vertices());
    for(int iter = 0; iter < iteration_number; iter++)
    {
        vector<TriMesh::Point> temp_points_q(temp_points_p);
		for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
		{
			vector<TriMesh::VertexHandle> vertex_neighbors;
			getVertexNeighbors(_mesh, *v_it, 1, vertex_neighbors);
			if (vertex_neighbors.size()>0)
			{
				num_t weight = 1.0f / ((num_t)vertex_neighbors.size());
				TriMesh::Point sum(0.0f, 0.0f, 0.0f);
				for (int i = 0; i<(int)(vertex_neighbors.size()); i++)
					sum = sum + ((mesh.point(vertex_neighbors[i]) - mesh.point(*v_it))*weight);
				uniform_displacement_points[v_it->idx()] = sum;
				temp_points_p[v_it->idx()] += sum;
			}
			else
				uniform_displacement_points[v_it->idx()] = TriMesh::Point(0,0,0);
		}
		for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
			temp_points_p[v_it->idx()]=uniform_displacement_points[v_it->idx()] + mesh.point(*v_it);
        vector<TriMesh::Point> temp_points_b(temp_points_p.size());
        for(size_t i = 0;i<temp_points_p.size();i++)
            temp_points_b[i]=temp_points_p[i]-(original_points[i]*alpha+(1.0f-alpha)*temp_points_q[i]);
        for(TriMesh::VertexIter v_it = mesh.vertices_begin();v_it!=mesh.vertices_end();v_it++)
        {
            TriMesh::Point t_sum(0,0,0);
            num_t num = 0;
            for(TriMesh::VertexVertexIter vv_it = mesh.vv_iter(*v_it);vv_it.is_valid();vv_it++,num+=1.0f)
                t_sum+=temp_points_b[vv_it->idx()];

			TriMesh::Point disp = (beta*temp_points_b[v_it->idx()] + ((1.0f - beta) / num)*t_sum);
			if (!hasIND(disp))
				temp_points_p[v_it->idx()]=temp_points_p[v_it->idx()]-disp;
        }
    }
    for(TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
        mesh.set_point(*v_it, temp_points_p[v_it->idx()]);
	mesh.request_face_normals();
	mesh.request_vertex_normals();
	mesh.update_normals();
    return mesh;
}

TriMesh HCLaplacian(TriMesh & _mesh, int iteration_number, num_t alpha, num_t beta, vector<size_t> & vertex_ids)
{
	TriMesh mesh = _mesh;
	vector<TriMesh::Point> original_points;
	getAllPoints(mesh, original_points);
	TriMesh temp_mesh_p;
	vector<TriMesh::Point> temp_points_p(original_points);
	vector<TriMesh::Point> uniform_displacement_points;
	uniform_displacement_points.resize(mesh.n_vertices());
	for (int iter = 0; iter < iteration_number; iter++)
	{
		vector<TriMesh::Point> temp_points_q(temp_points_p);
		for (size_t i = 0; i < vertex_ids.size(); i++)
		{
			TriMesh::VertexHandle vh((int)vertex_ids[i]);
			vector<TriMesh::VertexHandle> vertex_neighbors;
			getVertexNeighbors(_mesh, vh, 1, vertex_neighbors);
			if (vertex_neighbors.size()>0)
			{
				num_t weight = 1.0f / ((num_t)vertex_neighbors.size());
				TriMesh::Point sum(0.0f, 0.0f, 0.0f);
				for (int i = 0; i<(int)(vertex_neighbors.size()); i++)
					sum = sum + ((mesh.point(vertex_neighbors[i]) - mesh.point(vh))*weight);
				uniform_displacement_points[vh.idx()] = sum;
				temp_points_p[vh.idx()] += sum;
			}
			else
				uniform_displacement_points[vh.idx()] = TriMesh::Point(0, 0, 0);
		}
		for (size_t i = 0; i < vertex_ids.size(); i++)
		{
			TriMesh::VertexHandle vh((int)vertex_ids[i]);
			temp_points_p[vh.idx()] = uniform_displacement_points[vh.idx()] + mesh.point(vh);
		}
		vector<TriMesh::Point> temp_points_b(temp_points_p.size());
		for (size_t i = 0; i<temp_points_p.size(); i++)
			temp_points_b[i] = temp_points_p[i] - (original_points[i] * alpha + (1.0f - alpha)*temp_points_q[i]);
		for (size_t i = 0; i < vertex_ids.size(); i++)
		{
			TriMesh::VertexHandle vh((int)vertex_ids[i]);
			TriMesh::Point t_sum(0, 0, 0);
			num_t num = 0;
			for (TriMesh::VertexVertexIter vv_it = mesh.vv_iter(vh); vv_it.is_valid(); vv_it++, num += 1.0f)
				t_sum += temp_points_b[vv_it->idx()];

			TriMesh::Point disp = (beta*temp_points_b[vh.idx()] + ((1.0f - beta) / num)*t_sum);
			if (!hasIND(disp))
				temp_points_p[vh.idx()] = temp_points_p[vh.idx()] - disp;
		}
	}
	for (size_t i = 0; i < vertex_ids.size(); i++)
	{
		TriMesh::VertexHandle vh((int)vertex_ids[i]);
		mesh.set_point(vh, temp_points_p[vh.idx()]);
	}
	mesh.request_face_normals();
	mesh.request_vertex_normals();
	mesh.update_normals();
	return mesh;
}

/////////////////////////////////////////////////
/// Bilateral normal filtering for mesh denoising (Zheng et al.)
/////////////////////////////////////////////////

void updateFilteredNormals(TriMesh &mesh, int normal_iteration_number, num_t sigma_c_scalar, num_t sigma_s, vector<TriMesh::Normal> &filtered_normals)
{
    filtered_normals.resize(mesh.n_faces());

    vector< vector<TriMesh::FaceHandle> > all_face_neighbors;
    getAllFaceNeighbors_VertexBased(mesh,0,all_face_neighbors);
    vector<TriMesh::Normal> previous_normals;
    getAllFaceNormals(mesh, previous_normals);
    vector<num_t> face_areas;
    getAllFaceAreas(mesh, face_areas);
    vector<TriMesh::Point> face_centroids;
    getAllFaceCentroids(mesh, face_centroids);

    num_t sigma_c = getSigmaC(mesh, face_centroids, sigma_c_scalar);

    for(int it = 0; it < normal_iteration_number; it++)
    {
        for(TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
        {
            int face_idx = f_it->idx();
			TriMesh::Normal ni = previous_normals[face_idx];
			TriMesh::Point ci = face_centroids[face_idx];
			vector<TriMesh::FaceHandle> face_neighbors = all_face_neighbors[face_idx];
            TriMesh::Normal temp_normal(0.0, 0.0, 0.0);
            num_t weight_sum = 0.0;
			for (int i = 0; i < (int)face_neighbors.size(); i++)
            {
                int neighbor_idx = face_neighbors[i].idx();
				TriMesh::Normal nj = previous_normals[neighbor_idx];
				TriMesh::Point cj = face_centroids[neighbor_idx];

                num_t spatial_distance = (ci - cj).length();
                num_t spatial_weight = GaussianWeight(spatial_distance,sigma_c);
                num_t range_distance = (ni - nj).length();
                num_t range_weight = GaussianWeight(range_distance,sigma_s);

				num_t weight = face_areas[neighbor_idx] * spatial_weight * range_weight;
                weight_sum += weight;
                temp_normal += nj * weight;
            }
            temp_normal /= weight_sum;
            temp_normal.normalize_cond();
			filtered_normals[face_idx] = temp_normal;
        }
        previous_normals = filtered_normals;
    }
}

TriMesh bilateralNormal(TriMesh &_mesh, int normal_iteration_number, int vertex_iteration_number, num_t sigma_c_scalar,  num_t sigma_s )
{
    TriMesh mesh = _mesh;
    vector<TriMesh::Normal> filtered_normals;
    updateFilteredNormals(mesh, normal_iteration_number, sigma_c_scalar, sigma_s, filtered_normals);
    updateVertexPositions(mesh, filtered_normals, vertex_iteration_number, true);
	mesh.request_face_normals();
	mesh.request_vertex_normals();
	mesh.update_normals();
	//mesh.update_face_normals();
    return mesh;
}

