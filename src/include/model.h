#pragma once

#include <vector>
#include <filesystem>

#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "./mesh.h"
#include "./camera.h"
#include "./shader.h"

class Model : public Object
{
public:
    Model(std::string, const Shader &, GLfloat, GLfloat, GLfloat);
    ~Model();

    void create() override;
    bool isModelNameShown = false;

private:
    std::string path;

    std::vector<Mesh> meshes;
    std::vector<Mesh::Texture> textures_loaded;

    std::vector<GLfloat> modelVertices, modelNormals, modelTexCoords, modelColors, modelMaterials;

    void loadModel();
    void processNode(aiNode*, const aiScene*);
    Mesh processMesh(aiMesh*, const aiScene*);

    std::vector<Mesh::Texture> loadMaterial(aiMaterial*, aiTextureType, const std::string &);
    static GLint loadTexture(const std::string &, const std::string &);
};

Model::Model(std::string _path, const Shader &_shader, GLfloat _x, GLfloat _y, GLfloat _z)
        : Object(_shader, _x, _y, _z), path(std::move(_path))
{
    if (validatePath(path).empty())
    {
        std::cerr << "Error: Model \"" << path << "\" is not loaded." << std::endl;
        return;
    }

    loadModel();
}

Model::~Model()
{
    for (auto &i: textures_loaded) glDeleteTextures(1, &i.id);
}

void Model::create()
{
    Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    shader.use();

    glPushMatrix();
    glTranslatef(x, y, z);

    for (auto &i: meshes) i.draw(shader);
    glPopMatrix();

    if (isModelNameShown)
    {
        glPushMatrix();
        glTranslatef(x, y, z);

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);

        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glEnd();

        putText(shader, "Model name: " + path.substr(path.find_last_of("/\\") + 1), glm::vec3(-1.0f, 5.0f, 0.0f), 1.0f,
                glm::vec3(0.0f, 0.0f, 0.0f));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);

        glPopMatrix();
    }
}

void Model::loadModel()
{
    path = std::filesystem::absolute(path).string();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Error while loading \"" << path << "\": " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    std::stack<aiNode*> nodeStack;
    nodeStack.push(node);

    while (!nodeStack.empty())
    {
        aiNode* currentNode = nodeStack.top();
        nodeStack.pop();

        for (GLuint i = 0; i < currentNode->mNumMeshes; ++i)
            meshes.push_back(processMesh(scene->mMeshes[currentNode->mMeshes[i]], scene));

        for (GLuint i = 0; i < currentNode->mNumChildren; ++i) nodeStack.push(currentNode->mChildren[i]);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Mesh::Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; ++i)
    {
        Mesh::Vertex vertex = {};
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->mNormals)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        } else vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (GLuint i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; ++j) indices.push_back(face.mIndices[j]);
    }

    if ((GLint) mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Mesh::Texture> diffuseMaps = loadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Mesh::Texture> specularMaps = loadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return {vertices, indices, textures};
}

std::vector<Mesh::Texture> Model::loadMaterial(aiMaterial* mat, const aiTextureType type, const std::string &typeName)
{
    std::vector<Mesh::Texture> textures;
    for (GLuint i = 0; i < mat->GetTextureCount(type); ++i)
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
            Mesh::Texture texture =
                    {
                            (GLuint) loadTexture(str.C_Str(), path.substr(0, path.find_last_of("/\\"))),
                            typeName,
                            str
                    };

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

    GLint w, h, components;
    GLubyte* image = stbi_load(filename.c_str(), &w, &h, &components, 0);

    if (image)
    {
        GLenum format;
        if (components == 1) format = GL_RED;
        else if (components == 3) format = GL_RGB;
        else if (components == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, w, h, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else std::cerr << "Texture failed to load at path: " << filename << std::endl;

    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return (GLint) textureID;
}
