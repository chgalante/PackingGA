#include "../include/Animation.hpp"

Animation::Animation(Model* model)
{
    this->model = model;
}

void Animation::Register(unsigned int type, glm::vec4* direction, float speed, float lower_bound, float upper_bound)
{
    this->type = type;
    this->transform_direction = direction;
    this->speed = speed;
    this->lower_bound = lower_bound;
    this->upper_bound = upper_bound;
}

void Animation::Step(float input_direction, float dt)
{
    if( current_pos >= upper_bound || current_pos <= lower_bound )
    speed = (-1)*speed;

    current_pos += dt*speed*input_direction;

    switch(type)
    {
    case 0: // Translate
        model->TranslateModelAndChildren(glm::vec3(*transform_direction) * (dt*speed*input_direction));
        break;

    case 1: // Scale
            /* TODO */

        break;

    case 2: // Rotate
        glm::vec3 model_position = model->GetModelPosition();
        model->TranslateModelAndChildren(-model_position);
        model->RotateModelAndChildren(dt*speed*input_direction, *transform_direction);
        model->TranslateModelAndChildren(model_position);
        break;
    };
}
