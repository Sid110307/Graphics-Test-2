#pragma once

#include "./shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture
	{
		GLuint id;
		std::string type;
		aiString path;
	};

	Mesh(std::vector<Vertex> &, std::vector<GLuint> &,
		 std::vector<Texture> &);
	void draw(Shader &);
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	GLuint VAO, VBO, EBO;
	void setupMesh();
};


Mesh::Mesh(std::vector<Vertex> &_vertices, std::vector<GLuint> &_indices,
		   std::vector<Texture> &_textures)
		: vertices(_vertices), indices(_indices), textures(_textures), VAO(0),
		  VBO(0), EBO(0)
{
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (long) (vertices.size() * sizeof(Vertex)),
				 &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 (long) (indices.size() * sizeof(GLuint)), &indices[0],
				 GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (GLvoid*) offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						  (GLvoid*) offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::draw(Shader &shader)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLint) indices.size(), GL_UNSIGNED_INT,
				   nullptr);
	glBindVertexArray(0);
}
