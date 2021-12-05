#include "APU2A03.h"

// Used by Length Counters
uint8_t APU2A03::lengths[] = {	10, 254, 20,  2, 40,  4, 80,  6, 160,   8, 60, 10, 14, 12, 26, 14,
								12,  16, 24, 18, 48, 20, 96, 22, 192,  24, 72, 26, 16, 28, 32, 30 };

// Creation method of APU
APU2A03::APU2A03()
{
	// Default noise sequence
	noise_seq.sequence = 0xDBDB;
}

APU2A03::~APU2A03()
{
}

void APU2A03::write(uint16_t address, uint8_t data)
{
	switch (address)
	{
	case 0x4000:
		switch ((data & 0xC0) >> 6)
		{
		case 0x00:
			pulse1_seq.new_sequence = 0b01000000;
			pulse1_osc.dutycycle	= 0.125;
			break;
		case 0x01:
			pulse1_seq.new_sequence = 0b01100000;
			pulse1_osc.dutycycle	= 0.250;
			break;
		case 0x02:
			pulse1_seq.new_sequence = 0b01111000;
			pulse1_osc.dutycycle	= 0.500;
			break;
		case 0x03:
			pulse1_seq.new_sequence = 0b10011111;
			pulse1_osc.dutycycle	= 0.750;
			break;
		}
		pulse1_seq.sequence = pulse1_seq.new_sequence;
		pulse1_halt			= (data & 0x20);
		pulse1_env.volume	= (data & 0x0F);
		pulse1_env.disable	= (data & 0x10);
		break;
	case 0x4001:
		pulse1_sweep.enabled	= data & 0x80;
		pulse1_sweep.period		= (data & 0x70) >> 4;
		pulse1_sweep.down		= data & 0x08;
		pulse1_sweep.shift		= data & 0x07;
		pulse1_sweep.reload		= true;
		break;
	case 0x4002:
		pulse1_seq.reload = (pulse1_seq.reload & 0xFF00) | data;
		break;
	case 0x4003:
		pulse1_seq.reload	= (uint16_t)((data & 0x07)) << 8 | (pulse1_seq.reload & 0x00FF);
		pulse1_seq.timer	= pulse1_seq.reload;
		pulse1_seq.sequence = pulse1_seq.new_sequence;
		pulse1_lc.counter	= lengths[(data & 0xF8) >> 3];
		pulse1_env.start	= true;
		break;
	case 0x4004:
		switch ((data & 0xC0) >> 6)
		{
		case 0x00:
			pulse2_seq.new_sequence = 0b01000000;
			pulse2_osc.dutycycle = 0.125;
			break;
		case 0x01:
			pulse2_seq.new_sequence = 0b01100000;
			pulse2_osc.dutycycle = 0.250;
			break;
		case 0x02:
			pulse2_seq.new_sequence = 0b01111000;
			pulse2_osc.dutycycle = 0.500;
			break;
		case 0x03:
			pulse2_seq.new_sequence = 0b10011111;
			pulse2_osc.dutycycle = 0.750;
			break;
		}
		pulse2_seq.sequence = pulse2_seq.new_sequence;
		pulse2_halt = (data & 0x20);
		pulse2_env.volume = (data & 0x0F);
		pulse2_env.disable = (data & 0x10);
		break;
	case 0x4005:
		pulse2_sweep.enabled = data & 0x80;
		pulse2_sweep.period = (data & 0x70) >> 4;
		pulse2_sweep.down = data & 0x08;
		pulse2_sweep.shift = data & 0x07;
		pulse2_sweep.reload = true;
		break;
	case 0x4006:
		pulse2_seq.reload = (pulse2_seq.reload & 0xFF00) | data;
		break;
	case 0x4007:
		pulse2_seq.reload = (uint16_t)((data & 0x07)) << 8 | (pulse2_seq.reload & 0x00FF);
		pulse2_seq.timer = pulse2_seq.reload;
		pulse2_seq.sequence = pulse2_seq.new_sequence;
		pulse2_lc.counter = lengths[(data & 0xF8) >> 3];
		pulse2_env.start = true;
		break;
	case 0x4008:
		break;
	case 0x400C:
		noise_env.volume = (data & 0x0F);
		noise_env.disable = (data & 0x10);
		noise_halt = (data & 0x20);
		break;
	case 0x400E:
		switch (data & 0x0F)
		{
		case 0x00:
			noise_seq.reload = 0;
			break;
		case 0x01:
			noise_seq.reload = 4;
			break;
		case 0x02:
			noise_seq.reload = 8;
			break;
		case 0x03:
			noise_seq.reload = 16;
			break;
		case 0x04:
			noise_seq.reload = 32;
			break;
		case 0x05:
			noise_seq.reload = 64;
			break;
		case 0x06:
			noise_seq.reload = 96;
			break;
		case 0x07:
			noise_seq.reload = 128;
			break;
		case 0x08:
			noise_seq.reload = 160;
			break;
		case 0x09:
			noise_seq.reload = 202;
			break;
		case 0x0A:
			noise_seq.reload = 254;
			break;
		case 0x0B:
			noise_seq.reload = 380;
			break;
		case 0x0C:
			noise_seq.reload = 508;
			break;
		case 0x0D:
			noise_seq.reload = 1016;
			break;
		case 0x0E:
			noise_seq.reload = 2034;
			break;
		case 0x0F:
			noise_seq.reload = 4068;
			break;
		}
		break;

	case 0x4015:
		pulse1_enable	= data & 0x01;
		pulse2_enable	= data & 0x02;
		noise_enable	= data & 0x04;
		break;
	case 0x400F:
		pulse1_env.start = true;
		pulse2_env.start = true;
		noise_env.start = true;
		noise_lc.counter = lengths[(data & 0xF8) >> 3];
		break;
	}
}

