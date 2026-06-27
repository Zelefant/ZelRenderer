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


BSPNode* BSPTree::GenerateBSP(std::vector<BSPVertex>& vertList)
{
    // Step 1: Choose Wall
    // Choose 5 random walls to test for least crossed heuristic

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

    return nullptr;
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