#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "mesh/iomesh.h"

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

		numVertices = static_cast<GLuint>(_mesh.n_vertices());
		vertices = new Vertex[numVertices];
		numIndices = static_cast<GLuint>(_mesh.n_faces() * 3);
		indices = new GLuint[numIndices];

		GLuint currentIndex = 0;
		TriMesh::Point c(0, 0, 0);
		float n = 0.0f;
		for (TriMesh::VertexIter v_it = _mesh.vertices_begin(); v_it != _mesh.vertices_end(); v_it++)
		{
			TriMesh::Point p = _mesh.point(*v_it);
			c += p;
			n++;
			TriMesh::Normal n = _mesh.normal(*v_it);
			vertices[currentIndex].position = glm::vec3(p[0], p[1], p[2]);
			vertices[currentIndex].color = glm::vec3(1.0f, 0.0f, 0.0f);
			vertices[currentIndex].normal = glm::vec3(n[0], n[1], n[2]);
			//cout << vertices[currentIndex].position.x << " " << vertices[currentIndex].position.y << " " << vertices[currentIndex].position.z << endl;
			currentIndex++;

			//verpoint[0];
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
			//cout << endl;
		}
	}
	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLuint);
	}
	void cleanup()
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};

#endif // MYSHAPE_H