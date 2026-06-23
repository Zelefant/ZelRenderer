#include "BSPTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

BSPTree::BSPTree(std::string map_file_path)
{
    // Set up random device.
    g = std::mt19937(rd());

    // Load map file and create vertex list.
    std::ifstream map_file(map_file_path);
    std::string xs, ys;

    std::vector<BSPVertex> vertexList;

    while (std::getline(map_file, xs))
    {
        if (!std::getline(map_file, ys))
        {
            std::cerr << "Error: Invalid map file\n";
            return;
        }

        try
        {
            float x = std::stof(xs);
            float y = std::stof(ys);

            vertexList.push_back(BSPVertex(x, y));
        }
        catch (const std::invalid_argument&)
        {
            std::cerr << "Error: Invalid map file\n";
            return;
        }
    }

    // CRITICAL : DO NOT MODIFY THE VERTEX LIST AFTER THIS POINT.
    this->root = GenerateBSP(vertexList);
}

BSPNode* BSPTree::GenerateBSP(std::vector<BSPVertex>& wallList)
{
    // Step 1: Choose Wall
    // Choose 5 random walls to test for least crossed heuristic

    std::vector<int> wall_indices;

    for (int i = 0; i < wallList.size(); i++)
    {
        wall_indices.push_back(i);
    }

    std::shuffle(wall_indices.begin(), wall_indices.end(), g);

    // Determine crosses, front and back for the 5 random walls
    std::vector<BSPVertex*> crosses[5];
    std::vector<BSPVertex*> front[5];
    std::vector<BSPVertex*> behind[5];

    int numCandidates = std::min(5, (int)wallList.size());

    for (int i = 0; i < numCandidates; i++)
    {
        crosses[i] = std::vector<BSPVertex*>();
        front[i] = std::vector<BSPVertex*>();
        behind[i] = std::vector<BSPVertex*>();

        int wallIndex = wall_indices[i];

        BSPVertex& wall1 = wallList[wallIndex];
        BSPVertex& wall2 = wallList[(wallIndex + 1) % wallList.size()];

        for (int j = 0; j < wallList.size(); j++)
        {
            int crossIndex = j;

            BSPVertex& cross1 = wallList[crossIndex];
            BSPVertex& cross2 = wallList[(crossIndex + 1) % wallList.size()];

            if (wall1 == cross1)
                continue;

            // Check cross

            float firstHalf =
                (wall2.getX() - wall1.getX()) *
                (cross1.getY() - wall1.getY())
                -
                (cross1.getX() - wall1.getX()) *
                (wall2.getY() - wall1.getY());

            float secondHalf =
                (wall2.getX() - wall1.getX()) *
                (cross2.getY() - wall1.getY())
                -
                (cross2.getX() - wall1.getX()) *
                (wall2.getY() - wall1.getY());

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
    int wallIndex = -1;
    int numCrosses = INT_MAX;

    for (int i = 0; i < 5; i++)
    {
        if (crosses[i].size() < numCrosses)
        {
            wallIndex = i;
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