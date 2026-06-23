#include "BSPTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

BSPTree::BSPTree(std::string map_file_path)
{
	// Set up random device.
	g = std::mt19937(rd());

	// Load the map file and create an array containing every wall.
	std::ifstream map_file(map_file_path);
	std::string v1s, v2s, v3s, v4s;
	std::vector<Wall> wallList = std::vector<Wall>();
	
	bool eof = false;
	while (!eof)
	{
		float v1, v2, v3, v4;
		std::getline(map_file, v1s);
		std::getline(map_file, v2s);
		if (std::getline(map_file, v3s))
		{
			// Get 2nd vertex and assign vars for wall
			std::getline(map_file, v4s);
			try
			{
				v1 = std::stof(v1s);
				v2 = std::stof(v2s);
				v3 = std::stof(v3s);
				v4 = std::stof(v4s);
			}
			catch (const std::invalid_argument& e)
			{
				std::cerr << "Error: Invalid map file\n";
				return;
			}
		}
		else
		{
			// EOF - Make wall connect back to first vertex.
			v1 = std::stof(v1s);
			v2 = std::stof(v2s);
			v3 = wallList[0].vert1.x;
			v4 = wallList[0].vert1.y;
			eof = true;
		}

		// Create the wall from the vertices
		Wall wall = Wall(glm::vec2(v1, v2), glm::vec2(v3, v4));
		wallList.push_back(wall);
	}

	// CRITICAL : DO NOT MODIFY THE WALL LIST AFTER THIS POINT.
	// These are pointers to the original wall list vector, it should not be changed anymore.
	this->root = GenerateBSP(wallList);
}

BSPNode* BSPTree::GenerateBSP(std::vector<Wall> wallList)
{
	// Step 1: Choose Wall
	// Choose 5 random walls to test for least crossed heuristic
	// First create index list
	std::vector<int> wall_indices = std::vector<int>();
	for (int i = 0; i < wallList.size(); i++)
	{
		wall_indices.push_back(i);
	}

	// Shuffle index list (will use the 5 front indices in the shuffled list)

	std::shuffle(wall_indices.begin(), wall_indices.end(), g);

	// Determine crosses, front and back for the 5 random walls
	std::vector<Wall*> crosses[5];
	std::vector<Wall*> front[5];
	std::vector<Wall*> behind[5];

	int numCandidates = std::min(5, (int)wallList.size()); // Avoids edge case where number of walls is less than 5

	for (int i = 0; i < numCandidates; i++)
	{
		crosses[i] = std::vector<Wall*>();
		front[i] = std::vector<Wall*>();
		behind[i] = std::vector<Wall*>();
		Wall& wall = wallList[wall_indices[i]];
		for (int j = 0; j < wallList.size(); j++)
		{
			Wall& crossWall = wallList[j];
			if (wall == crossWall) continue; // Don't check the wall we selected for crosses.

			// Check cross. 
			// Formula:
			// Line segment endpoints: p1 = (x1, y1) and p2 = (x2, y2)
			// Points on line: p3 = (x3, y3) and p4 = (x4, y4) (These are just the wall vertices)
			// If and only if this inequality is satisfied does the line segment cross the line:
			// ((x4 - x3)(y1 - y3) - (x1 - x3)(y4 - y3)) * ((x4 - x3)(y2 - y3) - (x2 - x3)(y4 - y3)) <= 0
			float firstHalf = (wall.vert2.x - wall.vert1.x) * (crossWall.vert1.y - wall.vert1.y) - (crossWall.vert1.x - wall.vert1.x) * (wall.vert2.y - wall.vert1.y);
			float secondHalf = (wall.vert2.x - wall.vert1.x) * (crossWall.vert2.y - wall.vert1.y) - (crossWall.vert2.x - wall.vert1.x) * (wall.vert2.y - wall.vert1.y);
			if (firstHalf * secondHalf <= 0)
			{
				// If the above inequality is satisfied, the wall crosses, add to crosses.
				crosses[i].push_back(&crossWall);
			}
			else if (firstHalf > 0 && secondHalf > 0)
			{
				// Wall is in front of line.
				front[i].push_back(&crossWall);
			}
			else
			{
				// Wall is behind line.
				behind[i].push_back(&crossWall);
			}
		}
	}

	// Determine which wall has the least crosses and use that for 
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

}

void BSPTree::CreateNewShape(
	std::vector<Wall*>* shape1,
	std::vector<Wall*>* shape2, 
	std::vector<Wall*> cross, 
	std::vector<Wall*> front, 
	std::vector<Wall*> back
)
{

}