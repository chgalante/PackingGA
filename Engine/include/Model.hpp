#ifndef _MODEL_H
#define _MODEL_H

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include "ShaderProgram.hpp"
#include "Geometry.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "Animation.hpp"

class ShaderProgram;
class Geometry;
class Scene;
class Animation;
class Model
{
public:
    Scene* scene;
    Model* parent;
    Geometry* geometry;
    Texture* texture = nullptr;

    std::vector<Model*> children;
    std::vector<Animation> model_animations;

    glm::vec4* position;
    glm::vec4* forward_direction;
    glm::vec4* right_direction;


    glm::mat4 model_matrix;
    glm::vec4 fragment_colour;

    unsigned int render_mode = GL_TRIANGLES;
    bool textures_enabled = false;
    bool transparency_enabled = false;

    /* Shader */
    ShaderProgram* model_shader_program;

    /* Construct */
    Model(Geometry* g, Scene* s, Model* p);
    void AddChild(Model* child);

    /* Configure Model Parameters */
    void SetModelShaderProgram(ShaderProgram* p);
    void SetModelFragmentColour(glm::vec4 c);
    void SetModelTexture(Texture* tex);
    void SetModelRenderMode(unsigned int m);
    void SetModelTransparency(bool isTransparent);

    /* Model Transformations */
    void TranslateModel(glm::vec3 translation);
    void TranslateModelAndChildren(glm::vec3 translation);
    void ScaleModel(glm::vec3 scalars);
    void ScaleModelAndChildren(glm::vec3 scalars);
    void RotateModel(float angle, glm::vec3 axis);
    void RotateModelAndChildren(float angle, glm::vec3 axis);

    /* Animation */
    glm::vec3 GetModelPosition(void);
    glm::vec3 GetModelForwardDirectionVector(void);
    glm::vec3 GetModelRightDirectionVector(void);
    void AddModelAnimation(Animation anim);
    void StepModelAnimations(float input_direction, float dt);

    /* Drawing */
    void BindUniforms(void);
    void Draw(void);

    /* Toggle */
    void ToggleModelTexture(void);

};
#endif /* _MODEL_H */