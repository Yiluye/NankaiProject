#pragma once
#ifndef _RANDON_H
#define _RANDON_H
#include<cstdlib>
#include<ctime>

void Setrandomseed();

double Random();

double Random(double upper);

double Random(double lower, double upper);

int Random(int upper);

int Random(int upper, int lower);

#endif // !_RANDON_H


