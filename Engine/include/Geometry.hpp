#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <vector>
#include <string>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

class Geometry
{
public:
    void SetVertices(std::vector<glm::vec3> vs);
    void SetIndices(std::vector<int> is);
    void SetNormals(std::vector<glm::vec3> ns);
    void SetTextureCoordinates(std::vector<glm::vec2> uvs);
    void Activate();

    unsigned int vbo;
    unsigned int ibo;
    unsigned int vao;
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texture_coordinates;

};
#endif /* _GEOMETRY_H */    
    
    
