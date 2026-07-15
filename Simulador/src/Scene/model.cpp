#include "model.h"
#include <iostream>

// Se incluye stb_image definiendo la macro de implementación si no se ha hecho en otro .cpp
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Función de utilidad para la lógica de materiales sin textura
unsigned int TextureFromColor(float r, float g, float b, float a) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    unsigned char data[4] = {
        static_cast<unsigned char>(r * 255.0f),
        static_cast<unsigned char>(g * 255.0f),
        static_cast<unsigned char>(b * 255.0f),
        static_cast<unsigned char>(a * 255.0f)
    };
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureID;
}

// ==========================================
// IMPLEMENTACIÓN DE MÉTODOS DE LA CLASE MODEL
// ==========================================

// Constructor
Model::Model(const string& path, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path);
}

// Dibuja el modelo y todas sus mallas
void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

// Carga el modelo usando ASSIMP
void Model::loadModel(const string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

// Procesa el árbol de nodos de ASSIMP de forma recursiva
void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

// Extrae los vértices, índices y texturas de una malla de ASSIMP
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        // Posiciones
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normales
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // Coordenadas de textura, Tangentes y Bitangentes
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            // Tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // Bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Recuperar los índices de las caras
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Procesar Materiales
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. Diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    // =========================================================
    // LÓGICA DEL COMPAÑERO: Si no hay textura, crear una de 1x1 
    // extrayendo los colores (Kd) y opacidad (d) del .mtl
    // =========================================================
    if (diffuseMaps.empty()) {
        aiColor3D color(0.8f, 0.8f, 0.8f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

        float opacity = 1.0f;
        material->Get(AI_MATKEY_OPACITY, opacity);

        Texture tex;
        tex.id = TextureFromColor(color.r, color.g, color.b, opacity);
        tex.type = "texture_diffuse";
        tex.path = "color_" + to_string(color.r) + "_" + to_string(opacity);
        diffuseMaps.push_back(tex);
    }
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. Specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. Normal maps
    vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. Height maps
    vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // ¡NUEVO! Leemos el valor 'd' o 'Tr' del archivo .mtl de Anderson
    float opacity = 1.0f; // Por defecto es 100% sólido
    if (material->Get(AI_MATKEY_OPACITY, opacity) != AI_SUCCESS) {
        opacity = 1.0f;
    }

    return Mesh(vertices, indices, textures);
}

// Carga u optimiza la carga de texturas desde el material
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory, this->gammaCorrection);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        // 1. Inicializamos con un valor seguro por defecto para evitar el reporte de error
        GLenum format = GL_RGB;

        // 2. Evaluamos los componentes incluyendo el caso de 2 canales (GL_RG)
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 2) {
            // CORRECCIÓN DEL COMPAÑERO: Liberamos y forzamos a cargar en 4 canales (RGBA)
            stbi_image_free(data);
            // ¡AQUÍ ESTÁ LA CORRECCIÓN! Usamos filename en lugar de finalPath
            data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 4);
            format = GL_RGBA;
        }
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
        {
            cout << "WARNING::TextureFromFile:: Formato desconocido con " << nrComponents
                << " canales en: " << path << ". Usando GL_RGB por defecto." << endl;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}