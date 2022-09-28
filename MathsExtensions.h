#pragma once
class MathsExtensions
{
public:
	static float Lerp(float a, float b, float t) {
		return (1.0f - t) * a + b * t;
	}
};

