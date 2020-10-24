#ifndef GEOMETRY_H
#define GEOMETRY_H

const int MAX_DATA_SIZE = 270;
const int MAX_ATTRIBUTES = 3;

struct Geometry {
	int numAttributes;
	int attributes[MAX_ATTRIBUTES];

	int stride;  // number of bytes per vertex = sum(attributes) * sizeof(float)

	unsigned int dataLength;  // number of active vertices in the data array
	unsigned int dataSize;  // number active elements multiplied by sizeof(float)
	float data[MAX_DATA_SIZE];

	bool useIndices;
	unsigned int iLength;
	unsigned int iSize;  // number of active elements in indices mutiplied by sizeof(int)
	unsigned int indices[MAX_DATA_SIZE];
};

#endif
