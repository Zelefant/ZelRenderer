#include "BSPTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

BSPTree::BSPTree(std::string map_file_path)
{
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
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(wall_indices.begin(), wall_indices.end(), g);

	// Determine number of crosses for the 5 random walls
	int numCrosses[5];
	for (int i = 0; i < 5; i++)
	{
		numCrosses[i] = 0;
		Wall wall = wallList[wall_indices[i]];
		for (int j = 0; j < wallList.size(); j++)
		{
			if (wall == wallList[j]) continue; // Don't check the wall we selected for crosses.

			// TODO. check cross.
		}
	}


}