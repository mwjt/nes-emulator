#pragma once
#include <cstdint>
#include <array>

#include "CPU6502.h"
#include "PPU2C02.h"
#include "APU2A03.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

	// CPU, PPU and APU
	CPU6502 cpu;
	PPU2C02 ppu;
	APU2A03 apu;

	// Pointer to "cartridge" - *.nes iNES file format ROM 
	std::shared_ptr<Cartridge> cartridge;
	
	// CPU ram, with a total of 2kB!
	uint8_t cpuRam[2048];

	// Controllers - NES can use two controllers
	uint8_t controller[2];

	// Calculated audio, used by engine
	double	audioOutput = 0.0;

	// Read/write methods
	void	write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool readOnly = false);

	// Set sampling frequency
	void	SetSamplingFrequency(uint32_t sample_rate);

	// Loading cartridge into the Bus
	void	insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	
	void	reset();
	bool	clock();

private:

	// Audio constants
	double	audioTimeSample = 0.0f;
	double	audioTimeClock	= 0.0;
	// ...and variables
	double	audioTime		= 0.0;

	// Controller state is 8 bit - 1 for each button
	uint8_t	controller_state[2];

	// Direct Memory Access - transferring data from CPU memory into the OAM memory
	uint8_t dma_page	= 0x00;
	uint8_t dma_address = 0x00;
	uint8_t dma_data	= 0x00;
	// Flag to determine whether DMA transfer is happening
	bool	dma_happening = false;
	// Flag used to skip odd number of cycles,
	// as CPU has to be on an even number of cycles 
	bool	dma_odd = true;

	// Counting clock cycles to get CPU, PPU and APU to work synchronously
	uint32_t sysClockCount = 0;
};

