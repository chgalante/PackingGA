
#include "../include/Geometry.hpp"

void Geometry::SetVertices(std::vector<glm::vec3> vs)
{
    vertices = vs;
}

void Geometry::SetIndices(std::vector<int> is)
{
    indices = is;
}

void Geometry::SetNormals(std::vector<glm::vec3> ns)
{
    normals = ns;
}

void Geometry::SetTextureCoordinates(std::vector<glm::vec2> uvs)
{
    texture_coordinates = uvs;
}

void Geometry::Activate()
{
    // Create a vertex array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* Compile Vertex Data */
    std::vector<float> vertex_data = std::vector<float>();
    unsigned int size_of_vertex = 2*sizeof(float) + 3*sizeof(float) + 3* sizeof(float);

    for(int i = 0; i < vertices.size(); i ++)
    {
        vertex_data.push_back(vertices[i].x);
        vertex_data.push_back(vertices[i].y);
        vertex_data.push_back(vertices[i].z);
        vertex_data.push_back(normals[i].x);
        vertex_data.push_back(normals[i].y);
        vertex_data.push_back(normals[i].z);
        vertex_data.push_back(texture_coordinates[i].x);
        vertex_data.push_back(texture_coordinates[i].y);
    }

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size()*sizeof(float), &vertex_data[0], GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(	0,                          // attribute 0 matches aPos in Vertex Shader
                            3,                          // size
                            GL_FLOAT,                   // type
                            GL_FALSE,                   // normalized?
                            size_of_vertex,	            // stride - size of each vertex in bytes
                            (void*)0                    // array buffer offset
    );

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(	1,                          // attribute 1 matches aNorm in Vertex Shader
                            3,                          // size
                            GL_FLOAT,                   // type
                            GL_FALSE,                   // normalized?
                            size_of_vertex,	            // stride - size of each vertex in bytes
                            (void*)12                   // array buffer offset
    );

    // Texture Coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(	2,                          // attribute 2 matches aTexCoord in Vertex Shader
                            2,                          // size
                            GL_FLOAT,                   // type
                            GL_FALSE,                   // normalized?
                            size_of_vertex,	            // stride - size of each vertex in bytes
                            (void*)24                   // array buffer offset
    );

    // Upload Index Buffer Object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}