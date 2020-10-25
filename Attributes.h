#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <typeinfo>

#include "Constants.h"


float boundaryCheck(float value, float minValue, float maxValue, 
	float increment);
glm::vec3 boundaryCheck(glm::vec3 value, glm::vec3 minValue, 
	glm::vec3 maxValue, glm::vec3 increment);

template <class T> class Attribute
{
public:
	Attribute(T val, T minVal, T maxVal, T inc, const int scl);
	~Attribute();

	void update(const int factor);
	void update(const int x, const int y, const int z);
	void reset();
	T value;

protected:
	T initValue;
	T minValue;
	T maxValue;
	T increment;
	int scale;
};


template<class T> Attribute<T>::Attribute(
	T val, T minVal, T maxVal, T inc, const int scl)
{
	// TODO: more error checking e.g. 
	//        - confirm min < max 
	//        - confirm increment fits inside min and max etc.
	//        - val winthin min/max bounds for supported types
	//if (val < minVal || val > maxVal)
	//{
	//	val = minVal + ((maxVal - minVal) / 2);
	//	std::cout << "Init val out of bounds, setting to " << val << std::endl;
	//}

	initValue = val;
	value = val;
	minValue = minVal;
	maxValue = maxVal;
	increment = inc;
	scale = scl;
}


template<class T> Attribute<T>::~Attribute()
{

}


template<class T> void Attribute<T>::update(const int factor)
{
	T oldValue = value;
	T newValue = value;

	if (scale == LINEAR_SCALE)
	{
		// factor will be a constant of either 1 or -1
		T incrementChange = increment * T(factor);
		newValue = oldValue + incrementChange;
	}
	else if (scale == EXPONENTIAL_SCALE)
	{
		if (factor == ATTR_INCREASE)
		{
			newValue = oldValue * increment;
		}
		else if (factor == ATTR_DECREASE)
		{
			newValue = oldValue / increment;
		}
	}

	value = boundaryCheck(newValue, minValue, maxValue, increment);
}


template<class T> void Attribute<T>::update(const int x, const int y, const int z)
{
	assert(typeid(T) == typeid(glm::vec3));

	glm::vec3 oldValue = value;
	glm::vec3 newValue = value;

	// xyz are constants of either -1, 1 or 0
	glm::vec3 incrementChange = glm::vec3(
		increment.x * x,
		increment.y * y,
		increment.z * z
	);
	newValue = value + incrementChange;

	value = boundaryCheck(newValue, minValue, maxValue, increment);
}


template<class T> void Attribute<T>::reset()
{
	value = initValue;
}

#endif ATTRIBUTES_H
