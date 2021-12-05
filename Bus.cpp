#include "Bus.h"


Bus::Bus()
{
	// Connect bus to the CPU
	cpu.Connect(this);
}

Bus::~Bus()
{
}

// Writing into the bus
void Bus::write(uint16_t address, uint8_t data)
{
	// NES has a total of 64kB addressable space
	// All of it is wired, although, much is mirrored

	// Get data from cartridge
	if (cartridge->cpuWrite(address, data)) {}

	// Internal (CPU) RAM - 2kB
	else if (address >= 0x0000 && address <= 0x1FFF)
	{
		// $0000-$07FF - "real" range
		// $0800-$1FFF - mirrored range
		// To address RAM we only need the bits from the "real" range
		cpuRam[address & 0x07FF] = data;
	}
	// PPU Registers - 1B
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		// $2000-$2007 - "real" range
		// $2008-$3FFF - mirrored range
		// To address the register we only need the first 3 bits
		ppu.cpuWrite(address & 0x0007, data);
	}
	// APU registers
	else if (address >= 0x4000 && address <= 0x4013 || address == 0x4015 || address == 0x4017)
	{
		// Masking the address is determined later, in its own method
		apu.write(address, data);
	}
	// DMA stuff
	else if (address == 0x4014)
	{
		// Writing to the $4014 address initiates DMA
		dma_page = data;
		dma_address = 0x00;
		dma_happening = true;
	}
	// Controllers
	else if (address >= 0x4016 && address <= 0x4017)
	{
		// The last bit determines the controller
		controller_state[address & 0x0001] = controller[address & 0x0001];
	}
}

// Reading from the bus
uint8_t Bus::read(uint16_t address, bool readOnly)
{
	// Variable to store the read data
	uint8_t data = 0x00;

	// Get data from cartridge from cartridge
	if (cartridge->cpuRead(address, data)) {}

	// Internal (CPU) RAM - 2kB
	else if (address >= 0x0000 && address <= 0x1FFF)
	{
		data = cpuRam[address & 0x07FF];
	}
	// PPU Registers - 1B
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		data = ppu.cpuRead(address & 0x0007, readOnly);
	}
	// Used mainly for DMC, not implemented
	else if (address == 0x4015)
	{
		data = apu.read(address);
	}
	// Controllers
	else if (address >= 0x4016 && address <= 0x4017)
	{
		// The last bit determines the controller
		// Only the MSB of the controller is read
		data = (controller_state[address & 0x0001] & 0x80) > 0;
		// The controller state is shifted after a read
		controller_state[address & 0x0001] <<= 1;
	}
	// Return the read data
	return data;
}

// Sets the audio constants
void Bus::SetSamplingFrequency(uint32_t sample_rate)
{
	// Time determined by sample
	audioTimeSample = 1.0 / (double)sample_rate;
	// Time determined by clock
	audioTimeClock = 1.0 / 5369318.0;
}

// Loads the cartridge
void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cartridge = cartridge;
	ppu.ConnectCartridge(cartridge);
}

// Resets the bus devices
void Bus::reset()
{
	cartridge->reset();
	cpu.reset();
	ppu.reset();

	dma_page		= 0x00;
	dma_address		= 0x00;
	dma_data		= 0x00;
	dma_odd			= true;
	dma_happening	= false;

	sysClockCount = 0;
}

// A single clock cycle on the bus
bool Bus::clock()
{
	// PPU is the fastest clock in the system! It's clocked every time
	ppu.clock();

	// APU is 6 times slower than PPU.
	// However, in this implementation, it updates frequency sweep every clock cycle
	apu.clock();

	// CPU is clocked 3 times slower than PPU - so clock it every third global clock
	if (sysClockCount % 3 == 0)
	{
		// Check if a DMA transfer is in progress
		if (dma_happening)
		{
			// If so, wait one clock cycle - because they are on an odd count
			if (dma_odd)
			{
				// Check if they really are
				if (sysClockCount % 2 == 1)
				{
					// Yes - so the next cycle will be even
					dma_odd = false;
				}
			}
			else
			{
				// DMA consists of read and write
				// Reading happens off the CPU from the given $xx00-$xxFF range, byte at a time
				if (sysClockCount % 2 == 0)
				{
					// So - read 8 bytes from CPU RAM
					dma_data = read(dma_page << 8 | dma_address);
				}
				// Writing is to the PPU OAM memory, also byte at a time
				else
				{
					// So - write 8 bytes to the OAM memory of the PPU
					ppu.ppuOAM[dma_address] = dma_data;
					// And go to the next address
					dma_address++;

					// If value has overflown, DMA is finished
					if (dma_address == 0x00)
					{
						// Set the flags
						dma_happening	= false;
						// Odd flag to true, so it can be checked next time a DMA happens
						dma_odd			= true;
					}
				}
			}
		}
		// DMA is not in progress - CPU is not blocked, it clocks normally
		else
		{
			cpu.clock();
		}
	}

	// Synchronize the audio
	bool isAudioReady	= false;
	// Add single clock operation time to the audio time
	audioTime	+= audioTimeClock;
	// If the audio time exceeds the note time
	if (audioTime >= audioTimeSample)
	{
		// Subtract the note time from the audio time
		audioTime		-=	audioTimeSample;
		// And put the next note in
		audioOutput		=	apu.MixOutputs();
		// Audio is ready!
		isAudioReady	=	true;
	}

	// Check if non-masked interrupt is supposed to happen
	if (ppu.nmi)
	{
		ppu.nmi = false;
		// If so, execute it
		cpu.nmi();
	}

	// Add a clock count
	sysClockCount++;

	// Return true if audio is ready
	return isAudioReady;
}
