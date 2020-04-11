
#include <iostream>
#include <random>
#include <chrono>

#include "../Engine/include/Scene.hpp"

/* Problem Globals */
int window_width, window_height;
float truck_width, truck_height, truck_length;
int ***filled_volume;

/*
AXIS DEFINITIONS:
--------------------------------------------

 Y-Axis (height)|      /Z-Axis (length)
                |    /
                |  /
                |/__________
                        X-Axis (width)

--------------------------------------------
*/

enum Axis
{
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2
};

struct Package
{
    int     width;      // X
    int     height;     // Y
    int     length;     // Z
    float   value;      // $$$

    bool    is_on_truck;

    glm::vec3 pos;

    Model* model;

    Package(int width, int height, int length, float value)
    {
        this->width  = width;
        this->height = height;
        this->length = length;
        this->value  = value;
        this->is_on_truck = false;
    }

    void Rotate(Axis axis)
    {
        int temp;
        switch(axis)
        {
          case AXIS_X:
            temp = length;
            length = height;
            height = temp;
          break;
          case AXIS_Y:
            temp = length;
            length = width;
            width = temp;
          break;
          case AXIS_Z:
            temp = width;
            width = height;
            height = temp;
          break;  
        };
    }
};

struct Individual
{
    /* Package Permutation: Order Matters. Placement is from first index to last index */
    std::vector<Package> packages;
    /* Depth Maps: stores depth of closest package for a given position on the xy plane*/
    int **depth_map;

    Individual(std::vector<Package> packages)
    {
        this->packages = packages;

        /* Initialize the depth maps */
        depth_map = (int **)malloc(truck_width * sizeof(int *));
        for(int i=0; i < truck_width; i++)
        {
            depth_map[i] = (int *)malloc(truck_height * sizeof(int));
        }
    }

    /* Methods */

    /* DESCRIPTION: This function starts at first index of packages and places packages until there is overflow.
    */
    void Pack()
    {
        bool truck_not_overflowed = true;
        int current_package = 0;
        int current_pos_x = 0;
        int current_pos_y = 0;
        int current_pos_z = truck_length;
        int current_depth = truck_length;

        /* Reset the XY Plane depth map */
        for (int i = 0; i < truck_width; i++)
        {
            for(int j = 0; j < truck_height; j++)
            {
                /* Reset depth to the back of the truck */
                depth_map[i][j] = truck_length;
            }
        }

        /* Packages are placed from the back of the truck to the front of the truck (In the -Z direction) */
        while(truck_not_overflowed)
        {
            /* Get minimum depth for the cross-section of the current package */
            int min_depth = truck_length;
            for(int i = 0; i < packages[current_package].width; i++)
            {
                for(int j = 0; j < packages[current_package].height; j++)
                {
                    if(depth_map[i][j] < min_depth) min_depth = depth_map[i][j];
                }
            }

            /* Stack package at current position if there isn't a package obstructing it, or there is space to stack*/
            if(min_depth < current_pos_z && (current_pos_y + packages[current_package].height) <= truck_height)
            {
                /* Place package */
                packages[current_package].pos = glm::vec3(current_pos_x * 1.0f, current_pos_y * 1.0f, current_pos_z * 1.0f);
                packages[current_package].is_on_truck = true;

                /* Update current position */
                current_pos_y = current_pos_y + packages[current_package].height;
            }
            /* If not, start a new stack, if there is space in the row place the package there */
            else if(current_pos_x + packages[current_package].width <= truck_width)
            {
                /* Start new stack */
                current_pos_y = 0;
                current_pos_x = current_pos_x + packages[current_package].width;
                /* Place package */
                packages[current_package].pos = glm::vec3(current_pos_x * 1.0f, current_pos_y * 1.0f, current_pos_z * 1.0f);
            }
            /* If not, start a new row, if the truck doesn't overflow with the current package place it */
            else if(current_pos_z - packages[current_package].length <= 0)
            {
                /* Start new row */
                current_pos_y = 0;
                current_pos_z = current_pos_z - packages[current_package].length;
                /* Place package */
                packages[current_package].pos = glm::vec3(current_pos_x * 1.0f, current_pos_y * 1.0f, current_pos_z * 1.0f);
            }
            else
            {
                truck_not_overflowed = false;
            }
            
            /* Update depth map */
            for(int i = 0; i < packages[current_package].width; i++)
            {
                for(int j = 0; j < packages[current_package].height; j++)
                {
                    depth_map[i][j] = current_pos_z - packages[current_package].length;
                }
            }

            /* Next Package */
            current_package++;
        }
    }

