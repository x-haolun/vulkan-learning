#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <glm/glm.hpp> // For vertex storage

// Simple vertex structure (no normals or texture coordinates in this example)
struct SimpleVertex {
    glm::vec3 position;
};

// Function to load OBJ file
void loadOBJ(const std::string& filePath, std::vector<SimpleVertex>& vertices, std::vector<uint32_t>& indices) {
    std::ifstream objFile(filePath);
    if (!objFile.is_open()) {
        throw std::runtime_error("Failed to open OBJ file: " + filePath);
    }

    std::string line;
    std::vector<glm::vec3> tempPositions;

    while (std::getline(objFile, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        // Parse vertex positions
        if (prefix == "v") {
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }
        // Parse faces (indices)
        else if (prefix == "f") {
            uint32_t vertexIndex[3];
            for (int i = 0; i < 3; ++i) {
                std::string vertexData;
                lineStream >> vertexData;

                std::stringstream ss(vertexData);
                std::string indexStr;

                std::getline(ss, indexStr, '/'); // Read vertex index
                vertexIndex[i] = std::stoi(indexStr) - 1; // OBJ indices are 1-based
            }

            // Add indices and vertices for the face
            indices.push_back(vertexIndex[0]);
            indices.push_back(vertexIndex[1]);
            indices.push_back(vertexIndex[2]);
        }
    }

    // Copy vertex positions into vertices
    for (const auto& pos : tempPositions) {
        SimpleVertex vertex;
        vertex.position = pos;
        vertices.push_back(vertex);
    }

    objFile.close();
}

int main() {
    std::vector<SimpleVertex> vertices;
    std::vector<uint32_t> indices;

    try {
        loadOBJ("resources/viking_room.obj", vertices, indices);
        std::cout << "Loaded OBJ with " << vertices.size() << " vertices and " << indices.size() << " indices.\n";
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // You can now use `vertices` and `indices` in your rendering pipeline
    return EXIT_SUCCESS;
}