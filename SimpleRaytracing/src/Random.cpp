#include "Random.hpp"
#include <ctime>

std::default_random_engine Random::s_Engine;
std::uniform_int_distribution<int> Random::s_IntDist;
std::uniform_real_distribution<float> Random::s_FloatDist;

float Random::FRandom()
{
	return s_FloatDist(s_Engine);
}

float Random::FRandom(float a, float b)
{

	return s_FloatDist(s_Engine)*(b-a)+a;
}

int Random::IRandom()
{
	return s_IntDist(s_Engine);
}

int Random::IRandom(int a, int b)
{
	auto dist = std::uniform_int_distribution<int>(a, b);
	return dist(s_Engine);
}

void Random::SetSeed(uint32_t seed)
{
	s_Engine.seed(seed);
}
