#include "Cartridge.h"

Cartridge::Cartridge(const std::string& fileName)
{
	// iNES Header structure
	struct header
	{
		char	name[4];
		uint8_t prg_rom_size;
		uint8_t chr_rom_size;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char	unused[5];
	};

	header cartHeader;
	valid = false;

	// Create a new file stream
	std::ifstream ifstream;
	// Open the file
	ifstream.open(fileName, std::ifstream::binary);
	// Try to read the file
	if (ifstream.is_open())
	{
		// Read header - for iNES it's 16 bytes
		ifstream.read((char*)&cartHeader, sizeof(cartHeader));

		// If trainer is present (bit 2 set), move 512 bytes forward
		if (cartHeader.mapper1 & 0x04)
			ifstream.seekg(512, std::ios_base::cur);

		// Get Mapper ID
		mapperID	= ((cartHeader.mapper2 >> 4) << 4) | (cartHeader.mapper1 >> 4);
		
		// Get nametable mirroring
		mirror		= (cartHeader.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;
		
		// Get size of PRG ROM
		PRGSize = cartHeader.prg_rom_size;
		PRGMemory.resize(PRGSize * 16384);

		// Read PRG memory from ROM
		ifstream.read((char*)PRGMemory.data(), PRGMemory.size());

		// Get size of CHR ROM
		CHRSize = cartHeader.chr_rom_size;
		if (CHRSize == 0)
			CHRMemory.resize(8192);
		else
			CHRMemory.resize(CHRSize * 8192);

		// Read CHR memory from ROM
		ifstream.read((char*)CHRMemory.data(), CHRMemory.size());

		// Load mapper
		switch (mapperID)
		{
		case 0:
			mapper = std::make_shared<Mapper_000>(PRGSize, CHRSize);
			break;
		}

		// Cartridge read correctly 
		valid = true;
		ifstream.close();
	}

}

Cartridge::~Cartridge()
{
}

// Return whtether cartridge is valid
bool Cartridge::IsValid()
{
	return valid;
}

// Reading from PRG memory
bool Cartridge::cpuRead(uint16_t address, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (mapper->cpuMapRead(address, mapped_addr))
	{
		if (mapped_addr != 0xFFFFFFFF)
			data = PRGMemory[mapped_addr];
		return true;
	}
	return false;
}
// Writing to PRG memory
bool Cartridge::cpuWrite(uint16_t address, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (mapper->cpuMapWrite(address, mapped_addr))
	{
		if (mapped_addr != 0xFFFFFFFF)
			PRGMemory[mapped_addr] = data;
		return true;
	}
	return false;
}


// Reading from CHR memory
bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (mapper->ppuMapRead(addr, mapped_addr))
	{
		data = CHRMemory[mapped_addr];
		return true;
	}
	return false;
}

// Writing to CHR memory
bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (mapper->ppuMapWrite(addr, mapped_addr))
	{
		CHRMemory[mapped_addr] = data;
		return true;
	}
	return false;
}

// Reset cartridge
void Cartridge::reset()
{
	// If mapper is loaded, reset it
	if (mapper != nullptr)
		mapper->reset();
}
