#include "PPU2C02.h"

PPU2C02::PPU2C02()
{
	// Loading the palettes representing the NES colours
	palettes[0x00] = olc::Pixel(84, 84, 84);
	palettes[0x01] = olc::Pixel(0, 30, 116);
	palettes[0x02] = olc::Pixel(8, 16, 144);
	palettes[0x03] = olc::Pixel(48, 0, 136);
	palettes[0x04] = olc::Pixel(68, 0, 100);
	palettes[0x05] = olc::Pixel(92, 0, 48);
	palettes[0x06] = olc::Pixel(84, 4, 0);
	palettes[0x07] = olc::Pixel(60, 24, 0);
	palettes[0x08] = olc::Pixel(32, 42, 0);
	palettes[0x09] = olc::Pixel(8, 58, 0);
	palettes[0x0A] = olc::Pixel(0, 64, 0);
	palettes[0x0B] = olc::Pixel(0, 60, 0);
	palettes[0x0C] = olc::Pixel(0, 50, 60);
	palettes[0x0D] = olc::Pixel(0, 0, 0);
	palettes[0x0E] = olc::Pixel(0, 0, 0);
	palettes[0x0F] = olc::Pixel(0, 0, 0);

	palettes[0x10] = olc::Pixel(152, 150, 152);
	palettes[0x11] = olc::Pixel(8, 76, 196);
	palettes[0x12] = olc::Pixel(48, 50, 236);
	palettes[0x13] = olc::Pixel(92, 30, 228);
	palettes[0x14] = olc::Pixel(136, 20, 176);
	palettes[0x15] = olc::Pixel(160, 20, 100);
	palettes[0x16] = olc::Pixel(152, 34, 32);
	palettes[0x17] = olc::Pixel(120, 60, 0);
	palettes[0x18] = olc::Pixel(84, 90, 0);
	palettes[0x19] = olc::Pixel(40, 114, 0);
	palettes[0x1A] = olc::Pixel(8, 124, 0);
	palettes[0x1B] = olc::Pixel(0, 118, 40);
	palettes[0x1C] = olc::Pixel(0, 102, 120);
	palettes[0x1D] = olc::Pixel(0, 0, 0);
	palettes[0x1E] = olc::Pixel(0, 0, 0);
	palettes[0x1F] = olc::Pixel(0, 0, 0);

	palettes[0x20] = olc::Pixel(236, 238, 236);
	palettes[0x21] = olc::Pixel(76, 154, 236);
	palettes[0x22] = olc::Pixel(120, 124, 236);
	palettes[0x23] = olc::Pixel(176, 98, 236);
	palettes[0x24] = olc::Pixel(228, 84, 236);
	palettes[0x25] = olc::Pixel(236, 88, 180);
	palettes[0x26] = olc::Pixel(236, 106, 100);
	palettes[0x27] = olc::Pixel(212, 136, 32);
	palettes[0x28] = olc::Pixel(160, 170, 0);
	palettes[0x29] = olc::Pixel(116, 196, 0);
	palettes[0x2A] = olc::Pixel(76, 208, 32);
	palettes[0x2B] = olc::Pixel(56, 204, 108);
	palettes[0x2C] = olc::Pixel(56, 180, 204);
	palettes[0x2D] = olc::Pixel(60, 60, 60);
	palettes[0x2E] = olc::Pixel(0, 0, 0);
	palettes[0x2F] = olc::Pixel(0, 0, 0);

	palettes[0x30] = olc::Pixel(236, 238, 236);
	palettes[0x31] = olc::Pixel(168, 204, 236);
	palettes[0x32] = olc::Pixel(188, 188, 236);
	palettes[0x33] = olc::Pixel(212, 178, 236);
	palettes[0x34] = olc::Pixel(236, 174, 236);
	palettes[0x35] = olc::Pixel(236, 174, 212);
	palettes[0x36] = olc::Pixel(236, 180, 176);
	palettes[0x37] = olc::Pixel(228, 196, 144);
	palettes[0x38] = olc::Pixel(204, 210, 120);
	palettes[0x39] = olc::Pixel(180, 222, 120);
	palettes[0x3A] = olc::Pixel(168, 226, 144);
	palettes[0x3B] = olc::Pixel(152, 226, 180);
	palettes[0x3C] = olc::Pixel(160, 214, 228);
	palettes[0x3D] = olc::Pixel(160, 162, 160);
	palettes[0x3E] = olc::Pixel(0, 0, 0);
	palettes[0x3F] = olc::Pixel(0, 0, 0);

}

