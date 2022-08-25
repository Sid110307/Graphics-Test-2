#pragma once

#include "./mesh.h"
#include "./camera.h"
#include "./object.h"

#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#include <vector>

class Model : public Object
{
public:
	Model(std::string, GLfloat, GLfloat, GLfloat);
	~Model();

	void init() override;
	void showModelName(bool);

private:
	std::string path;

	std::vector<Mesh> meshes;
	std::vector<Mesh::Texture> textures_loaded;

	bool isModelNameShown = false;
	std::vector<GLfloat> modelVertices, modelNormals, modelTexCoords, modelColors, modelMaterials;

	void loadModel();
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);

	std::vector<Mesh::Texture>
	loadMaterial(aiMaterial*, aiTextureType, const std::string &);
	static GLint loadTexture(const std::string &, const std::string &);
};

Model::Model(std::string _path, GLfloat _x, GLfloat _y, GLfloat _z)
		: Object(_x, _y, _z), path(std::move(_path))
{
	loadModel();
}

Model::~Model()
{
	for (auto &i: textures_loaded) glDeleteTextures(1, &i.id);
}

void Model::init()
{
	Shader shader;
	shader.use();

	glm::mat4 projection = glm::perspective(glm::radians(Camera::fieldOfView),
											Camera::aspectRatio,
											Camera::nearClip, Camera::farClip);

	glm::mat4 view = glm::lookAt(glm::vec3(Camera::x, Camera::y, Camera::z),
								 glm::vec3(Camera::x + Camera::lx,
										   Camera::y + Camera::ly,
										   Camera::z + Camera::lz),
								 glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1,
					   GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1,
					   GL_FALSE, glm::value_ptr(projection));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1,
					   GL_FALSE, glm::value_ptr(model));

	for (auto &mesh: meshes) mesh.draw(shader);

	if (isModelNameShown)
	{
		std::string txt = "Model name: ";
		txt += path.substr(path.find_last_of("/\\") + 1);

		glRasterPos3f(-1.0f, 5.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18,
						 (const unsigned char*) txt.c_str());
	}
}

void Model::showModelName(bool _isModelNameShown)
{
	isModelNameShown = _isModelNameShown;
}

void Model::loadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
												   aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		std::cerr << "Error while loading '" << path << "': "
				  << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

// NOLINTBEGIN (misc-no-recursion)
void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}
// NOLINTEND

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Mesh::Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex = {};
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (mesh->mNormals)
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Mesh::Texture> diffuseMaps = loadMaterial(material,
															  aiTextureType_DIFFUSE,
															  "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(),
						diffuseMaps.end());

		std::vector<Mesh::Texture> specularMaps = loadMaterial(material,
															   aiTextureType_SPECULAR,
															   "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(),
						specularMaps.end());
	}

	return {vertices, indices, textures};
}

std::vector<Mesh::Texture>
Model::loadMaterial(aiMaterial* mat, const aiTextureType type,
					const std::string &typeName)
{
	std::vector<Mesh::Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (auto &texture: textures_loaded)
		{
			if (texture.path == str)
			{
				textures.push_back(texture);
				skip = true;

				break;
			}
		}

		if (!skip)
		{
			Mesh::Texture texture;

			texture.id = loadTexture(str.C_Str(),
									 path.substr(0, path.find_last_of("/\\")));
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);

			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

GLint Model::loadTexture(const std::string &_path, const std::string &directory)
{
	std::string filename = directory + '/' + _path;
	GLuint textureID;

	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height,
										   nullptr, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return (GLint) textureID;
}
