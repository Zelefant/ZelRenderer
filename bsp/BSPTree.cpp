#include "BSPTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <stdexcept>

#include <json/json.hpp>

BSPTree::BSPTree(std::string map_file_path)
{
    // Set up random device.
    g = std::mt19937(rd());

    // Load map file and create map geometry object.
    MapGeometry geo = LoadMapGeometryFromFile(map_file_path);
    
}

/*
Loads map geometry (vertices, walls) from JSON map file.

*/
MapGeometry BSPTree::LoadMapGeometryFromFile(std::string file_path)
{
    // Load Map File into Fstream
    std::ifstream mapfile(file_path);

    // Read and parse JSON data
    using json = nlohmann::json;
    json mapdata = json::parse(mapfile);

    // Create list of vertex pointers
    std::vector<BSPVertex*> vertices(mapdata["vertices"].size(), nullptr);
    for (const auto& vertex : mapdata["vertices"])
    {
        int id = vertex["id"];
        float x = vertex["x"];
        float y = vertex["y"];

        vertices[id] = new BSPVertex(id, x, y);
    }

    // Create list of walls
    std::vector<Linedef*> linedefs = std::vector<Linedef*>(mapdata["walls"].size(), nullptr);
    for (const auto& line : mapdata["walls"])
    {
        int id = line["id"];
        int start = line["start"];
        int end = line["end"];

        BSPVertex* vert1 = vertices[start];
        BSPVertex* vert2 = vertices[end];

        if (vert1 != nullptr && vert2 != nullptr)
        {
            linedefs[id] = new Linedef(id, vert1, vert2);
        }
        else
        {
            // Throw error. The linedef references a nonexistent vertex index.
            throw std::invalid_argument("Fatal error during compile: Line references nonexistent vertex id");
        }
    }

    // Create list of sectors
    std::vector<Sector*> sectors = std::vector<Sector*>(mapdata["sectors"].size(), nullptr);
    for (const auto& sector : mapdata["sectors"])
    {
        int id = sector["id"];
        std::vector<Linedef*> walls;
        for (int wallID : sector["walls"])
        {
            if (linedefs[wallID] == nullptr)
            {
                // Throw error. The sector has an invalid wall.
                throw std::invalid_argument("Fatal error during compile: Sector references nonexistent linedef id");
            }
            walls.push_back(linedefs[wallID]);
        }
        float floorHeight = sector["floatHeight"];
        float ceilingHeight = sector["ceilingHeight"];

        sectors[id] = new Sector(id, walls, floorHeight, ceilingHeight);
    }

    return MapGeometry(
        std::move(vertices),
        std::move(linedefs),
        std::move(sectors)
    );
}


BSPNode* BSPTree::GenerateBSP(MapGeometry geo, int heuristic)
{
    // Step 1: Choose Wall
    // Choose X random walls to test for least crossed heuristic
    std::vector<int> wallIndices;
    for (const auto& wall : geo.linedefs)
    {
        wallIndices.push_back(wall->GetID());
    }

    std::shuffle(wallIndices.begin(), wallIndices.end(), g);

    // Determine crosses, front, and back for X random walls
    std::vector<Linedef*> crosses;
    std::vector<Linedef*> front;
    std::vector<Linedef*> back;
    
    int numCandidates = std::min(heuristic, (int)wallIndices.size());
    for (int i = 0; i < numCandidates; i++)
    {
        std::vector<Linedef*> currCrosses;
        std::vector<Linedef*> currFront;
        std::vector<Linedef*> currBack;

    }


    /*// Step 1: Choose Wall
    // Choose  random walls to test for least crossed heuristic

    std::vector<int> vert_indices;

    for (int i = 0; i < vertList.size(); i++)
    {
        vert_indices.push_back(i);
    }

    std::shuffle(vert_indices.begin(), vert_indices.end(), g);

    // Determine crosses, front and back for the 5 random walls
    std::vector<BSPVertex*> crosses[5];
    std::vector<BSPVertex*> front[5];
    std::vector<BSPVertex*> behind[5];

    int numCandidates = std::min(5, (int)vertList.size());

    for (int i = 0; i < numCandidates; i++)
    {
        crosses[i] = std::vector<BSPVertex*>();
        front[i] = std::vector<BSPVertex*>();
        behind[i] = std::vector<BSPVertex*>();

        int vertIndex = vert_indices[i];

        BSPVertex& vert1 = vertList[vertIndex];
        BSPVertex& vert2 = vertList[(vertIndex + 1) % vertList.size()];

        for (int j = 0; j < vertList.size(); j++)
        {
            int crossIndex = j;

            BSPVertex& cross1 = vertList[crossIndex];
            BSPVertex& cross2 = vertList[(crossIndex + 1) % vertList.size()];

            if (vert1 == cross1)
                continue;

            // Check cross

            float firstHalf =
                (vert2.getX() - vert1.getX()) *
                (cross1.getY() - vert1.getY())
                -
                (cross1.getX() - vert1.getX()) *
                (vert2.getY() - vert1.getY());

            float secondHalf =
                (vert2.getX() - vert1.getX()) *
                (cross2.getY() - vert1.getY())
                -
                (cross2.getX() - vert1.getX()) *
                (vert2.getY() - vert1.getY());

            if (firstHalf * secondHalf <= 0)
            {
                // Wall crosses
                crosses[i].push_back(&cross1);
            }
            else if (firstHalf > 0 && secondHalf > 0)
            {
                // Wall is in front
                front[i].push_back(&cross1);
            }
            else
            {
                // Wall is behind
                behind[i].push_back(&cross1);
            }
        }
    }

    // Determine which wall has the least crosses and use that
    int vertIndex = -1;
    int numCrosses = INT_MAX;

    for (int i = 0; i < 5; i++)
    {
        if (crosses[i].size() < numCrosses)
        {
            vertIndex = i;
            numCrosses = crosses[i].size();
        }
    }

    // Split and create new shapes

    // Create BSP Nodes

    return nullptr;*/
}


/*
Check cross helper function.

Returns:
If the line and check are the same line: -1
If line passes through check: 0
If check is in front of line: 1
If check is behind line: 2
*/
int CheckCross(Linedef* line, Linedef* check)
{
    if (line->Start() == check->Start() && line->End() == check->End())
    {
        return -1;
    }

    int x1 = check->Start()->getX();
    int y1 = check->Start()->getY();

    int x2 = check->End()->getX();
    int y2 = check->End()->getY();

    int x3 = line->Start()->getX();
    int y3 = line->Start()->getY();

    int x4 = line->End()->getX();
    int y4 = line->End()->getY();

    float firstHalf = (x4 - x3) * (y1 - y3) - (x1 - x3) * (y4 - y3);

    float secondHalf = (x4 - x3) * (y2 - y3) - (x2 - x3) * (y4 - y3);

    if (firstHalf * secondHalf <= 0)
    {
        // Wall crosses
        return 0;
    }
    else if (firstHalf > 0 && secondHalf > 0)
    {
        // Wall is in front
        return 1;
    }
    else
    {
        // Wall is behind
        return 2;
    }

}

void BSPTree::CreateNewShape(
    std::vector<BSPVertex*>* shape1,
    std::vector<BSPVertex*>* shape2,
    std::vector<BSPVertex*> cross,
    std::vector<BSPVertex*> front,
    std::vector<BSPVertex*> back
)
{

}