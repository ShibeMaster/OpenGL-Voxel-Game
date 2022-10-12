#pragma once
class MathsExtensions
{
public:
	static float Lerp(float a, float b, float t);
	static float InverseLerp(float a, float b, float t);
	static float Clamp(float min, float max, float value);
};

