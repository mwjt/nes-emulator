#pragma once

#include "Cartridge.h"

#include <cstdint>
#include "olcPixelGameEngine.h"

class PPU2C02
{
public:
	PPU2C02();
	~PPU2C02();

	// Get functions
	olc::Sprite& GetScreen();
	olc::Pixel&  GetColour(uint8_t palette, uint8_t pixel);

	// Connect with Main Bus
	uint8_t	cpuRead(uint16_t addr, bool readOnly = false);
	void	cpuWrite(uint16_t addr, uint8_t data);

	// Connect with PPU Bus
	uint8_t	ppuRead(uint16_t addr, bool readOnly = false);
	void	ppuWrite(uint16_t addr, uint8_t data);
	
	// Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

	// Object Attribute Memory
	uint8_t* ppuOAM		= (uint8_t*)OAM;

	// Flag to show when a frame is done and ready to be displayed
	bool frame_complete = false;
	// Flag to show when a non-masked interrupt was requested
	bool nmi = false;

private:
	// Nametables
	uint8_t		nameTable[2][1024];
	// Pallete tables
	uint8_t		paletteTable[32];
	// Pattern tables
	uint8_t		patternTable[2][4096];

	// Palettes
	olc::Pixel	palettes[0x40];
	// "Sprite" - a screen, where pixels are drawn
	olc::Sprite	screen = olc::Sprite(256, 240);

	// Loaded cartridge
	std::shared_ptr<Cartridge> cartridge;
	
	// Drawn pixel information
	int16_t		scanline = 0;
	int16_t		cycle = 0;

	// Below defined are PPU Registers, such as:
	// Control register - controlling PPU opeartion 
	union PPUCTRL
	{
		struct
		{
			uint8_t nametable_x			: 1;
			uint8_t nametable_y			: 1;
			uint8_t increment_mode		: 1;
			uint8_t sprite_pattern		: 1;
			uint8_t background_pattern	: 1;
			uint8_t sprite_size			: 1;
			uint8_t master_slave_mode	: 1;
			uint8_t enable_nmi			: 1;
		};
		uint8_t reg = 0x00;
	};
	// Mask register - controlling graphics rendering
	union PPUMASK
	{
		struct
		{
			uint8_t greyscale					: 1;
			uint8_t render_background_leftmost	: 1;
			uint8_t render_sprites_leftmost		: 1;
			uint8_t render_background			: 1;
			uint8_t render_sprites				: 1;
			uint8_t enhance_red					: 1;
			uint8_t enhance_green				: 1;
			uint8_t enhance_blue				: 1;
		};
		uint8_t reg = 0x00;
	};

	// Status register - reflecting state of PPU 
	union PPUSTATUS
	{
		struct
		{
			uint8_t unused			: 5;
			uint8_t sprite_overflow : 1;
			uint8_t sprite_0_hit	: 1;
			uint8_t vertical_blank	: 1;
		};
		uint8_t reg = 0x00;
	};

	// And, to actually have something to call
	PPUCTRL		control;
	PPUMASK		mask;
	PPUSTATUS	status;

	// loopy register - named after the solution creator
	// Smooth scrolling between nametables
	union LOOPY
	{
		struct
		{
			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};
		uint16_t reg = 0x0000;
	};
	LOOPY	vram_addr;
	LOOPY	tram_addr;

	// Pixel offset - horizontal
	uint8_t fine_x			= 0x00;

	uint8_t address_latch	= 0x00;
	uint8_t ppu_data_buffer = 0x00;

	// Rendering background
	uint8_t nexttile_id		= 0x00;
	uint8_t nexttile_attrib = 0x00;
	uint8_t nexttile_lsb	= 0x00;
	uint8_t nexttile_msb	= 0x00;

	uint16_t pattern_lo		= 0x0000;
	uint16_t pattern_hi		= 0x0000;
	uint16_t attrib_lo		= 0x0000;
	uint16_t attrib_hi		= 0x0000;

	// OAM
	struct ObjectAttribute
	{
		uint8_t y;
		uint8_t id;
		uint8_t attribute;
		uint8_t x;
	};

	ObjectAttribute OAM[64];
	uint8_t			oam_addr = 0x00;

	// Scanlines
	ObjectAttribute spriteScanline[8];
	uint8_t			sprite_count;
	uint8_t			sprite_shifter_pattern_lo[8];
	uint8_t			sprite_shifter_pattern_hi[8];

	// Sprite 0 hit
	bool sprite0HitPossible = false;
	bool sprite0HitRendered = false;
};

