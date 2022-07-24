#include "Utils.h"
#include <cmath>
#include <ctime>

int Utils::RoundInt(float value)
{
	return (int)roundf(value);
}

float Utils::Clamp(float value, float left, float right)
{
	value = std::fmax(value, left);
	value = std::fmin(value, right);
	return value;
}
float Utils::RandomInRange(float a, float b)
{
	return a + (rand() / (RAND_MAX / (b - a)));
}