#pragma once

void SeedRandomNumber();

int RandomInt(int max, int min);

int RandInRange(int min, int max);
// float RandInRange(float min, float max); // #TODO Support random decimal values

// #TODO RandomWeighted(array) { ; // Get random number and find where it fits along a sum of all array elements, then return array index where rand lands }

// #TODO seed rand()
// #TODO float RandomNumberInRange(float min, float max) { return ((rand()) / (RAND_MAX / (max - min)) + min); };