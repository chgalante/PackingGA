#include "../include/Primitive.hpp"

#define M_PI          3.141592653589793238462643383279502884L /* pi */

Geometry* Primitive::Plane(void)
{
    Geometry* Plane = new Geometry();
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<int> indices = std::vector<int>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> texture_coordinates = std::vector<glm::vec2>();

    vertices.push_back(glm::vec3(Unit, 0, -Unit)); // 0
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(10.0f, 10.0f));

    vertices.push_back(glm::vec3(-Unit, 0, -Unit)); // 1
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 10.0f));

    vertices.push_back(glm::vec3(-Unit, 0, Unit)); // 2
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));

    vertices.push_back(glm::vec3(Unit, 0, Unit)); // 3
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(10.0f, 0.0f));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    Plane->SetVertices(vertices);
    Plane->SetIndices(indices);
    Plane->SetNormals(normals);
    Plane->SetTextureCoordinates(texture_coordinates);
    Plane->Activate();

    return Plane;
}

Geometry* Primitive::Cube(void)
{
    Geometry* Cube = new Geometry();
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<int> indices = std::vector<int>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> texture_coordinates = std::vector<glm::vec2>();
    // Cube Primitive
    // Back Vertices
    vertices.push_back(glm::vec3(-(Unit/2), 0.0f, -(Unit / 2)));	        // Bottom-Left-North		0
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3(-(Unit/2), 0.0f, -(Unit / 2)));	        // Bottom-Left-West	        1
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3(-(Unit/2), 0.0f, -(Unit / 2)));	        // Bottom-Left-Down	        2
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 1.0f));

    vertices.push_back(glm::vec3((Unit / 2), 0.0f, -(Unit / 2)));			// Bottom-Right-North		3
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));    
    vertices.push_back(glm::vec3((Unit / 2), 0.0f, -(Unit / 2)));			// Bottom-Right-East		4
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3((Unit / 2), 0.0f, -(Unit / 2)));			// Bottom-Right-Down		5
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));

    vertices.push_back(glm::vec3((Unit / 2), Unit, -(Unit / 2)));		// Top-Right-North		    6
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3((Unit / 2), Unit, -(Unit / 2)));		// Top-Right-East		    7
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3((Unit / 2), Unit, -(Unit / 2)));		// Top-Right-Up		        8
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 0.0f));

    vertices.push_back(glm::vec3(-(Unit / 2), Unit, -(Unit / 2)));		// Top-Left-North			9
    normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3(-(Unit / 2), Unit, -(Unit / 2)));		// Top-Left-West			10
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3(-(Unit / 2), Unit, -(Unit / 2)));		// Top-Left-Up			    11
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));

    // Front Vertices
    vertices.push_back(glm::vec3(-(Unit / 2), 0.0f, (Unit / 2)));			// Bottom-Left-South		12
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3(-(Unit / 2), 0.0f, (Unit / 2)));			// Bottom-Left-West		    13
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3(-(Unit / 2), 0.0f, (Unit / 2)));			// Bottom-Left-Down		    14
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));

    vertices.push_back(glm::vec3((Unit / 2), 0.0f, (Unit / 2)));			// Bottom-Right-South		15
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));
    vertices.push_back(glm::vec3((Unit / 2), 0.0f, (Unit / 2)));			// Bottom-Right-East		16
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 0.0f));
    vertices.push_back(glm::vec3((Unit / 2), 0.0f, (Unit / 2)));			// Bottom-Right-Down		17
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 0.0f));

    vertices.push_back(glm::vec3((Unit / 2), Unit, (Unit / 2)));		// Top-Right-South		    18
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3((Unit / 2), Unit, (Unit / 2)));		// Top-Right-East		    19
    normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3((Unit / 2), Unit, (Unit / 2)));		// Top-Right-Up		        20
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 1.0f));

    vertices.push_back(glm::vec3(-(Unit / 2), Unit, (Unit / 2)));		// Top-Left-South			21
    normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    texture_coordinates.push_back(glm::vec2(1.0f, 1.0f));
    vertices.push_back(glm::vec3(-(Unit / 2), Unit, (Unit / 2)));		// Top-Left-West			22
    normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 1.0f));
    vertices.push_back(glm::vec3(-(Unit / 2), Unit, (Unit / 2)));		// Top-Left-Up			    23
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 1.0f));

    // North Face
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(9);
    indices.push_back(9);
    indices.push_back(6);
    indices.push_back(3);

    // South Face
    indices.push_back(15);
    indices.push_back(18);
    indices.push_back(21);
    indices.push_back(21);
    indices.push_back(12);
    indices.push_back(15);

    // West Face
    indices.push_back(13);
    indices.push_back(22);
    indices.push_back(10);
    indices.push_back(10);
    indices.push_back(1);
    indices.push_back(13);

    // East Face
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(19);
    indices.push_back(19);
    indices.push_back(16);
    indices.push_back(4);

    // Up Face
    indices.push_back(20);
    indices.push_back(8);
    indices.push_back(11);
    indices.push_back(11);
    indices.push_back(23);
    indices.push_back(20);

    // Down Face
    indices.push_back(5);
    indices.push_back(17);
    indices.push_back(14);
    indices.push_back(14);
    indices.push_back(2);
    indices.push_back(5);

    Cube->SetVertices(vertices);
    Cube->SetIndices(indices);
    Cube->SetNormals(normals);
    Cube->SetTextureCoordinates(texture_coordinates);
    Cube->Activate();

    return Cube;
}

