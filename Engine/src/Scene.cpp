#include "../include/Scene.hpp"

/* Constructor */
Scene::Scene(void)
{   
    Primitive prim = Primitive();

    primitive_plane = prim.Plane();
    primitive_cube = prim.Cube();
    primitive_sphere = prim.Sphere(100,100);
    primitive_line = prim.Line();
}

Model* Scene::CreateModelPrimitive(PrimitiveType type, Model* parent)
{
    Model* newModel = nullptr;
    

    switch(type)
    {
        case PLANE:
            newModel = new Model(primitive_plane, this, parent);
            break;
        case CUBE:
            newModel = new Model(primitive_cube, this, parent);
            break;
        case SPHERE:
            newModel = new Model(primitive_sphere, this, parent);
            break;
        case LINE:
            newModel = new Model(primitive_line, this, parent);
            break;
    };
    /* Set default shader program */
    newModel->SetModelShaderProgram(scene_shader_program);
    /* Append model to scene models */
    models.push_back(newModel);

    return newModel;
}

/* Draw the Scene */
void Scene::DrawScene(void)
{  
    for(unsigned int i = 0; i < models.size(); i++)
    {
        models[i]->Draw();
    }
}

void Scene::ToggleSceneTextures(void)
{
    for(int i = 0; i < models.size(); i++)
    {
        models[i]->ToggleModelTexture();
    }
}

void Scene::SetCameraView(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp)
{
    camera_position = cameraPosition;
    camera_front = cameraFront;
    camera_up = cameraUp;

    view_matrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void Scene::SetCameraPerspective(float fov, float ar, float near, float far)
{
    camera_fov = fov;
    camera_aspect_ratio = ar;
    camera_near = near;
    camera_far = far;

    projection_matrix = glm::perspective(fov, ar, near, far);
}

void Scene::SetCameraOrthographic(float a, float b, float c, float d, float near, float far)
{
    camera_near = near;
    camera_far = far;
    projection_matrix = glm::ortho(a, b, c, d, near, far);
}

std::vector<Model*> Scene::GetSceneModels(void)
{
    return models;
}

ShaderProgram* Scene::GetSceneShaderProgram(void)
{
    return scene_shader_program;
}

glm::vec3 Scene::GetCameraPosition(void)
{
    return camera_position;
}

float Scene::GetCameraYaw(void)
{
    return camera_yaw;
}

float Scene::GetCameraPitch(void)
{
    return camera_pitch;
}

glm::mat4 Scene::GetCameraViewMatrix(void)
{
    return view_matrix;
}

glm::mat4 Scene::GetCameraProjectionMatrix(void)
{
    return projection_matrix;
}

void Scene::SetSceneShaderProgram(ShaderProgram* shader)
{
    scene_shader_program = shader;
}

void Scene::SetCameraBoundingBox(float x_neg, float x_pos, float z_neg, float z_pos)
{
    camera_bound_x_neg = x_neg;
    camera_bound_x_pos = x_pos;
    camera_bound_z_neg = z_neg;
    camera_bound_z_pos = z_pos;
}

bool Scene::CheckBoundingBoxCollision(glm::vec3 pos, glm::vec3 dir, float distance)
{
    pos += distance * glm::vec3(dir.x, 0.0f, dir.z);
    if( pos.x > camera_bound_x_pos || 
        pos.x < camera_bound_x_neg || 
        pos.z > camera_bound_z_pos ||
        pos.z < camera_bound_z_neg)
        return false;

    return true;
}

void Scene::TranslateCameraRight(float distance)
{
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, camera_up));
    /* Check Boundary */
    if(CheckBoundingBoxCollision(camera_position, camera_right, distance))
    {
        /* Stay on Ground */
        camera_position += distance * glm::vec3(camera_right.x, 0.0f, camera_right.z);
    }
}
void Scene::TranslateCameraLeft(float distance)
{
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, camera_up));
    /* Check Boundary */
    if(CheckBoundingBoxCollision(camera_position, -camera_right, distance))
    {
        /* Stay on Ground */
        camera_position -= distance * glm::vec3(camera_right.x, 0.0f, camera_right.z);
    }
}
void Scene::TranslateCameraForward(float distance)
{
    /* Check Boundary */
    if(CheckBoundingBoxCollision(camera_position, camera_front, distance))
    {
        /* Stay on Ground */
        camera_position += distance * glm::vec3(camera_front.x, 0.0f, camera_front.z);
    }
}
void Scene::TranslateCameraBackward(float distance)
{
    /* Check Boundary */
    if(CheckBoundingBoxCollision(camera_position, -camera_front, distance))
    {
        /* Stay on Ground */
        camera_position -= distance * glm::vec3(camera_front.x, 0.0f, camera_front.z);
    }
}
void Scene::RotateCameraEuler(float yaw, float pitch)
{
    camera_yaw = yaw;
    camera_pitch = pitch;

    glm::vec3 new_camera_front;
    new_camera_front.x = cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
    new_camera_front.y = sin(glm::radians(camera_pitch));
    new_camera_front.z = sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
    camera_front = glm::normalize(new_camera_front);
}

void Scene::UpdateCameraView(void)
{
    SetCameraView(camera_position, camera_front, camera_up);
}
