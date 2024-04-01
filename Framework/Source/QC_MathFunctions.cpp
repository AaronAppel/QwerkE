#include "QC_MathFunctions.h"

#include <stdlib.h>

#define DEFAULT_RANDOM_SEED 17

void SeedRandomNumber()
{
	srand(DEFAULT_RANDOM_SEED);
}

int RandomInt(int max, int min)
{
	return (rand() % (max - min + 1)) + min;
}

int RandInRange(int min, int max)
{
	return rand() / (RAND_MAX / (max - min)) + min;
}