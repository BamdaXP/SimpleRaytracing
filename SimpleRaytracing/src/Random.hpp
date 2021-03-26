#pragma once

#include <random>
class Random
{
public:
	static float FRandom();
	static float FRandom(float a, float b);
	static int IRandom();
	static int IRandom(int a,int b);
	static void SetSeed(uint32_t seed);
private:
	static std::default_random_engine s_Engine;
	static std::uniform_int_distribution<int> s_IntDist;
	static std::uniform_real_distribution<float> s_FloatDist;
};