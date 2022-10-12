#include "MathsExtensions.h"
#include <algorithm>
float MathsExtensions::Lerp(float a, float b, float t) {
	return (1.0f - t) * a + b * t;
}
float MathsExtensions::InverseLerp(float a, float b, float value) {
	return (value - a) / (b - a);
}
float MathsExtensions::Clamp(float minValue, float maxValue, float value) {
	return std::clamp(value, minValue, maxValue);
}