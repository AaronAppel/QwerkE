#include "QC_Math_Includes.h"

#include "QC_Vector.h"

bool PointInBox(vec2 point, vec2 boxPosition, float width, float length)
{
	return  point.x > boxPosition.x - width * 0.5f &&
			point.x < boxPosition.x + width * 0.5f &&
			point.y > boxPosition.y - length * 0.5f &&
			point.y < boxPosition.y + length * 0.5f;
}