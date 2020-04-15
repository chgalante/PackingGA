
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_map>

#include "../Engine/include/Scene.hpp"


/* Problem Globals */

#define NUM_PACKAGES                100
#define MIN_PACKAGE_DIM             5
#define MAX_PACKAGE_DIM             50
#define MIN_PACKAGE_VALUE           5
#define MAX_PACKAGE_VALUE           5000

#define TRUCK_WIDTH                 100
#define TRUCK_HEIGHT                100
#define TRUCK_LENGTH                100

#define NUM_POPULATIONS             1
#define POPULATION_SIZE             200
#define PARENT_POOL_SIZE            100
#define ELITE_POPULATION_SIZE       25
#define TOURNAMENT_WINDOW           4

#define MUTATION_CHANCE             5 /* Percent */

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
    unsigned int     id;         // Package identifier relating to master list
    int     width;      // X
    int     height;     // Y
    int     length;     // Z
    float   value;      // $$$

    bool    is_on_truck;

    glm::vec3 pos;

    Model* model;
    /* Default Constructor */
    Package()
    {
        /* Do nothing */
    }
    /* Constructor */
    Package(int width, int height, int length, float value, unsigned int id)
    {
        this->width  = width;
        this->height = height;
        this->length = length;
        this->value  = value;
        this->id     = id;
        this->is_on_truck = false;
    }

    /* DESCRIPTION: This function rotates a package along a given axis.
    */
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
    std::vector<Package> packages;    // Package Permutation: Order Matters. Placement is from first index to last index
    float                fitness;     // Measure of fitness   
    int                **depth_map;   // Depth Map: stores depth of closest package for a given position on the xy plane

    Individual()
    {
        this->packages = std::vector<Package>();

        /* Initialize the depth maps */
        depth_map = (int **)malloc(TRUCK_WIDTH * sizeof(int *));
        for(int i=0; i < TRUCK_WIDTH; i++)
        {
            depth_map[i] = (int *)malloc(TRUCK_HEIGHT * sizeof(int));
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
        int current_pos_z = 0;

        /* Reset the XY Plane depth map */
        for (int i = 0; i < TRUCK_WIDTH; i++)
        {
            for(int j = 0; j < TRUCK_HEIGHT; j++)
            {
                /* Reset depth to the back of the truck */
                depth_map[i][j] = 0;
            }
        }

        /* Packages are placed from the back of the truck to the front of the truck (In the -Z direction) */
        while(truck_not_overflowed)
        {
            /* Get maximum depth for the xy projection of the current package */
            int max_depth = 0;
            for(int i = 0; i < packages[current_package].width; i++)
            {
                for(int j = 0; j < packages[current_package].height; j++)
                {
                    if(depth_map[i][j] > max_depth) max_depth = depth_map[i][j];
                }
            }

            /* Stack package at current position if there isn't a package obstructing it, or there is space to stack*/
            if(max_depth > current_pos_z && (current_pos_y + packages[current_package].height) <= TRUCK_HEIGHT)
            {
                /* Place package */
                packages[current_package].pos = glm::vec3(current_pos_x * 1.0f, current_pos_y * 1.0f, current_pos_z * 1.0f);
                packages[current_package].is_on_truck = true;

                /* Update current y position */
                current_pos_y = current_pos_y + packages[current_package].height;
            }
            /* If not, start a new stack, if there is space in the row place the package there */
            else if(current_pos_x + packages[current_package].width <= TRUCK_WIDTH)
            {
                /* Start new stack */
                current_pos_y = 0;
                current_pos_x = current_pos_x + packages[current_package].width;

                /* Place package */
                packages[current_package].pos = glm::vec3(current_pos_x * 1.0f, current_pos_y * 1.0f, current_pos_z * 1.0f);
                packages[current_package].is_on_truck = true;

                /* Update current y position */
                current_pos_y = current_pos_y + packages[current_package].height;
            }
            /* If not, start a new row, if the truck doesn't overflow with the current package place it */
            else if(current_pos_z - packages[current_package].length <= 0)
            {
                /* Start new row */
                current_pos_y = 0;
                current_pos_z = current_pos_z - packages[current_package].length;

                /* Place package */
                packages[current_package].pos = glm::vec3(current_pos_x * 1.0f, current_pos_y * 1.0f, current_pos_z * 1.0f);
                packages[current_package].is_on_truck = true;

                /* Update current y position */
                current_pos_y = current_pos_y + packages[current_package].height;
            }
            else
            {
                truck_not_overflowed = false;
                break;
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
        /* Rotate a random package along random axis */
        if((rand() % 100) < MUTATION_CHANCE)
        {
            packages[rand() % NUM_PACKAGES].Rotate((Axis)(rand() % 3));
        }

        /* Swap two random packages */
        unsigned int rand_index1, rand_index2;
        Package temp;
        if((rand() % 100) < MUTATION_CHANCE)
        {
            rand_index1 = rand() % NUM_PACKAGES;
            rand_index2 = rand_index1;
            while(rand_index2 == rand_index1) rand_index2 = rand() % NUM_PACKAGES;

            temp = packages[rand_index1];
            packages[rand_index1] = packages[rand_index2];
            packages[rand_index2] = temp;
        }
    }

    /* DESCRIPTION: 
    */
    void CalculateFitness()
    {
        /* Map filled units in fitness volume with the value per volume of the package filling that space */
        unsigned int start_x, end_x;
        unsigned int start_y, end_y;
        unsigned int start_z, end_z;
        Package current_package;
        float value_on_truck = 0.0f;
        int filled_volume = 0;
        for(int i = 0; i < packages.size(); i++)
        {
            current_package = packages[i];
            /* First check if package in question is placed on the truck */
            if(current_package.is_on_truck)
            {
                value_on_truck += current_package.value;
                start_x = (unsigned int)current_package.pos.x;
                end_x   = start_x + current_package.width;
                start_y = (unsigned int)current_package.pos.y;
                end_y   = start_y + current_package.height;
                start_z = (unsigned int)current_package.pos.z;
                end_z   = start_z + current_package.length;

                for(int x = start_x; x <= end_x; x++)
                {
                    for(int y = start_y; y <= end_y; y++)
                    {
                        for(int z = start_z; z <= end_z; z++)
                        {
                            filled_volume += 1;
                        }
                    }
                }        
            }
            else
            {
                /* Since packages are placed from index 0 to number of packages
                    we can assume that the rest of the packages are not on the truck */
                break;
            }
        }

        /* Fitness is fraction of filled volume over truck volume multiplied by the value on the truck */
        fitness = (float)(filled_volume / (TRUCK_WIDTH * TRUCK_HEIGHT * TRUCK_LENGTH)) * value_on_truck;
    }
};

struct Population
{
    std::vector<Individual> population;
    std::vector<Individual> parents;

    Population()
    {
        population = std::vector<Individual>();
        parents = std::vector<Individual>();
    }

    /* Methods */

    /* DESCRIPTION: This function runs a genetic algorithm on the population for one iteration.
    */
    void Iterate(void)
    {
        SelectParents();

        /* Crossover random parents from the parent pool */
        unsigned int parent1_index, parent2_index;
        for(int i = 0; i < PARENT_POOL_SIZE; i++)
        {
            /* Get two unique parents */
            parent1_index = rand() % PARENT_POOL_SIZE;
            parent2_index = parent1_index;
            while(parent1_index == parent2_index) parent2_index = rand() % PARENT_POOL_SIZE;

            Crossover(parents[parent1_index], parents[parent2_index]);
        }

        SelectSurvivors();
    } 

    /* DESCRIPTION: Select parents using tournament selection.
    */
    void SelectParents(void)
    {
        /* Clear current parent pool */
        parents.clear();
        unsigned int index_of_winner, random_index;
        while(parents.size() <= PARENT_POOL_SIZE)
        {
            index_of_winner = rand() % POPULATION_SIZE;
            for(int i = 0; i < TOURNAMENT_WINDOW; i++)
            {
                random_index = rand() % POPULATION_SIZE;
                if(population[index_of_winner].fitness < population[random_index].fitness)
                    index_of_winner = random_index;
            }
            parents.push_back(population[index_of_winner]);
        }
    }
    /* DESCRIPTION: Cyclically Crossover package lists of two parents to generate a new individual.
    */
    void Crossover(Individual p1, Individual p2)
    {
        /* Initialise Hash Maps and Flags for use with finding cycles */
        /* These will be used to fetch the positions of elements within each parent's package list */
        std::unordered_map<unsigned int, unsigned int> hash_parent_1 = std::unordered_map<unsigned int, unsigned int>();
        std::unordered_map<unsigned int, unsigned int> hash_parent_2 = std::unordered_map<unsigned int, unsigned int>();
        std::vector<bool> flags_parent_1 = std::vector<bool>();
        std::vector<bool> flags_parent_2 = std::vector<bool>();

        Individual offspring = Individual();
        offspring.packages.resize(NUM_PACKAGES);

        for(unsigned int i = 0; i < NUM_PACKAGES; i++)
        {
            hash_parent_1.emplace(std::pair<unsigned int, unsigned int>(p1.packages[i].id, i));
            flags_parent_1.push_back(false);

            hash_parent_2.emplace(std::pair<unsigned int, unsigned int>(p2.packages[i].id, i));
            flags_parent_2.push_back(false);
        }

        /* Find Cycles */
        bool swap = true;
        bool end = false;
        unsigned int index;
        int element;
        int element_index;
        int first_element;

        while(!end)
        {
            if(swap)
            {
                /* Start with Parent 1 */
                index = 0;
                /* Search for an element that hasn't been used */
                while(flags_parent_1[index]) index++;
                if(index >= NUM_PACKAGES) break;
                first_element = p1.packages[index].id;

                while(true)
                {
                    element = p2.packages[index].id;
                    element_index = index;
                    flags_parent_2[index] = true;
                    index = hash_parent_1[element];
                    flags_parent_1[index] = true;
                    offspring.packages[index] = p2.packages[element_index];
                    if(element == first_element) break;
                }
                swap = false;
            }
            if(!swap)       
            {
                /* Start with Parent 2 */
                index = 0;
                /* Search for an element that hasn't been used */
                while(flags_parent_2[index]) index++;
                if(index >= NUM_PACKAGES) break;
                first_element = p2.packages[index].id;

                while(true)
                {
                    element = p1.packages[index].id;
                    element_index = index;
                    flags_parent_1[index] = true;
                    index = hash_parent_2[element];
                    flags_parent_2[index] = true;
                    offspring.packages[index] = p1.packages[element_index];
                    if(element == first_element) break;
                }
                swap = true;
            }
            end = true;
            for(int i = 0; i < NUM_PACKAGES; i++)
            {
                if(flags_parent_1[i] == false || flags_parent_2[i] == false)
                {
                    end = false;
                }
            }
        }
        /* Mutate Offspring and Calculate Fitness */
        offspring.Mutate();
        offspring.Pack();
        offspring.CalculateFitness();
        /* Add offspring to population */
        population.push_back(offspring);
    }

    /* DESCRIPTION: Select next population using Tournament Selection.
    */
    void SelectSurvivors(void)
    {
        std::vector<Individual> nextPopulation = std::vector<Individual>();

        /* Bring elites to the next population */
        for(int i = 0; i < ELITE_POPULATION_SIZE; i++)
        {
            nextPopulation.push_back(population[0]);
            population.erase(population.begin());
        }

        /* Rest of the population selected tournament style */
        unsigned int index_of_winner, random_index;
        while(nextPopulation.size() <= POPULATION_SIZE)
        {
            random_index = rand() % population.size();
            index_of_winner = random_index;
            for(int i = 0; i < TOURNAMENT_WINDOW; i++)
            {
                random_index = rand() % population.size();
                if(population[index_of_winner].fitness < population[random_index].fitness)
                    index_of_winner = random_index;
            }
            InsertIntoPopulation(population[index_of_winner], &nextPopulation);
            population.erase(population.begin() + index_of_winner);
        }
        /* Replace current population with new population */
        population = nextPopulation;
    }

    /* DESCRIPTION: Insert an individual into the population in order of descending fitness
    */
    void InsertIntoPopulation(Individual indiv, std::vector<Individual>* popul)
    {
        int i = 0;
        while(1)
        {            
            if(popul->size() > i)
            {
                if(popul->at(i).fitness >= indiv.fitness)
                {
                    // The individual has a lower fitness than the individual at the current index
                    i++;
                }
                else
                {
                    // Place individual in order of greatest fitness
                    popul->insert(popul->begin() + i, indiv);
                    break;
                }            
            }
            else
            {
                // We have reached the end, place individual at the end of the vector
                popul->push_back(indiv);
                break;
            }        
        }
    }
};

class DeliveryTruckScene : public Scene
{
public:
    /* Scene Globals */
    GLFWwindow* window;
    int width, height;

    std::vector<Package> master_package_list = std::vector<Package>();
    std::vector<Population> populations = std::vector<Population>();

    Model* mTruck;
    Model *mAxisX, *mAxisY, *mAxisZ;
    std::unordered_map<int, Model*> mPackages = std::unordered_map<int, Model*>();

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
        this->window = window;
        glfwGetFramebufferSize(window, &width, &height);
    }

    /* This initializes the scene */
    void inline Initialize(void)
    {
        srand(static_cast <unsigned int> (time(0)));

        SetSceneShaderProgram(scene_shader);
        SetCameraView(glm::vec3(0.0f, 5.0f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        SetCameraPerspective(70.0f, width / height, 0.001*Unit, 200*Unit);
        SetCameraBoundingBox(-100.0f, 100.0f, -100.0f, 100.0f);

        /* Lighting */
        SetSceneLightPosition(glm::vec3(200.0f, 200.0f, 200.0f));
        SetSceneLightDirection(-GetSceneLightPosition());
        SetSceneLightCutoff(glm::cos(glm::radians(180.0f)));
        SetSceneLightSwitch(true);

        /* Initialize Truck Model for Rendering */
        mTruck = CreateModelPrimitive(CUBE, NULL);
        mTruck->ScaleModel(glm::vec3((float)TRUCK_WIDTH/10, (float)TRUCK_HEIGHT/10, (float)TRUCK_LENGTH/10));
        mTruck->TranslateModel(glm::vec3((float)TRUCK_WIDTH/20, 0.0f, (float)TRUCK_LENGTH/20));
        mTruck->SetModelRenderMode(GL_LINE_LOOP);
        mTruck->SetModelFragmentColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        mAxisX = CreateModelPrimitive(CUBE, NULL);
        mAxisX->ScaleModel(glm::vec3(2.0f, 0.05f, 0.05f));
        mAxisX->TranslateModel(glm::vec3(Unit, 0.0f, 0.0f));
        mAxisX->SetModelFragmentColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        mAxisY = CreateModelPrimitive(CUBE, NULL);
        mAxisY->ScaleModel(glm::vec3(2.0f, 0.05f, 0.05f));
        mAxisY->RotateModel(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mAxisY->TranslateModel(glm::vec3(0.0f, Unit, 0.0f));
        mAxisY->SetModelFragmentColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    

        mAxisZ = CreateModelPrimitive(CUBE, NULL);
        mAxisZ->ScaleModel(glm::vec3(2.0f, 0.05f, 0.05f));
        mAxisZ->RotateModel(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        mAxisZ->TranslateModel(glm::vec3(0.0f, 0.0f, Unit));
        mAxisZ->SetModelFragmentColour(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        /* Generate Packages */
        Package newPackage;
        Model* newPackageModel;
        unsigned int width, height, length;
        float value;
        for(unsigned int i = 0; i < NUM_PACKAGES; i++)
        {
            /* Determine dimensions and value of package */
            width = rand()%(MAX_PACKAGE_DIM + 1) + MIN_PACKAGE_DIM;
            height = rand()%(MAX_PACKAGE_DIM + 1) + MIN_PACKAGE_DIM;
            length = rand()%(MAX_PACKAGE_DIM + 1) + MIN_PACKAGE_DIM;
            value = (float)(rand()%(MAX_PACKAGE_VALUE + 1) + MIN_PACKAGE_VALUE);

            /* Add the package to the master package list */
            newPackage = Package(width, height, length, value, i);
            master_package_list.push_back(newPackage);

            /* Create a model for this package for rendering*/
            newPackageModel = CreateModelPrimitive(CUBE, mTruck);
            newPackageModel->SetModelVisibility(false);
            newPackageModel->SetModelFragmentColour(glm::vec4(1.0f,1.0f,1.0f,1.0f));
            mPackages.insert(std::pair<int, Model*>(newPackage.id, newPackageModel));
        }

        /* Generate Populations */
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine e(seed);
        Population newPopulation;
        Individual newIndividual;
        for(int i = 0; i < NUM_POPULATIONS; i++)
        {
            newPopulation = Population();
            for(int j = 0; j < POPULATION_SIZE; j++)
            {
                /* Individuals in the population will have a shuffled package list from the master package list */
                newIndividual = Individual();
                newIndividual.packages = master_package_list;
                std::shuffle(newIndividual.packages.begin(), newIndividual.packages.end(), e);
                newIndividual.Mutate();
                newIndividual.Pack();
                newIndividual.CalculateFitness();

                newPopulation.InsertIntoPopulation(newIndividual, &newPopulation.population);
            }
            populations.push_back(newPopulation);
        }

        /*  */
    }

    /* This is called once per frame */
    void inline Update(float dt)
    {
        /* Iterate Populations */
        for(int i = 0; i < NUM_POPULATIONS; i++)
        {
            populations[i].Iterate();
        }

        /* Rendering */
        Package current_package;
        Model* current_package_model;

        /* Reset visibility and position of models */
        for(int i = 0; i < master_package_list.size(); i++)
        {
            current_package = master_package_list[i];
            current_package_model = mPackages[current_package.id];
            current_package_model->ResetModel();
            current_package_model->ScaleModel(glm::vec3((float)current_package.width/10, (float)current_package.height/10, (float)current_package.length/10));
            current_package_model->TranslateModel(glm::vec3((float)current_package.width/20, (float)current_package.height/20, (float)current_package.length/20));
            current_package_model->SetModelVisibility(false);
        }

        /* Prepare best individual for rendering */
        Individual best_individual = populations[0].population[0];
        unsigned int package_index = 0;
        while(best_individual.packages[package_index].is_on_truck)
        {
            current_package = best_individual.packages[package_index];
            current_package_model = mPackages[current_package.id];
            current_package_model->TranslateModel(current_package.pos/(float)10);
            current_package_model->SetModelVisibility(true);

            package_index++;
        }

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