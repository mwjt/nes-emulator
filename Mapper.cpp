#include "Mapper.h"

// This is just a base class for a mapper
Mapper::Mapper(uint8_t prgSize, uint8_t chrSize)
{
	PRGSize = prgSize;
	CHRSize = chrSize;

	reset();
}

Mapper::~Mapper()
{
}

void Mapper::reset()
{
}