Geometry* Primitive::Sphere(unsigned int sectors, unsigned int stacks)
{
    Geometry* Sphere = new Geometry();
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<int> indices = std::vector<int>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> texture_coordinates = std::vector<glm::vec2>();

    float sector_step = 2 * M_PI / sectors;
    float stack_step = M_PI / stacks;
    float sector_angle, stack_angle;
    float radius = Unit/2;
    float x, y, z, zx;                                     
    float nx, ny, nz;                                       
    float u, v;                                             

    /* Vertices */
    for(unsigned int stack = 0; stack <= stacks; stack++)
    {
        stack_angle = (M_PI / 2) - (stack * stack_step);
        zx = radius * cosf(stack_angle);
        y = radius * sinf(stack_angle);

        for(unsigned int sector = 0; sector <= sectors; sector++)
        {
            // vertex position
            sector_angle = sector * sector_step;
            z = zx * cosf(sector_angle);
            x = zx * sinf(sector_angle);
            vertices.push_back(glm::vec3(x, y, z));

            // normalized vertex normal
            nx = x / radius;
            ny = y / radius;
            nz = z / radius;
            normals.push_back(glm::vec3(nx,ny,nz));

            // vertex texture coordinate
            u = float(sector) / sectors;
            v = float(stack) / stacks;
            texture_coordinates.push_back(glm::vec2(u, v));
        }
    }

    /* Vertex Indices */
    unsigned int it1, it2; /* iterators */
    unsigned int stack_offset = sectors + 1; /* How many vertices before the start of the next stack */
    for(int stack = 0; stack < stacks; ++stack)
    {
        it1 = stack * stack_offset; /* Start at first sector of each stack */
        it2 = it1 + stack_offset;  /* Start another iterator at the start of the stack after */

        /* List indices of vertices for each triangle defined by it1 and it2 */
        for(int sector = 0; sector < sectors; sector++, it1++, it2++)
        {
            if(stack != 0)
            {
                indices.push_back(it1);      // Top Left
                indices.push_back(it2);      // Bottom Left
                indices.push_back(it1 + 1);  // Top Right
            }

            if(stack != (stacks-1))
            {
                indices.push_back(it1 + 1); // Top Right
                indices.push_back(it2);     // Bottom Left
                indices.push_back(it2 + 1); // Bottom Right
            }
        }
    }

    Sphere->SetVertices(vertices);
    Sphere->SetIndices(indices);
    Sphere->SetNormals(normals);
    Sphere->SetTextureCoordinates(texture_coordinates);
    Sphere->Activate();

    return Sphere;
}

Geometry* Primitive::Line(void)
{
    Geometry* Line = new Geometry();
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
    std::vector<int> indices = std::vector<int>();
    std::vector<glm::vec3> normals = std::vector<glm::vec3>();
    std::vector<glm::vec2> texture_coordinates = std::vector<glm::vec2>();

    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // 0
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(0.0f, 0.0f));

    vertices.push_back(glm::vec3(Unit, 0.0f, 0.0f)); // 1
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    texture_coordinates.push_back(glm::vec2(Unit, 0.0f));

    indices.push_back(0);
    indices.push_back(1);

    Line->SetVertices(vertices);
    Line->SetIndices(indices);
    Line->SetNormals(normals);
    Line->SetTextureCoordinates(texture_coordinates);
    Line->Activate();

    return Line;
}

// Geometry* Primitive::InitializeGrid(void)
// {
//     // 100x100 Grid
//     std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
//     std::vector<unsigned int> indices = std::vector<unsigned int>();
//      std::vector<glm::vec3> normals = std::vector<glm::vec3>();
//     std::vector<glm::vec2> texture_coordinates = std::vector<glm::vec2>();
//     Geometry* Grid = new Geometry();

// 	// Z Lines
// 	float xCoord = -(Unit*100/2);
// 	for (unsigned int x = 0; x < 200; x += 2)
// 	{
// 		if (x != 0)
// 			xCoord += Unit;

// 		vertices.push_back(glm::vec3(xCoord, 0.0f, -(Unit * 100 / 2)));
//         normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
//         texture_coordinates.push_back(glm::vec2(0.0f));
// 		vertices.push_back(glm::vec3(xCoord, 0.0f, (Unit * 100 / 2)));
//         normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
//         texture_coordinates.push_back(glm::vec2(0.0f));
//         indices.push_back(x);
//         indices.push_back(x+1);
// 	}
// 	// X Lines
// 	float zCoord = -(Unit * 100 / 2);
// 	for (unsigned int z = 200; z < 400; z += 2)
// 	{
// 		if (z != 200)
// 			zCoord += Unit;

// 		vertices.push_back(glm::vec3(-(Unit * 100 / 2), 0.0f, zCoord));
//         normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
//         texture_coordinates.push_back(glm::vec2(0.0f));
// 		vertices.push_back(glm::vec3((Unit * 100 / 2), 0.0f, zCoord));
//         normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
//         texture_coordinates.push_back(glm::vec2(0.0f));
//         indices.push_back(z);
//         indices.push_back(z+1);
// 	};
//     Grid->SetVertices(vertices);
//     Grid->SetIndices(indices);
//     Grid->SetNormals(normals);
//     Grid->SetTextureCoordinates(texture_coordinates);
//     Grid->Activate();

//     return Grid;
// }