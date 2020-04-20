
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_map>

#include "../Engine/include/Scene.hpp"


/* Problem Globals */

#define NUM_PACKAGES                1000
#define MIN_PACKAGE_DIM             5
#define MAX_PACKAGE_DIM             50
#define MIN_PACKAGE_VALUE           5
#define MAX_PACKAGE_VALUE           100

#define TRUCK_WIDTH                 100
#define TRUCK_HEIGHT                100
#define TRUCK_LENGTH                300

#define POPULATION_SIZE             100
#define PARENT_POOL_SIZE            50
#define ELITE_POPULATION_SIZE       10
#define TOURNAMENT_WINDOW           4

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
    int              width;      // X
    int              height;     // Y
    int              length;     // Z
    float            value;      // $$$

    bool    is_on_truck;

    glm::vec3 pos;

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
    float                 fitness;     // Measure of fitness
    int         ***fitness_volume;
    int         **xy_map, **yz_map, **xz_map;

    Individual(int ***fit_vol)
    {
        this->packages = std::vector<Package>();
        this->fitness_volume = fit_vol;

        xy_map = (int **)malloc(TRUCK_WIDTH * sizeof(int *));
        for(int x= 0; x < TRUCK_WIDTH; x++)
        {
            xy_map[x] = (int *)malloc(TRUCK_HEIGHT * sizeof(int));
        }

        yz_map = (int **)malloc(TRUCK_HEIGHT * sizeof(int *));
        for(int y= 0; y < TRUCK_HEIGHT; y++)
        {
            yz_map[y] = (int *)malloc(TRUCK_LENGTH * sizeof(int));
        }

        xz_map = (int **)malloc(TRUCK_WIDTH * sizeof(int *));
        for(int x= 0; x < TRUCK_WIDTH; x++)
        {
            xz_map[x] = (int *)malloc(TRUCK_LENGTH * sizeof(int));
        }
    }

    /* Methods */

    /* DESCRIPTION: This function starts at first index of packages and places packages until there is overflow.
    */
    void Pack()
    {
        /* Reset packages (For offspring who've inherited data from parents) */
        for(int p = 0; p < packages.size(); p++)
        {
            packages[p].pos = glm::vec3(0.0f);
            packages[p].is_on_truck = false;
        }

        /* Initialize Depth Maps */
        for(int x= 0; x < TRUCK_WIDTH; x++)
        {
            for(int y=0; y < TRUCK_HEIGHT; y++)
            {
                xy_map[x][y] = 0; /* Back of truck */
            }
        }
        for(int y= 0; y < TRUCK_HEIGHT; y++)
        {
            for(int z=0; z < TRUCK_LENGTH; z++)
            {
                yz_map[y][z] = TRUCK_WIDTH; /* Right Wall of truck */
            }
        }
        for(int x= 0; x < TRUCK_WIDTH; x++)
        {
            for(int z=0; z < TRUCK_LENGTH; z++)
            {
                xz_map[x][z] = 0; /* Bottom of truck */
            }
        }

        /* Loop through packages */
        int package_right_bound, package_top_bound;
        int pos_x, pos_y, pos_z;
        int depth_x;

        for(int p = 0; p < NUM_PACKAGES; p++)
        {
            package_right_bound = packages[p].width;
            package_top_bound = packages[p].height;

            /* Determine Z */
            pos_z = 0;
            for(int x = 0; x < package_right_bound; x++)
            {
                for(int y = 0; y < package_top_bound; y++)
                {
                    if(xy_map[x][y] > pos_z) pos_z = xy_map[x][y];
                }
            }

            /* Determine X */
            package_right_bound = pos_z + packages[p].length;

            if(package_right_bound > TRUCK_LENGTH) break;

            depth_x = TRUCK_WIDTH;
            for(int y = 0; y < package_top_bound; y++)
            {
                for(int z = pos_z; z < package_right_bound; z++)
                {
                    if(yz_map[y][z] < depth_x) depth_x = yz_map[y][z];
                }
            }
            pos_x = depth_x - packages[p].width;

            if(pos_x < 0) break;

            /* place package */
            packages[p].pos = glm::vec3(pos_x*1.0f, 0.0f, pos_z*1.0f);
            packages[p].is_on_truck = true;

            /* Update Depth Maps */
            for(int x = pos_x; x < pos_x + packages[p].width; x++)
            {
                for(int y = 0; y < package_top_bound; y++)
                {
                    xy_map[x][y] = pos_z + packages[p].length;
                }
            }
            for(int y = 0; y < package_top_bound; y++)
            {
                for(int z = pos_z; z < package_right_bound; z++)
                {
                    yz_map[y][z] = pos_x;
                }
            }

            /* Try stacking packages until next package is too big */
            pos_y = packages[p].height;
            while( p < NUM_PACKAGES && 
                   packages[p+1].width < packages[p].width &&
                   packages[p+1].length < packages[p].length &&
                   pos_y + packages[p+1].height < TRUCK_HEIGHT)
            {
                packages[p+1].pos = glm::vec3(pos_x*1.0f, pos_y*1.0f, pos_z*1.0f);
                packages[p+1].is_on_truck = true;
                pos_y += packages[p+1].height;
                p++;
            }
        }

    }

    /* DESCRIPTION: 
    */
    void Mutate(int mutationChance)
    {
        /* Rotate packages at random */
        for(int i = 0; i < NUM_PACKAGES; i++)
        {
            if((rand() % 100) < mutationChance)
            {
                packages[rand() % NUM_PACKAGES].Rotate((Axis)(rand() % 3));
            }

            /* Swap two random packages */
            unsigned int rand_index1, rand_index2;
            Package temp;
            if((rand() % 100) < mutationChance)
            {
                rand_index1 = rand() % NUM_PACKAGES;
                rand_index2 = rand_index1;
                while(rand_index2 == rand_index1) rand_index2 = rand() % NUM_PACKAGES;

                temp = packages[rand_index1];
                packages[rand_index1] = packages[rand_index2];
                packages[rand_index2] = temp;
            }
        }
    }

    /* DESCRIPTION: 
    */
    void CalculateFitness()
    {
        /* Clear Fitness Volume*/
        for( int x = 0; x < TRUCK_WIDTH; x++)
        {
            for(int y = 0; y < TRUCK_HEIGHT; y++)
            {
                for(int z = 0; z < TRUCK_LENGTH; z++)
                {
                    fitness_volume[x][y][z] = 0;
                }
            }
        }

        
        unsigned int start_x, end_x;
        unsigned int start_y, end_y;
        unsigned int start_z, end_z;
        Package current_package;
        float value_on_truck = 0.0f;
        int filled_volume = 0;
        int overlapped_volume = 0;
        int exceeding_volume = 0;

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

                for(int x = start_x; x < end_x; x++)
                {
                    for(int y = start_y; y < end_y; y++)
                    {
                        for(int z = start_z; z < end_z; z++)
                        {
                            fitness_volume[x][y][z] += 1;
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

        for(int x = 0; x < TRUCK_WIDTH; x++)
        {
            for(int y = 0; y < TRUCK_HEIGHT; y++)
            {
                for(int z = 0; z < TRUCK_LENGTH; z++)
                {
                    if(fitness_volume[x][y][z] > 0) filled_volume++;
                    if(fitness_volume[x][y][z] > 1) overlapped_volume++;
                }
            }
        }

        /* Greater fitness for greater volume filled */
        fitness = (float)(filled_volume / (float)(TRUCK_WIDTH * TRUCK_HEIGHT * TRUCK_LENGTH));
        /* Lesser fitness for greater volume overlapped*/
        fitness = fitness - (float)(overlapped_volume / (float)(TRUCK_WIDTH * TRUCK_HEIGHT * TRUCK_LENGTH));
        /* Greater fitness for greater value per volume (fraction of maximum possible value) */
        fitness = fitness + (float)(value_on_truck / (float)(TRUCK_WIDTH * TRUCK_HEIGHT * TRUCK_LENGTH * MAX_PACKAGE_VALUE));
    }
};

struct Population
{
    std::vector<Individual> population;
    std::vector<Individual> parents;
    std::vector<Package> master_package_list = std::vector<Package>();
    int ***fitness_volume;
    int **diversity_matrix;
    float diversity = 0;
    float target_diversity = 0;
    int mutation_chance = 5;
    int num_generations = 0;
    int num_runs = 0;

    Population()
    {
        fitness_volume = (int ***)malloc(TRUCK_WIDTH * sizeof(int **));
        for(int x=0; x < TRUCK_WIDTH; x++)
        {
            fitness_volume[x] = (int **)malloc(TRUCK_HEIGHT * sizeof(int *));
            for(int y=0; y < TRUCK_HEIGHT; y++)
            {
                fitness_volume[x][y] = (int *)malloc(TRUCK_LENGTH * sizeof(int));
            }
        }

        /* Initialize 2D Array for diversity calculations */
        diversity_matrix = (int **)malloc((NUM_PACKAGES + 1) * sizeof(int *)); 
        for (int i=0; i <= NUM_PACKAGES; i++)
        {
            diversity_matrix[i] = (int *)malloc((NUM_PACKAGES + 1) * sizeof(int));
        }
    }

    /* Methods */

    Individual NewIndividual(void)
    {
        return Individual(fitness_volume);
    }

    void Initialize(void)
    {
        population = std::vector<Individual>();
        parents = std::vector<Individual>();

        /* Generate Population */
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine e(seed);
        Individual newIndividual(nullptr);

        diversity = 0;
        target_diversity = 0;
        mutation_chance = 5;
        num_generations = 0;

        for(int j = 0; j < POPULATION_SIZE; j++)
        {
            /* Individuals in the population will have a shuffled package list from the master package list */
            newIndividual = Individual(fitness_volume);
            newIndividual.packages = master_package_list;
            std::shuffle(newIndividual.packages.begin(), newIndividual.packages.end(), e);
            
            newIndividual.Mutate(mutation_chance);
            newIndividual.Pack();
            newIndividual.CalculateFitness();

            InsertIntoPopulation(newIndividual, &population);
        }
        num_runs++;
    }
    /* DESCRIPTION: This function runs a genetic algorithm on the population for one iteration.
    */
    void Iterate(void)
    {
        /* Check for genetic algorithm restart */
        if(num_generations == 0)
            Initialize();

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

        CalculatePopulationDiversity();

        /* increment number of generations */
        num_generations++;

        /* Set the target diversity to the maximum diversity acheived */
        if(target_diversity < diversity) target_diversity = diversity;

        /* Adjust mutation chance with respect to population diversity */
        float frac_diversity = diversity/target_diversity;
        mutation_chance = (100*(1-frac_diversity));
        if(mutation_chance > 100) mutation_chance = 100;
        if(mutation_chance < 0) mutation_chance = 0;

        std::cout << "RUN # " << num_runs << " <--> Best Fitness: " << population[0].fitness << " // Diversity: " << diversity << std::endl;

        /* Export to CSV */
	    std::string fileName = "run_data.csv";
	    std::fstream outfile = std::fstream();
        outfile.open(fileName, std::ios::app);
	    outfile << num_generations << "," << population[0].fitness << ","<< diversity << std::endl;
	    outfile.close();

        /* Restart the genetic algorithm */
        if(num_generations == 100)
            num_generations = 0;
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

        Individual offspring = Individual(fitness_volume);
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
        offspring.Mutate(mutation_chance);
        offspring.Pack();
        offspring.CalculateFitness();
        /* Add offspring to population */
        InsertIntoPopulation(offspring, &population);
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

    /* DESCRIPTION: Measure how different individuals in the population are compared to the best fitness.
    */
    void CalculatePopulationDiversity()
    {
        /* Minimum Edit Distance */
        /* Get average MED for whole population compared to the best individual */
        for(int indiv = 1; indiv < POPULATION_SIZE; indiv++)
        {
            /* Initialize the 2D array to 0 */
            for (int i = 0; i <= NUM_PACKAGES; i++)
            {
                for(int j = 0; j <= NUM_PACKAGES; j++)
                {
                    diversity_matrix[i][j] = 0;
                }
            }

            for (int row = 0; row <= NUM_PACKAGES; row++)
            {
                diversity_matrix[row][0] = row;
            }

            for (int col = 0; col <= NUM_PACKAGES; col++)
            {
                diversity_matrix[0][col] = col;
            }

            for(int row = 0; row <= NUM_PACKAGES; row++)
            {
                for(int col = 0; col <= NUM_PACKAGES; col++)
                {
                    if(row != 0 && col != 0)
                    {
                        if(population[indiv].packages[row-1].id == population[0].packages[col-1].id)
                        {
                            diversity_matrix[row][col] = diversity_matrix[row-1][col-1];
                        }
                        else
                        {
                            diversity_matrix[row][col] = std::min<int>(diversity_matrix[row-1][col] + 1, 
                                                                    std::min<int>(diversity_matrix[row][col-1] + 1,
                                                                                    diversity_matrix[row-1][col-1] + 2)
                                                                        );
                        }
                        
                    }
                }
            }
            diversity += (float)diversity_matrix[NUM_PACKAGES][NUM_PACKAGES];
            diversity = (float)(diversity / (POPULATION_SIZE - 1));
        }
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

    Population population = Population();

    Model* mTruck;
    Model *mAxisX, *mAxisY, *mAxisZ;
    std::unordered_map<int, Model*> mPackages = std::unordered_map<int, Model*>();

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
        SetCameraView(glm::vec3((float)TRUCK_WIDTH/2, (float)TRUCK_HEIGHT/20, (float)TRUCK_LENGTH/20), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        SetCameraPerspective(70.0f, width / height, 0.001*Unit, 200*Unit);
        SetCameraBoundingBox(-100.0f, 100.0f, -100.0f, 100.0f);

        /* Lighting */
        SetSceneLightPosition(glm::vec3(200.0f, 200.0f, 200.0f));
        SetSceneLightDirection(-GetSceneLightPosition());
        SetSceneLightCutoff(glm::cos(glm::radians(180.0f)));
        SetSceneLightSwitch(true);

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
            population.master_package_list.push_back(newPackage);

            /* Create a model for this package for rendering*/
            newPackageModel = CreateModelPrimitive(CUBE, NULL);
            newPackageModel->SetModelVisibility(false);
            newPackageModel->SetModelTransparency(false);
            newPackageModel->SetModelFragmentColour(glm::vec4(0.0f, value * 0.01f ,0.0f, 1.0f));
            mPackages.insert(std::pair<int, Model*>(newPackage.id, newPackageModel));
        }

        /* Initialize Truck Model for Rendering */
        mTruck = CreateModelPrimitive(CUBE, NULL);
        mTruck->ScaleModel(glm::vec3((float)TRUCK_WIDTH/10, (float)TRUCK_HEIGHT/10, (float)TRUCK_LENGTH/10));
        mTruck->TranslateModel(glm::vec3((float)TRUCK_WIDTH/20, 0.0f, (float)TRUCK_LENGTH/20));
        //mTruck->SetModelRenderMode(GL_LINE_LOOP);
        mTruck->SetModelTransparency(true);
        mTruck->SetModelFragmentColour(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));
    }

    /* This is called once per frame */
    void inline Update(float dt)
    {
        InputController(window);

        /* Iterate Populations */
        population.Iterate();

        /* Rendering */
        Package current_package;
        Model* current_package_model;

        /* Reset visibility and position of models */
        for(int i = 0; i < population.master_package_list.size(); i++)
        {
            current_package = population.master_package_list[i];
            current_package_model = mPackages[current_package.id];
            current_package_model->ResetModel();
            current_package_model->ScaleModel(glm::vec3((float)current_package.width/10, (float)current_package.height/10, (float)current_package.length/10));
            current_package_model->TranslateModel(glm::vec3((float)current_package.width/20, 0.0f, (float)current_package.length/20));
            current_package_model->SetModelVisibility(false);
        }

        /* Prepare best individual for rendering */
        Individual best_individual = population.population[0];
        unsigned int package_index = 0;
        while(best_individual.packages[package_index].is_on_truck)
        {
            current_package = best_individual.packages[package_index];
            current_package_model = mPackages[current_package.id];
            current_package_model->TranslateModel(current_package.pos/10.0f);
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
        if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            SetCameraView(glm::vec3((float)TRUCK_WIDTH/2, (float)TRUCK_HEIGHT/20, (float)TRUCK_LENGTH/20), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            SetCameraView(glm::vec3((float)TRUCK_WIDTH/20, (float)TRUCK_HEIGHT/20, (float)TRUCK_LENGTH/10 + TRUCK_LENGTH/16), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            SetCameraView(glm::vec3(-(float)TRUCK_WIDTH/2, (float)TRUCK_HEIGHT/20, (float)TRUCK_LENGTH/20), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            SetCameraView(glm::vec3((float)TRUCK_WIDTH/20, (float)TRUCK_HEIGHT/20, -(float)TRUCK_LENGTH/16), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            SetCameraView(glm::vec3((float)TRUCK_WIDTH/20, (float)TRUCK_HEIGHT/2, (float)TRUCK_LENGTH/20), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        }
    }
};