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
    
    // Generate BSP from map geometry walls.
    this->root = GenerateBSP(geo.linedefs);
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
        float floorHeight = sector["floorHeight"];
        float ceilingHeight = sector["ceilingHeight"];

        sectors[id] = new Sector(id, walls, floorHeight, ceilingHeight);
    }

    return MapGeometry(
        std::move(vertices),
        std::move(linedefs),
        std::move(sectors)
    );
}


BSPNode* BSPTree::GenerateBSP(std::vector<Linedef*>& linedefs, int heuristic)
{
    // Step 0: Base Case
    if (linedefs.empty()) return nullptr;
    if (linedefs.size() == 1) return new BSPNode(linedefs[0]);

    // Step 1: Choose Wall
    // Choose X random walls to test for least crossed heuristic
    std::vector<int> wallIndices;
    for (const auto& wall : linedefs)
    {
        wallIndices.push_back(wall->GetID());
    }

    std::shuffle(wallIndices.begin(), wallIndices.end(), g);

    // Determine crosses, front, and back for X random walls
    int bestWallIndex = -1;
    std::vector<Linedef*> crosses;
    std::vector<Linedef*> bestFront;
    std::vector<Linedef*> bestBack;
    int score = INT_MAX;
    
    int numCandidates = std::min(heuristic, (int)wallIndices.size());
    for (int i = 0; i < numCandidates; i++)
    {
        Linedef* curr = linedefs[wallIndices[i]];
        std::vector<Linedef*> currCrosses;
        std::vector<Linedef*> currFront;
        std::vector<Linedef*> currBack;

        for (int j = 0; j < linedefs.size(); j++)
        {
            Side result = CheckCross(curr, linedefs[j]);
            if (result == Side::Coplanar || result == Side::Front)
            {
                currFront.push_back(linedefs[j]);
            }
            else if (result == Side::Back)
            {
                currBack.push_back(linedefs[j]);
            }
            else
            {
                currCrosses.push_back(linedefs[j]);
            }
        }

        // Balance score heuristic - will try to find most balanced tree + least crosses.
        int currScore = currCrosses.size() * 8 + abs((int)currFront.size() - (int)currBack.size());
        if (currScore < score)
        {
            crosses = currCrosses;
            bestFront = currFront;
            bestBack = currBack;

            score = currScore;
            bestWallIndex = wallIndices[i];
        }
    }

    // Step 2: Build new shapes.
    Linedef* splitter = linedefs[bestWallIndex];

    std::vector<Linedef*> finalFront;
    std::vector<Linedef*> finalBack;

    for (int i = 0; i < linedefs.size(); i++)
    {
        Linedef* line = linedefs[i];

        if (i == bestWallIndex)
            continue;

        Side result = CheckCross(splitter, line);

        if (result == Side::Front || result == Side::Coplanar)
        {
            finalFront.push_back(line);
        }
        else if (result == Side::Back)
        {
            finalBack.push_back(line);
        }
        else
        {
            // Only spanning lines are split
            SplitLine(line, splitter, finalFront, finalBack);
        }
    }

    // Step 3: Build nodes and recurse.
    BSPNode* node = new BSPNode(splitter);

    node->leftNode = GenerateBSP(finalFront, heuristic);
    node->rightNode = GenerateBSP(finalBack, heuristic);

    return node;
}


/*
Check cross helper function.
*/
Side BSPTree::CheckCross(Linedef* line, Linedef* check)
{
    if ((line->Start() == check->Start() && line->End() == check->End())
        ||
        (line->Start() == check->End() && line->End() == check->Start()))
    {
        return Side::Coplanar;
    }

    // Compute dot products
    float x1 = check->Start()->getX();
    float y1 = check->Start()->getY();

    float x2 = check->End()->getX();
    float y2 = check->End()->getY();

    float x3 = line->Start()->getX();
    float y3 = line->Start()->getY();

    float x4 = line->End()->getX();
    float y4 = line->End()->getY();

    float d1 = (x4 - x3) * (y1 - y3) - (x1 - x3) * (y4 - y3);

    float d2 = (x4 - x3) * (y2 - y3) - (x2 - x3) * (y4 - y3);

    // Epsilon fix for float precision
    const float EPS = 1e-5f;

    // Perform check
    if (fabs(d1) < EPS && fabs(d2) < EPS)
    {
        return Side::Coplanar;
    }
    else if (d1 > EPS && d2 > EPS)
    {
        return Side::Front;
    }
    else if (d1 < -EPS && d2 < -EPS)
    {
        return Side::Back;
    }
    else
    {
        return Side::Spanning;
    }

}

