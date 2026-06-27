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