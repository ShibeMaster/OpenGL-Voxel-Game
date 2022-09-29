#include "MathsExtensions.h"
float MathsExtensions::Lerp(float a, float b, float t) {
	return (1.0f - t) * a + b * t;
}