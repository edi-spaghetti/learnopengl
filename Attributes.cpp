#include "Attributes.h"


float boundaryCheck(float value, float minValue, float maxValue, float increment)
{
	// avoid floating point precision issues by checking for 
	// one increment above limits
	if (value < (minValue + increment))
	{
		return minValue;
	}
	else if ((maxValue - increment) < value)
	{
		return maxValue;
	}

	// otherwise value is within bounds, so return it unchanged
	return value;
}


glm::vec3 boundaryCheck(glm::vec3 value, glm::vec3 minValue,
	glm::vec3 maxValue, glm::vec3 increment)
{
	value.x = boundaryCheck(value.x, minValue.x, maxValue.x, increment.x);
	value.y = boundaryCheck(value.y, minValue.y, maxValue.y, increment.y);
	value.z = boundaryCheck(value.z, minValue.z, maxValue.z, increment.z);

	return value;
}