PPU2C02::~PPU2C02()
{
}

// Get screen used for rendering graphics
olc::Sprite& PPU2C02::GetScreen()
{
	return screen;
}

// Get color of a pixel from a given palette
olc::Pixel& PPU2C02::GetColour(uint8_t palette, uint8_t pixel)
{
	return palettes[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

// Get data from PPU registers
uint8_t PPU2C02::cpuRead(uint16_t addr, bool readOnly)
{
	uint8_t data = 0x00;

	if (readOnly)
	{
		switch (addr) {
		case 0x0000:	// Control
			data = control.reg;
			break;
		case 0x0001:	// Mask
			data = mask.reg;
			break;
		case 0x0002:	// Status
			data = status.reg;
			break;
		case 0x0003:	// OAM Address
			break;
		case 0x0004:	// OAM Data
			break;
		case 0x0005:	// Scroll
			break;
		case 0x0006:	// PPU Address
			break;
		case 0x0007:	// PPU Data
			break;
		}
	}
	else
	{
		switch (addr) {
		case 0x0000:	// Control
			break;
		case 0x0001:	// Mask
			break;
		case 0x0002:	// Status
			data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
			status.vertical_blank = 0;
			address_latch = 0;
			break;
		case 0x0003:	// OAM Address
			break;
		case 0x0004:	// OAM Data
			data = ppuOAM[oam_addr];
			break;
		case 0x0005:	// Scroll
			break;
		case 0x0006:	// PPU Address
			break;
		case 0x0007:	// PPU Data
			data = ppu_data_buffer;
			ppu_data_buffer = ppuRead(vram_addr.reg);

			if (vram_addr.reg >= 0x3F00)
				data = ppu_data_buffer;
			vram_addr.reg += (control.increment_mode ? 32 : 1);
			break;
		}
	}

	return data;
}

// Write to the PPU registers
void PPU2C02::cpuWrite(uint16_t addr, uint8_t data)
{
	switch (addr) {
	case 0x0000:	// Control
		control.reg = data;
		tram_addr.nametable_x = control.nametable_x;
		tram_addr.nametable_y = control.nametable_y;
		break;
	case 0x0001:	// Mask
		mask.reg = data;
		break;
	case 0x0002:	// Status
		break;
	case 0x0003:	// OAM Address
		oam_addr = data;
		break;
	case 0x0004:	// OAM Data
		ppuOAM[oam_addr] = data;
		break;
	case 0x0005:	// Scroll
		if (address_latch == 0)
		{
			fine_x = data & 0x07;
			tram_addr.coarse_x = data >> 3;
			address_latch = 1;
		}
		else
		{
			tram_addr.fine_y = data & 0x07;
			tram_addr.coarse_y = data >> 3;
			address_latch = 0;
		}
		break;
	case 0x0006:	// PPU Address
		if (address_latch == 0)
		{
			tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
			address_latch = 1;
		}
		else
		{
			tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
			vram_addr = tram_addr;
			address_latch = 0;
		}
		break;
	case 0x0007:	// PPU Data
		ppuWrite(vram_addr.reg, data);
		vram_addr.reg += (control.increment_mode ? 32 : 1);
		break;
	}
}

// Read from PPU memory
uint8_t PPU2C02::ppuRead(uint16_t address, bool readOnly)
{
	uint8_t	data = 0x00;

	address &= 0x3FFF;

	if (cartridge->ppuRead(address, data)) {}

	else if (address >= 0x0000 && address <= 0x1FFF)
	{
		data = patternTable[(address & 0x1000) >> 12][address & 0x0FFF];
	}
	else if (address >= 0x2000 && address <= 0x3EFF)
	{
		address &= 0x0FFF;

		if (cartridge->mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical
			if (address >= 0x0000 && address <= 0x03FF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0400 && address <= 0x07FF)
				data = nameTable[1][address & 0x03FF];
			if (address >= 0x0800 && address <= 0x0BFF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0C00 && address <= 0x0FFF)
				data = nameTable[1][address & 0x03FF];
		}
		else if (cartridge->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal
			if (address >= 0x0000 && address <= 0x03FF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0400 && address <= 0x07FF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0800 && address <= 0x0BFF)
				data = nameTable[1][address & 0x03FF];
			if (address >= 0x0C00 && address <= 0x0FFF)
				data = nameTable[1][address & 0x03FF];
		}
	}
	else if (address >= 0x3F00 && address <= 0x3FFF)
	{
		address &= 0x001F;
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		data = paletteTable[address] & (mask.greyscale ? 0x30 : 0x3F);
	}


	return data;
}

// Write to PPU memory
void PPU2C02::ppuWrite(uint16_t addr, uint8_t data)
{
	addr &= 0x3FFF;

	if (cartridge->ppuWrite(addr, data)) {}

	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;
		if (cartridge->mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical
			if (addr >= 0x0000 && addr <= 0x03FF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				nameTable[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				nameTable[1][addr & 0x03FF] = data;
		}
		else if (cartridge->mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				nameTable[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				nameTable[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		paletteTable[addr] = data;
	}
}
// Connect cartridge to the PPU
void PPU2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cartridge = cartridge;
}
// Reset PPU registers to the default values (0s)
void PPU2C02::reset()
{
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	nexttile_id = 0x00;
	nexttile_attrib = 0x00;
	nexttile_lsb = 0x00;
	nexttile_msb = 0x00;
	pattern_lo = 0x0000;
	pattern_hi = 0x0000;
	attrib_lo = 0x0000;
	attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}

void PPU2C02::clock()
{
	// Go to the next horizontal tile
	auto IncrementScrollX = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			if (vram_addr.coarse_x == 31)
			{
				vram_addr.coarse_x = 0;
				vram_addr.nametable_x = ~vram_addr.nametable_x;
			}
			else
				vram_addr.coarse_x++;
		}
	};
	// Go to the next vertical tile
	auto IncrementScrollY = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			if (vram_addr.fine_y < 7)
				vram_addr.fine_y++;
			else
			{
				vram_addr.fine_y = 0;

				if (vram_addr.coarse_y == 29)
				{
					vram_addr.coarse_y = 0;
					vram_addr.nametable_y = ~vram_addr.nametable_y;
				}
				else if (vram_addr.coarse_y == 31)
					vram_addr.coarse_y = 0;
				else
					vram_addr.coarse_y++;
			}
		}
	};
	// Transfer the temporar horizontal nametable to the pointer
	auto TransferAddressX = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			vram_addr.nametable_x = tram_addr.nametable_x;
			vram_addr.coarse_x = tram_addr.coarse_x;
		}
	};
	// Transfer the temporar vertical nametable to the pointer
	auto TransferAddressY = [&]()
	{
		if (mask.render_background || mask.render_sprites)
		{
			vram_addr.fine_y = tram_addr.fine_y;
			vram_addr.nametable_y = tram_addr.nametable_y;
			vram_addr.coarse_y = tram_addr.coarse_y;
		}
	};

	auto LoadBackgroundShifters = [&]()
	{
		pattern_lo = (pattern_lo & 0xFF00) | nexttile_lsb;
		pattern_hi = (pattern_hi & 0xFF00) | nexttile_msb;

		attrib_lo = (attrib_lo & 0xFF00) | ((nexttile_attrib & 0b01) ? 0xFF : 0x00);
		attrib_hi = (attrib_hi & 0xFF00) | ((nexttile_attrib & 0b10) ? 0xFF : 0x00);
	};

	auto UpdateShifters = [&]()
	{
		if (mask.render_background)
		{
			pattern_lo <<= 1;
			pattern_hi <<= 1;
			attrib_lo <<= 1;
			attrib_hi <<= 1;
		}

		if (mask.render_sprites && cycle >= 1 && cycle < 258)
		{
			for (uint8_t i = 0; i < sprite_count; i++)
			{
				if (spriteScanline[i].x > 0)
				{
					spriteScanline[i].x--;
				}
				else
				{
					sprite_shifter_pattern_lo[i] <<= 1;
					sprite_shifter_pattern_hi[i] <<= 1;
				}
			}
		}
	};


	if (scanline >= -1 && scanline < 240)
	{
		if (scanline == 0 && cycle == 0)
		{
			cycle = 1;
		}

		if (scanline == -1 && cycle == 1)
		{
			status.vertical_blank = 0;
			status.sprite_0_hit = 0;
			status.sprite_overflow = 0;

			for (int i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}

		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			UpdateShifters();

			switch ((cycle - 1) % 8)
			{
			case 0:
				LoadBackgroundShifters();
				nexttile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
				break;
			case 2:
				nexttile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
					| (vram_addr.nametable_x << 10)
					| ((vram_addr.coarse_y >> 2) << 3)
					| (vram_addr.coarse_x >> 2));
				if (vram_addr.coarse_y & 0x02)
					nexttile_attrib >>= 4;
				if (vram_addr.coarse_x & 0x02)
					nexttile_attrib >>= 2;
				nexttile_attrib &= 0x03;
				break;
			case 4:
				nexttile_lsb = ppuRead((control.background_pattern << 12)
					+ ((uint16_t)nexttile_id << 4)
					+ (vram_addr.fine_y) + 0);
				break;
			case 6:
				nexttile_msb = ppuRead((control.background_pattern << 12)
					+ ((uint16_t)nexttile_id << 4)
					+ (vram_addr.fine_y) + 8);
				break;
			case 7:
				IncrementScrollX();
				break;
			}
		}

		if (cycle == 256)
		{
			IncrementScrollY();
		}

		if (cycle == 257)
		{
			LoadBackgroundShifters();
			TransferAddressX();
		}

		if (cycle == 338 || cycle == 340)
		{
			nexttile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			TransferAddressY();
		}

		if (cycle == 257 && scanline >= 0)
		{
			std::memset(spriteScanline, 0xFF, 8 * sizeof(ObjectAttribute));
			sprite_count = 0;

			for (uint8_t i = 0; i < 8; i++)
			{
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}

			uint8_t nOAMEntry = 0;

			sprite0HitPossible = false;

			while (nOAMEntry < 64 && sprite_count < 9)
			{
				int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);
				if (diff >= 0 && diff < (control.sprite_size ? 16 : 8) && sprite_count < 8)
				{
					if (sprite_count < 8)
					{
						if (nOAMEntry == 0)
							sprite0HitPossible = true;
						memcpy(&spriteScanline[sprite_count], &OAM[nOAMEntry], sizeof(ObjectAttribute));
					}
					sprite_count++;
				}
				nOAMEntry++;
			}
			status.sprite_overflow = (sprite_count >= 8);
		}

		if (cycle == 340)
		{
			for (uint8_t i = 0; i < sprite_count; i++)
			{
				uint8_t pattern_bits_lo, pattern_bits_hi;
				uint16_t pattern_addr_lo, pattern_addr_hi;

				if (!control.sprite_size)
				{
					if (!(spriteScanline[i].attribute & 0x80))
					{
						pattern_addr_lo =
							(control.sprite_pattern << 12)
							| (spriteScanline[i].id << 4)
							| (scanline - spriteScanline[i].y);
					}
					else
					{
						pattern_addr_lo =
							(control.sprite_pattern << 12)
							| (spriteScanline[i].id << 4)
							| (7 - (scanline - spriteScanline[i].y));
					}
				}
				else
				{
					if (!(spriteScanline[i].attribute & 0x80))
					{
						if (scanline - spriteScanline[i].y < 8)
						{
							pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| ((spriteScanline[i].id & 0xFE) << 4)
								| ((scanline - spriteScanline[i].y) & 0x07);
						}
						else
						{
							pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| (((spriteScanline[i].id & 0xFE) + 1) << 4)
								| ((scanline - spriteScanline[i].y) & 0x07);
						}
					}
					else
					{
						if (scanline - spriteScanline[i].y < 8)
						{
							pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| ((spriteScanline[i].id & 0xFE) << 4)
								| (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
						else
						{
							pattern_addr_lo =
								((spriteScanline[i].id & 0x01) << 12)
								| (((spriteScanline[i].id & 0xFE) + 1) << 4)
								| (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
					}
				}
				pattern_addr_hi = pattern_addr_lo + 8;

				pattern_bits_lo = ppuRead(pattern_addr_lo);
				pattern_bits_hi = ppuRead(pattern_addr_hi);

				if (spriteScanline[i].attribute & 0x40)
				{
					auto flipbyte = [](uint8_t b)
					{
						b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
						b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
						b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
						return b;
					};

					pattern_bits_lo = flipbyte(pattern_bits_lo);
					pattern_bits_hi = flipbyte(pattern_bits_hi);
				}

				sprite_shifter_pattern_lo[i] = pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = pattern_bits_hi;

			}
		}

	}

	if (scanline == 240)
	{

	}


	if (scanline >= 241 && scanline < 261)
	{
		if (scanline == 241 && cycle == 1)
		{
			status.vertical_blank = 1;
			if (control.enable_nmi)
				nmi = true;
		}
	}

	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (mask.render_background)
	{
		if (mask.render_background_leftmost || (cycle >= 9))
		{
			uint16_t bit_mux = 0x8000 >> fine_x;

			uint8_t p0_pixel = (pattern_lo & bit_mux) > 0;
			uint8_t p1_pixel = (pattern_hi & bit_mux) > 0;
			
			bg_pixel = (p1_pixel << 1) | p0_pixel;

			uint8_t bg_pal0 = (attrib_lo & bit_mux) > 0;
			uint8_t bg_pal1 = (attrib_hi & bit_mux) > 0;
			bg_palette = (bg_pal1 << 1) | bg_pal0;
		}
	}

	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette = 0x00;
	uint8_t fg_priority = 0x00;

	if (mask.render_sprites)
	{
		if (mask.render_sprites_leftmost || (cycle >= 9))
		{
			sprite0HitRendered = false;

			for (int i = 0; i < sprite_count; i++)
			{
				if (spriteScanline[i].x == 0)
				{
					uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
					uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
					fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

					fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
					fg_priority = (spriteScanline[i].attribute & 0x20) == 0;

					if (fg_pixel != 0)
					{
						if (i == 0)
							sprite0HitRendered = true;
						break;
					}
				}
			}
		}
	}

	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	if (bg_pixel == 0 && fg_pixel == 0)
	{
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0)
	{
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0)
	{
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0)
	{
		if (fg_priority)
		{
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else
		{
			pixel = bg_pixel;
			palette = bg_palette;
		}

		if (sprite0HitPossible && sprite0HitRendered)
			if (mask.render_background && mask.render_sprites)
				if (~(mask.render_background_leftmost | mask.render_sprites_leftmost))
					if (cycle >= 9 && cycle < 258)
						status.sprite_0_hit = 1;
					else
						if (cycle >= 1 && cycle < 258)
							status.sprite_0_hit = 1;
	}

	screen.SetPixel(cycle - 1, scanline, GetColour(palette, pixel));

	cycle++;
	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			scanline = -1;
			frame_complete = true;
		}
	}
}