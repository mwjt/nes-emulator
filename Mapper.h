#pragma once
#include <cstdint>

// This is just a base class for a mapper
class Mapper
{
public:
	Mapper(uint8_t prgSize, uint8_t chrSize);
	~Mapper();

	virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr)	= 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr)	= 0;
	virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr)	= 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)	= 0;

	virtual void reset() = 0;


protected:
	uint8_t PRGSize = 0;
	uint8_t CHRSize = 0;
};

