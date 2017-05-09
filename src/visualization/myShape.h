#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../mesh/iomesh.h"

/** @addtogroup visualization
  * @brief Shape data.
  *
  * @{
  */

/**
 * @brief The Vertex struct - single vertex with its corresponding attributes
 */
struct Vertex
{
    /**
     * @brief position - vertex position (x,y,z)
     */
    glm::vec3 position;
    /**
     * @brief color - vertex color (r,g,b)
     */
	glm::vec3 color;
    /**
     * @brief normal - vertex normal (nx,ny,nz)
     */
	glm::vec3 normal;
};

/**
 * @brief The ShapeData struct - triangular mesh for OpenGL buffers manipulation
 */
struct ShapeData
{
    /**
     * @brief numVertices - number of vertices of the mesh
     */
    GLuint numVertices;
    /**
     * @brief numIndices - number of indices of all triangles
     */
    GLuint numIndices;

    /**
     * @brief vertices - array containing all vertices
     */
    Vertex* vertices;
    /**
     * @brief indices - array containing all indices
     */
    GLuint* indices;

    /**
     * @brief centroid - shape centroid
     */
	glm::vec3 centroid;

    /**
     * @brief ShapeData - default constructor
     */
    ShapeData():vertices(0), numVertices(0),indices(0), numIndices(0){}
    /**
     * @brief ShapeData - generates shape data using a triangular mesh file supported in OpenMesh library
     * @param fileName - mesh file name
     */
	ShapeData(string fileName)
	{
		loadFromFile(fileName);
	}
    /**
     * @brief loadFromFile - load shape data using a triangular mesh file supported in OpenMesh library
     * @param fileName - mesh file name
     */
	void loadFromFile(string fileName)
	{
		TriMesh _mesh;
		importMesh(_mesh, fileName);
		_mesh.request_face_normals();
		_mesh.request_vertex_normals();
		_mesh.update_normals();
		loadMesh(_mesh);
	}
    /**
     * @brief loadMesh - load shape data from a data structure defined in OpenMesh library
     * @param _mesh - triangular mesh
     */
	void loadMesh(TriMesh & _mesh)
	{
		numVertices = static_cast<GLuint>(_mesh.n_vertices());
		vertices = new Vertex[numVertices];
		numIndices = static_cast<GLuint>(_mesh.n_faces() * 3);
		indices = new GLuint[numIndices];

		GLuint currentIndex = 0;
		TriMesh::Point c(0, 0, 0);
		float n = 0.0f;
		for (TriMesh::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); v_it++)
		{
			TriMesh::Color current_color = _mesh.color(*v_it);
			TriMesh::Point current_point = _mesh.point(*v_it);
			TriMesh::Normal current_normal = _mesh.normal(*v_it);
			c += current_point;
			n++;
			vertices[currentIndex].position = glm::vec3(current_point[0], current_point[1], current_point[2]);
			vertices[currentIndex].color = glm::vec3(0.5f, 0.5f, 0.5f);
			vertices[currentIndex].normal = glm::vec3(current_normal[0], current_normal[1], current_normal[2]);
			currentIndex++;
		}
		c = c / n;
		centroid[0] = c[0];
		centroid[1] = c[1];
		centroid[2] = c[2];
		currentIndex = 0;
		for (TriMesh::FaceIter f_it = _mesh.faces_begin(); f_it != _mesh.faces_end(); f_it++)
		{
			for (TriMesh::FaceVertexIter fv_it = _mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
			{
				indices[currentIndex] = static_cast<GLuint>(fv_it->idx());
				currentIndex++;
			}
		}
	}
    /**
     * @brief loadMeshVertexSelection - load as shape data a subset of vertices (only vertices) from a data structure defined in OpenMesh library
     * @param _mesh - triangular mesh
     * @param selected_vertices - subset of vertices ids of _mesh
     */
	void loadMeshVertexSelection(TriMesh & _mesh,vector<size_t> & selected_vertices)
	{
		numVertices = static_cast<GLuint>(_mesh.n_vertices());
		vertices = new Vertex[numVertices];
		numIndices = 0;
		indices = new GLuint[numIndices];
		TriMesh::Point c(0, 0, 0);
		float n = 0.0f;
		for (size_t i = 0; i < selected_vertices.size(); i++)
		{
			TriMesh::VertexHandle vh((int)selected_vertices[i]);
			TriMesh::Point current_point = _mesh.point(vh);
			TriMesh::Normal current_normal = _mesh.normal(vh);
			c += current_point;
			n++;
			vertices[i].position = glm::vec3(current_point[0], current_point[1], current_point[2]);
			vertices[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices[i].normal = glm::vec3(current_normal[0], current_normal[1], current_normal[2]);
		}
		c = c / n;
		centroid[0] = c[0];
		centroid[1] = c[1];
		centroid[2] = c[2];
	}
    /**
     * @brief vertexBufferSize - get size of the array of vertices
     * @return - size of array of vertices
     */
	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
    /**
     * @brief indexBufferSize - get size of the array of indices
     * @return - size of array of indices
     */
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLuint);
	}
    /**
     * @brief clear - erase arrays and reinitialize values
     */
	void clear()
	{
		delete[] vertices;
		delete[] indices;
		vertices = 0;
		indices = 0;
		numVertices = numIndices = 0;
		centroid = glm::vec3(0.0f,0.0f,0.0f);
	}
};

/** @} */

#endif // MYSHAPE_H
