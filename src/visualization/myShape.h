#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../mesh/iomesh.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};

struct ShapeData
{
	glm::vec3 centroid;
	Vertex* vertices;
	GLuint numVertices;
	GLuint* indices;
	GLuint numIndices;

	ShapeData() :
	vertices(0), numVertices(0),
	indices(0), numIndices(0) {}
	ShapeData(string fileName)
	{
		loadFromFile(fileName);
	}
	void loadFromFile(string fileName)
	{
		TriMesh _mesh;
		importMesh(_mesh, fileName);
		_mesh.request_face_normals();
		_mesh.request_vertex_normals();
		_mesh.update_normals();
		loadMesh(_mesh);
	}
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
	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLuint);
	}
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

#endif // MYSHAPE_H