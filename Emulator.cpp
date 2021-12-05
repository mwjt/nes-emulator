#include <iostream>
#include <sstream>

#include "Bus.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define	OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"



class Emulator : public olc::PixelGameEngine
{
public:
	Emulator(std::string &filename)
	{ 
		this->filename	= filename;
		sAppName		= "Emulator systemu NES"; 
	}

private:
	// The NES
	Bus		nes;

	// To start/stop emulation
	bool	running = false;

	// Pointer to cartridge
	std::shared_ptr<Cartridge> cartridge;
	std::string filename;

	// Used to tie together timing of NES and audio
	static	Emulator* instance;

	// OLC engine - create method, start up emulator
	bool OnUserCreate() override
	{
		// Load the cartridge
		cartridge = std::make_shared<Cartridge>(filename);

		// If can't load cartridge, finish here
		if (!cartridge->IsValid())
			return false;

		// Insert cartridge into NES
		nes.insertCartridge(cartridge);

		// Reset NES - set starting values
		nes.reset();

		instance = this;

		// Set audio consts
		nes.SetSamplingFrequency(44100);
		olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
		olc::SOUND::SetUserSynthFunction(GetSound);

		return true;
	}
	
	
	// Synchonizing NES clock with audio 
	static float GetSound(int channel, float time, float timeStep)
	{
		// Do cycles until audio sample is ready
		while (!instance->nes.clock());

		// Return the prepared audio sample
		return static_cast<float>(instance->nes.audioOutput);
	}

	// OLC engine - destroy method, kill audio on exit
	bool OnUserDestroy() override
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

	// Main method - update emu
	bool EmulatorUpdate(float time)
	{
		// Clear screen
		Clear(olc::DARK_BLUE);

		// Get controller input
		nes.controller[0] = 0x00;
		nes.controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;		// A
		nes.controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;		// B
		nes.controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;		// Select
		nes.controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;		// Start
		nes.controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;		// UP
		nes.controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;	// DOWN
		nes.controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;	// LEFT
		nes.controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;	// RIGHT

		// Start/stop
		if (GetKey(olc::Key::SPACE).bPressed) running = !running;
		// Reset
		if (GetKey(olc::Key::R).bPressed) nes.reset();

		// Draw rendered output
		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
		return true;
	}

	// OLC engine - update method
	bool OnUserUpdate(float time)
	{
		// Get input
		EmulatorUpdate(time);
		return true;
	}
};

Emulator* Emulator::instance = nullptr;
int main(int argc, char** argv)
{
	std::string filename = argv[1];
	Emulator emulator(filename);
	// Construct window (width, height, pixel_width, pixel_height, fullscreen, vsync)
	emulator.Construct(512, 480, 2, 2, 0, 1);
	// Start emulation
	emulator.Start();
	return 0;
}