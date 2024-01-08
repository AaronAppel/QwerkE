#pragma once

// #TODO Try to be specific with helpers. Delete this file but move functions to specifically named helper files

// seed rand()
//int RandomNumber() { return rand(); };

//float RandomNumberInRange(float min, float max) { return ((rand()) / (RAND_MAX / (max - min)) + min); };

int helpers_GetUniqueID();

// #define MyColor vec4
class MyColor
{
public:
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 0;

    MyColor() = default;
    MyColor(unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
    {
        r = nr;
        g = ng;
        b = nb;
        a = na;
    }
};

int CharToInt(char num);