    /* DESCRIPTION: 
    */
    void Mutate()
    {

    }

    /* DESCRIPTION: 
    */
    void CalculateFitness()
    {
        
    }
};

struct Population
{
    std::vector<Individual> population;
    std::vector<Individual> parents;

    /* Methods */

    /* DESCRIPTION: 
    */
    void SelectParents(void)
    {

    }
    /* DESCRIPTION: 
    */
    void Crossover(void)
    {

    }
    /* DESCRIPTION: 
    */
    void SelectSurvivors(void)
    {

    }
};

class DeliveryTruckScene : public Scene
{
public:
    /* Scene Globals */
    GLFWwindow* window;

    /* Interaction */
    

    /* Models */


    /* Loading Shaders 
    --------------------------------*/
    ShaderProgram* scene_shader = new ShaderProgram("./res/shaders/truck.fs","./res/shaders/truck.vs");

    /* Loading Textures 
    --------------------------------*/


    /* Constructor */
    DeliveryTruckScene(GLFWwindow* window)
    {
        glfwGetFramebufferSize(window, &window_width, &window_height);
        truck_length = rand() % 1000 + 500;  // X
        truck_height = rand() % 1000 + 500; // Y
        truck_width = rand() % 1000 + 500; // Z
        
        /* Initialize Fitness Volumes */
        filled_volume = (int ***)malloc(truck_width * sizeof(int **));
        //overlapped_space = (int ***)malloc(truck_width * sizeof(int **));
        for(int i=0; i < truck_width; i++)
        {
            filled_volume[i] = (int **)malloc(truck_height * sizeof(int *));
            //overlapped_space[i] = (int **)malloc(truck_height * sizeof(int **));
            for(int j=0; j < truck_height; j++)
            {
                filled_volume[i][j] = (int *)malloc(truck_length * sizeof(int));
                //overlapped_space[i][j] = (int *)malloc(truck_length * sizeof(int *));
            }
        }
    }

    /* This initializes the scene */
    void inline Initialize(void)
    {
        srand(static_cast <unsigned int> (time(0)));

        SetSceneShaderProgram(scene_shader_program);
        SetCameraView(glm::vec3(0.0f, 2*Unit, 25*Unit), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        SetCameraPerspective(70.0f, window_width / window_height, 0.001*Unit, 200*Unit);
        SetCameraBoundingBox(bound_neg_x, bound_pos_x, bound_neg_z, bound_pos_z);

        /* Lighting */
        SetSceneLightPositionOne(glm::vec3(0.0f, 45*Unit, 0.0f));
        SetSceneLightDirectionOne(glm::vec3(0.0f, -1.0f, 0.0f));
        //SetSceneLightColourOne(glm::vec4(0.7f, 0.1f, 0.1f, 1.0f));
        SetSceneLightCutoffOne(glm::cos(glm::radians(180.0f)));
        SetSceneLightSwitchOne(true);
    }

    /* This is called once per frame */
    void inline Update(float dt)
    {
        DrawScene();
    }

    void SetKeyCallback(GLFWwindow* window)
    {
        glfwSetKeyCallback(window, ArtifactRoom_keyCallback);
    }

    static void ArtifactRoom_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    /* Define Key Bindings */
    void inline InputController(GLFWwindow* window)
    {

    }
};