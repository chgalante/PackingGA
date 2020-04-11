#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "Model.hpp"
#include <vector>

class Model;
class Animation
{
private:
    Model* model;           /* The model being animated */
    float lower_bound;
    float upper_bound;
    float speed;
    float current_pos = 0;
    glm::mat4 transform;
    glm::vec4* transform_direction;
    unsigned int type;

public:
    Animation(Model* model);

    void Register( unsigned int type,
                    glm::vec4* direction,
                    float speed,
                    float lower_bound, 
                    float upper_bound); /* Register an animation transform component */

    void Step(float input_direction, float dt);    /* Take a step through all animation components */
};


#endif /* _ANIMATION_H */