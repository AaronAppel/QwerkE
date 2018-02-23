#include "MathStuff.h"
#include <time.h>

// TODO: seed rand()
int RandomNumber()
{
    return rand(); 
}

float RandomNumberInRange(float min, float max)
{
    return ((rand()) / (RAND_MAX / (max - min)) + min);
};