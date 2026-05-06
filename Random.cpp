#include"Random.h"
void Setrandomseed()
{
	srand((unsigned)time(nullptr));
}

double Random()
{
	return (double)rand() / (double)RAND_MAX;
}

double Random(double upper)
{
	return Random() * upper;
}

double Random(double lower, double upper)
{
	return lower + Random() * (upper - lower);
}

int Random(int upper)
{
	return rand() % upper;
}

int Random(int lower, int upper)
{
	return lower + rand() % (upper - lower);
}