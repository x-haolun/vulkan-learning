#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <glm/glm.hpp>

// Simple vertex structure
struct ObjectVertex {
    glm::vec3 position;
};
// Function to load OBJ file
void loadOBJ(const std::string& filePath, std::vector<ObjectVertex>& vertices, std::vector<uint32_t>& indices) {
    std::ifstream objFile(filePath);
    if (!objFile.is_open()) {
        throw std::runtime_error("Failed to open OBJ file: " + filePath);
    }

    std::string line;
    std::vector<glm::vec3> tempPositions; // Temporary storage for positions

    while (std::getline(objFile, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v") {
            // Parse vertex position directly into the vertex structure
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            ObjectVertex vertex;
            vertex.position = position;
            vertices.push_back(vertex); // Add the vertex directly to the vertex list
        }
        else if (prefix == "f") {
            // Parse face (indices)
            uint32_t vertexIndex[3];
            for (int i = 0; i < 3; ++i) {
                std::string vertexData;
                lineStream >> vertexData;

                std::stringstream ss(vertexData);
                std::string indexStr;

                std::getline(ss, indexStr, '/'); // Read vertex index
                vertexIndex[i] = std::stoi(indexStr) - 1; // OBJ indices are 1-based
            }

            // Add indices to the indices list
            indices.push_back(vertexIndex[0]);
            indices.push_back(vertexIndex[1]);
            indices.push_back(vertexIndex[2]);
        }
        else if (prefix == "o") {
            // Object name (e.g., `o mesh_all1_Texture1_0`)
            std::string objectName;
            lineStream >> objectName;
            std::cout << "Object: " << objectName << std::endl;
        }
      
    }

    objFile.close();
}