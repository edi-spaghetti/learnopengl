#ifndef SCALAR_ATTRIBUTE_H
#define SCALAR_ATTRIBUTE_H

#include <iostream>

float boundaryCheck(float value, float minValue, float maxValue, float increment);


class ScalarAttribute
{
public:
	ScalarAttribute(float val, float minVal, float maxVal, float inc);
	~ScalarAttribute();

	void update(bool increase);
	float value;

private:
	float initValue;
	float minValue;
	float maxValue;
	float increment;
};

#endif