uint8_t APU2A03::read(uint16_t addr)
{
	return 0x00;
}

void APU2A03::clock()
{
	bool	quarter	= false;
	bool	half	= false;

	globalTime += (0.3333333333 / 1789773);

	if (clock_count % 6 == 0)
	{
		frame_clock_count++;

		if (frame_clock_count == 3729)
		{
			quarter	= true;
		}
		if (frame_clock_count == 7457)
		{
			quarter	= true;
			half		= true;
		}
		if (frame_clock_count == 11186)
		{
			quarter	= true;
		}
		if (frame_clock_count == 14916)
		{
			quarter	= true;
			half		= true;
			frame_clock_count	= 0;
		}

		// Updates volume envelopes
		if (quarter)
		{
			pulse1_env.clock(pulse1_halt);
			pulse2_env.clock(pulse2_halt);
			noise_env.clock(noise_halt);
		}
		// Updates note length and frequency sweeper
		if (half)
		{
			pulse1_lc.clock(pulse1_enable, pulse1_halt);
			pulse2_lc.clock(pulse2_enable, pulse2_halt);
			noise_lc.clock(noise_enable, noise_halt);

			pulse1_sweep.clock(pulse1_seq.reload, 0);
			pulse2_sweep.clock(pulse2_seq.reload, 1);
		}

		pulse1_seq.clock(pulse1_enable, [](uint32_t& s)
			{
				s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
			});

		pulse1_osc.frequency = 1789773.0 / (16.0 * (double)(pulse1_seq.reload + 1));
		pulse1_osc.amplitude = (double)(pulse1_env.output - 1) / 16.0;
		pulse1_sample = pulse1_osc.sample(globalTime);

		if (pulse1_lc.counter > 0 && pulse1_seq.timer >= 8 && !pulse1_sweep.mute && pulse1_env.output > 2)
			pulse1_output += (pulse1_sample - pulse1_output) * 0.5;
		else
			pulse1_output = 0;

		pulse2_seq.clock(pulse2_enable, [](uint32_t& s)
			{
				s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
			});

		pulse2_osc.frequency = 1789773.0 / (16.0 * (double)(pulse2_seq.reload + 1));
		pulse2_osc.amplitude = (double)(pulse2_env.output - 1) / 16.0;
		pulse2_sample = pulse2_osc.sample(globalTime);

		if (pulse2_lc.counter > 0 && pulse2_seq.timer >= 8 && !pulse2_sweep.mute && pulse2_env.output > 2)
			pulse2_output += (pulse2_sample - pulse2_output) * 0.5;
		else
			pulse2_output = 0;

		noise_seq.clock(noise_enable, [](uint32_t& s)
			{
				s = (((s & 0x0001) ^ ((s & 0x0002) >> 1)) << 14) | ((s & 0x7FFF) >> 1);
			});

		if (noise_lc.counter > 0 && noise_seq.timer >= 8)
			noise_output = (double)noise_seq.output * ((double)(noise_env.output - 1) / 16.0);

		if (!pulse1_enable) pulse1_output = 0;
		if (!pulse2_enable) pulse2_output = 0;
		if (!noise_enable)	noise_output = 0;
	}

	pulse1_sweep.track(pulse1_seq.reload);
	pulse2_sweep.track(pulse2_seq.reload);

	clock_count++;
}

void APU2A03::reset()
{

}

double APU2A03::MixOutputs()
{
	return
		((1.0 * pulse1_output) - 0.8) * 0.1 +
		((1.0 * pulse2_output) - 0.8) * 0.1 +
		((2.0 * noise_output - 0.5) * 0.1);
}
