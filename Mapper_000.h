#pragma once
#include "Mapper.h"
class Mapper_000 : public Mapper
{
public:
	Mapper_000(uint8_t prgSize, uint8_t chrSize);
	~Mapper_000();

	// Transform CPU address to PRG ROM offset
	bool cpuMapRead(uint16_t address, uint32_t& mapped_address) override;
	bool cpuMapWrite(uint16_t address, uint32_t& mapped_address) override;
	// Transform PPU address to CHR ROM offset
	bool ppuMapRead(uint16_t address, uint32_t& mapped_address) override;
	bool ppuMapWrite(uint16_t address, uint32_t& mapped_address) override;
	
	void reset() override;
};

