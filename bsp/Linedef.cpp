#include "Linedef.h"

Linedef::Linedef(glm::vec2 vert1, glm::vec2 vert2, bool visible)
{
	this->start = new BSPVertex(vert1);
	this->end = new BSPVertex(vert2);
	this->visible = visible;
}

Linedef::Linedef(bool visible)
{
	this->start = new BSPVertex(0.0, 0.0);
	this->end = new BSPVertex(1.0, 0.0);

	this->visible = visible;
}

Linedef::Linedef(BSPVertex* vert1, BSPVertex* vert2, bool visible)
{
	this->start = vert1;
	this->end = vert2;
	this->visible = visible;
}

BSPVertex* Linedef::Start()
{
	return start;
}

BSPVertex* Linedef::End()
{
	return end;
}

bool Linedef::operator==(const Linedef& comp)
{
	if (*this->start == *comp.start)
	{
		if (*this->end == *comp.end)
		{
			return true;
		}
	}
	
	return false;
}
