#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

#include "Mapper_000.h"

class Cartridge
{
public:
	Cartridge(const std::string& fileName);
	~Cartridge();

	// Check the validness flag
	bool IsValid();

	// Which way to mirror nametables - or, to scroll
	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI
	};

	MIRROR	mirror = HORIZONTAL;

private:

	// Flag to set when ROM was succesfuly read
	bool valid = false;

	// Memory containing program
	uint8_t PRGSize		= 0;
	std::vector<uint8_t> PRGMemory;
	// Memory containing graphics
	uint8_t CHRSize		= 0;
	std::vector<uint8_t> CHRMemory;

	// Loaded mapper
	uint8_t mapperID	= 0;
	std::shared_ptr<Mapper> mapper;

public:
	// Connect with CPU bus
	bool	cpuRead(uint16_t addr, uint8_t& data);
	bool	cpuWrite(uint16_t addr, uint8_t data);

	// Connect with PPU bus
	bool	ppuRead(uint16_t addr, uint8_t& data);
	bool	ppuWrite(uint16_t addr, uint8_t data);

	void reset();
};

