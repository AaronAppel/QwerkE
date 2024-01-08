#include "QC_Helpers.h"

static unsigned int g_UniqueID = 0;
int helpers_GetUniqueID()
{
	return g_UniqueID++;
}

int CharToInt(char num)
{
	return num - 49; // or is it 48?
}