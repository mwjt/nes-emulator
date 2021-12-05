#include "Mapper_000.h"

Mapper_000::Mapper_000(uint8_t prgSize, uint8_t chrSize) : Mapper(prgSize, chrSize)
{
}

Mapper_000::~Mapper_000()
{
}

// Transform CPU address to PRG ROM offset
bool Mapper_000::cpuMapRead(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x8000 && address <= 0xFFFF)
	{
		mapped_address = address & (PRGSize > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

// Transform CPU address to PRG ROM offset
bool Mapper_000::cpuMapWrite(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x8000 && address <= 0xFFFF)
	{
		mapped_address = address & (PRGSize > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

// Transform PPU address to CHR ROM offset
bool Mapper_000::ppuMapRead(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		mapped_address = address;
		return true;
	}

	return false;
}

// Transform PPU address to CHR ROM offset
bool Mapper_000::ppuMapWrite(uint16_t address, uint32_t& mapped_address)
{
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		if (CHRSize == 0)
		{
			mapped_address = address;
			return true;
		}
	}
	return false;
}

void Mapper_000::reset()
{
}