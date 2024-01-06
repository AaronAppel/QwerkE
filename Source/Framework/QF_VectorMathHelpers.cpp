#include "QF_VectorMathHelpers.h"

#include "QC_MathConstants.h"

static float CalculateDistance2D(Vector2 a_TipPosition, Vector2 a_TailPosition)
{
	Vector2 difference = a_TipPosition - a_TailPosition;
	return sqrt(difference.x * difference.x + difference.y * difference.y);
}

static float CalculateDistance3D(Vector3 a_TipPosition, Vector3 a_TailPosition)
{
	Vector3 difference = a_TipPosition - a_TailPosition;
	return sqrt(difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
}

static float CalculateDistance3DXZ(Vector3 a_TipPosition, Vector3 a_TailPosition)
{
	Vector3 difference = a_TipPosition - a_TailPosition;
	return sqrt(difference.x * difference.x + difference.z * difference.z);
}

static float CalculateDistance3DXY(Vector3 a_TipPosition, Vector3 a_TailPosition)
{
	Vector3 difference = a_TipPosition - a_TailPosition;
	return sqrt(difference.x * difference.x + difference.y * difference.y);
}

static float CalculateAngleXZ(Vector3 a_TipPosition, Vector3 a_TailPosition)
{
	Vector3 difference = a_TipPosition - a_TailPosition;
	float angle = atan2(difference.z, difference.x) * (float)RAD_TO_DEG;

	if (angle < 0)
		angle = 360 + angle; // #TODO Review case, and also handle above 360 degrees

	return angle;
}