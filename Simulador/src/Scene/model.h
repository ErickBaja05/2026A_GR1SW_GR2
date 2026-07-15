#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../Graphics/mesh.h"
#include "../Graphics/shader.h"

#include <string>
#include <vector>

using namespace std;

unsigned int TextureFromFile(
    const char* path,
    const string& directory,
    bool gamma);

class Model
{
public:

    Model(const string& path, bool gamma = false);

    void Draw(Shader& shader);

private:

    void loadModel(const string& path);

    void processNode(
        aiNode* node,
        const aiScene* scene);

    Mesh processMesh(
        aiMesh* mesh,
        const aiScene* scene);

    vector<Texture> loadMaterialTextures(
        aiMaterial* mat,
        aiTextureType type,
        string typeName);

public:

    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    // Busca la malla por su nombre (o parte de él) y le cambia el ID de la textura
    void swapTexture(std::string meshName, unsigned int newTextureID) {
        bool encontrada = false;

        for (Mesh& mesh : meshes) {
            // Usamos .find() para que funcione aunque Assimp la haya renombrado (ej. "Cube_Mat_Pared_001")
            if (mesh.name.find(meshName) != std::string::npos) {
                if (mesh.textures.size() > 0) {
                    mesh.textures[0].id = newTextureID; // ¡Aplicamos la nueva textura!
                    encontrada = true;
                }
            }
        }

        // RADAR DE DEPURACIÓN: Si Anderson nombró mal las cosas, la consola te avisará
        if (!encontrada) {
            std::cout << "=================================================\n";
            std::cout << "[ERROR] No se encontro ninguna malla con el nombre: " << meshName << "\n";
            std::cout << "Assimp detecto los siguientes nombres en el .obj:\n";
            for (Mesh& m : meshes) {
                std::cout << " -> " << m.name << "\n";
            }
            std::cout << "=================================================\n";
        }
    }
};

#endif