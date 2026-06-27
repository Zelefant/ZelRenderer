#include "MapGeometry.h"


MapGeometry::MapGeometry(
    std::vector<BSPVertex*> vertices,
    std::vector<Linedef*> linedefs,
    std::vector<Sector*> sectors)
    : vertices(std::move(vertices)),
    linedefs(std::move(linedefs)),
    sectors(std::move(sectors))
{
}

MapGeometry::~MapGeometry()
{
    for (Sector* sector : sectors)
    {
        delete(sector);
    }
    for (Linedef* line : linedefs)
    {
        delete(line);
    }
    for (BSPVertex* vertex : vertices)
    {
        delete(vertex);
    }
}