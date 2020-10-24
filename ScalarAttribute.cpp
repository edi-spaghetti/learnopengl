#include "ScalarAttribute.h"

ScalarAttribute::ScalarAttribute(float val, float minVal, float maxVal, float inc)
{
	// TODO: more error checking e.g. 
	//        - confirm min < max 
	//        - confirm increment fits inside min and max etc.
	if (val < minVal || val > maxVal)
	{
		val = minVal + ((maxVal - minVal) / 2);
		std::cout << "Init val out of bounds, setting to " << val << std::endl;
	}

	initValue = val;
	value = val;
	minValue = minVal;
	maxValue = maxVal;
	increment = inc;
}

ScalarAttribute::~ScalarAttribute()
{

}


void ScalarAttribute::update(bool increase)
{
	float oldValue = value;
	float newValue;

	if (increase)
	{
		newValue = oldValue + increment;
	}
	else
	{
		newValue = oldValue - increment;
	}

	value = boundaryCheck(newValue, minValue, maxValue, increment);
	std::cout
		<< "Value="
		<< oldValue << " -> " << value
		<< " (" << newValue << ")"
		<< std::endl;
}


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