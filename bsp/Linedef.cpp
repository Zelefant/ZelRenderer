#include "Linedef.h"



Linedef::Linedef(int id, BSPVertex* vert1, BSPVertex* vert2, bool visible)
{
	this->id = id;
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