BSPVertex* BSPTree::Intersect(Linedef* line, Linedef* splitter)
{
    float x1 = line->Start()->getX();
    float y1 = line->Start()->getY();
    float x2 = line->End()->getX();
    float y2 = line->End()->getY();

    float x3 = splitter->Start()->getX();
    float y3 = splitter->Start()->getY();
    float x4 = splitter->End()->getX();
    float y4 = splitter->End()->getY();

    float denom =
        (x1 - x2) * (y3 - y4) -
        (y1 - y2) * (x3 - x4);

    const float EPS = 1e-6f;
    if (fabs(denom) < EPS)
    {
        return nullptr; // parallel or invalid intersection
    }

    float px =
        ((x1 * y2 - y1 * x2) * (x3 - x4) -
            (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;

    float py =
        ((x1 * y2 - y1 * x2) * (y3 - y4) -
            (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;

    BSPVertex* v = new BSPVertex(nextVertexID++, px, py);
    splitVertices.push_back(v); // for safe memory handling
    return v;
}

void BSPTree::SplitLine(
    Linedef* line,
    Linedef* splitter,
    std::vector<Linedef*>& front,
    std::vector<Linedef*>& back)
{
    BSPVertex* i = Intersect(line, splitter);
    if (!i)
        return;

    BSPVertex* a = line->Start();
    BSPVertex* b = line->End();

    // Compute which side 'a' (the start vertex) is on relative to the splitter.
    float x3 = splitter->Start()->getX();
    float y3 = splitter->Start()->getY();
    float x4 = splitter->End()->getX();
    float y4 = splitter->End()->getY();

    float d = (x4 - x3) * (a->getY() - y3) - (a->getX() - x3) * (y4 - y3);

    // If 'a' is on the front side, the a->i fragment is front and i->b is back.
    // If 'a' is on the back side, it's reversed.
    const float EPS = 1e-5f;

    Linedef* f = new Linedef(nextLineID++, a, i);
    Linedef* ba = new Linedef(nextLineID++, i, b);
    splitLinedefs.push_back(f);
    splitLinedefs.push_back(ba);
    if (d > EPS) 
    {
        front.push_back(f);
        back.push_back(ba);
    }
    else if (d < -EPS) 
    { 
        front.push_back(ba);
        back.push_back(f);
    }
    else {
        // 'a' is essentially on the splitter line itself — degenerate split,
        // just push the whole line to front to avoid a zero-length fragment.
        front.push_back(line);
    }
}

bool BSPTree::CompileBSPIntoFile(std::string path) {
    std::ofstream out(path, std::ios::binary);

    // Collect nodes via DFS, assign indices
    std::vector<BSPNode*> ordered;
    std::function<void(BSPNode*)> collect = [&](BSPNode* n) {
        if (!n) return;
        ordered.push_back(n);
        collect(n->leftNode);
        collect(n->rightNode);
        };
    collect(root);

    // Build index map
    std::unordered_map<BSPNode*, int32_t> indexMap;
    for (int i = 0; i < ordered.size(); i++)
        indexMap[ordered[i]] = i;

    // Write header
    out.write("BSP1", 4);
    // ... write counts, then blocks

    // Write nodes
    for (auto* node : ordered) {
        int32_t lid = node->GetLinedef()->GetID();
        int32_t left = node->leftNode ? indexMap[node->leftNode] : -1;
        int32_t right = node->rightNode ? indexMap[node->rightNode] : -1;
        out.write((char*)&lid, 4);
        out.write((char*)&left, 4);
        out.write((char*)&right, 4);
    }
}

BSPTree::~BSPTree()
{
    // Delete the node tree recursively
    std::function<void(BSPNode*)> deleteNodes = [&](BSPNode* node)
        {
            if (!node) return;
            deleteNodes(node->leftNode);
            deleteNodes(node->rightNode);
            delete node;
        };
    deleteNodes(root);

    // Delete split geometry (MapGeometry handles the originals)
    for (Linedef* l : splitLinedefs)  delete l;
    for (BSPVertex* v : splitVertices) delete v;

    // geo destructor handles the rest automatically
}