#include "Bus.h"
#include "CPU6502.h"

CPU6502::CPU6502()
{
	// Instruction set Op Code matrix
	// With the same MSD|LSD as 6502
	using a = CPU6502;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU6502::~CPU6502()
{
}

uint8_t CPU6502::read(uint16_t addr)
{
	return bus->read(addr, false);
}

void CPU6502::write(uint16_t addr, uint8_t data)
{
	bus->write(addr, data);
}

// Performing a single clock cycle
void CPU6502::clock()
{
	if (cycles == 0)
	{
		// Get current Opcode
		opcode = read(pc);

		SetFlag(U, true);

		// Prepare Program Counter to read the next byte
		pc++;

		// Get cycle count for current Opcode
		cycles = lookup[opcode].cycles;

		// Check if Opcode requires additional cycles
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		cycles += (additional_cycle1 & additional_cycle2);

		SetFlag(U, true);
	}

	cycles--;
}

// Configure the CPU to a known state
void CPU6502::reset()
{
	// Get Program Counter address and set it
	addr_abs	= 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc			= (hi << 8) | lo;

	// Reset registers
	a		= 0;
	x		= 0;
	y		= 0;
	stkp	= 0xFD;
	status	= 0x00 | U;

	// Clear internal variables
	addr_rel	= 0x0000;
	addr_abs	= 0x0000;
	fetched		= 0x00;

	// Execution time
	cycles	= 8;
}

// Interrupt request
void CPU6502::irq()
{
	// IF Interrupts are allowed
	if (GetFlag(I) == 0)
	{
		// Push Program Counter to stack
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		// Push Status register to stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		// Get Program Counter address and set it
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// Execution time
		cycles = 7;
	}
}

// Non-masked Interrupt Request
void CPU6502::nmi()
{
	// Push Program Counter to stack
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	// Push Status register to stack
	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + stkp, status);
	stkp--;

	// Get Program Counter address and set it
	addr_abs	= 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc			= (hi << 8) | lo;

	// Execution time
	cycles = 8;
}

// Return value of a specific bit from status register
uint8_t CPU6502::GetFlag(FLAGS flag)
{
	return ((status & flag) > 0) ? 1 : 0;
}

// Set value of a specific bit from status register
void CPU6502::SetFlag(FLAGS flag, bool newStatus)
{
	if (newStatus)
		status |= flag;
	else
		status &= ~flag;
}

// Addressing Modes

// Immediate addressing:
// Data is supplied as part of instruction, so next byte
uint8_t CPU6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

// Absolute addressing:
// Full address, consisting of provided Low Byte and High Byte
uint8_t CPU6502::ABS()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	return 0;
}

// Zero Page addressing:
// Byte of data the instruction needs to read is on Page 0
// Hence the High Byte is always going to be 0, so reading Low Byte is enough
uint8_t CPU6502::ZP0()
{
	addr_abs = read(pc);
	pc++;

	addr_abs &= 0x00FF;
	return 0;
}

// Indexed Zero Page addressing:
// Zero Page addressing with X register offset
uint8_t CPU6502::ZPX()
{
	addr_abs = (read(pc) + x);
	pc++;

	addr_abs &= 0x00FF;
	return 0;
}

// Indexed Zero Page addressing:
// Zero Page addressing with Y register offset
uint8_t CPU6502::ZPY()
{
	addr_abs = (read(pc) + y);
	pc++;

	addr_abs &= 0x00FF;
	return 0;
}

// Indexed Absolute addressing:
// Absolute addressing with X register offset
uint8_t CPU6502::ABX()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	addr_abs += x;

	// Check if the address changed to a different page,
	// if so notify system that additional clock cycle may be necessary
	if ((addr_abs & 0xFF00) != (hi << 8))	
		return 1;
	else
		return 0;
}

// Indexed Absolute addressing:
// Absolute addressing with Y register offset
uint8_t CPU6502::ABY()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	addr_abs += y;

	// Check if the address changed to a different page,
	// if so notify system that additional clock cycle may be necessary
	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Implied addressing:
// No data in instruction, but it can be operating upon Accumulator
uint8_t CPU6502::IMP()
{
	fetched = a;
	return 0;
}

// Relative addressing:
// Only applies to branching instructions, hence the 127 constraints
uint8_t CPU6502::REL()
{
	addr_rel = read(pc);
	pc++;

	// Check if relative address is negative (last bit set)
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}

// Indirect Indexed addressing:
// Supplied address is offset by X register, then read
uint8_t CPU6502::IZX()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	return 0;
}

// Indirect Indexed addressing
// Supplied address is read, then offset by Y register
uint8_t CPU6502::IZY()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;
	
	// Check if the address changed to a different page,
	// if so notify system that additional clock cycle may be necessary
	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Absolute Indirect:
// Supplied address is a pointer, so it is necessary to extract data with read()
uint8_t CPU6502::IND()
{
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	// Page boundary hardware bug
	if (ptr_lo == 0x00FF)
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}
	return 0;
}

// Instructions

// Fetch:
// Get value used by instruction
// It is defined because not every instruction fetches data (e.g. CLC)
uint8_t CPU6502::fetch()
{
	// There is nothing to fetch in Implied addressing
	if (!(lookup[opcode].addrmode == &CPU6502::IMP))
		fetched = read(addr_abs);
	return fetched;
}

// Add with Carry:
// Adds fetched value and Carry bit to Accumulator
// Carry is set if val > 0x00FF
// Zero is set if val == 0x0000
// Negative set if val >= 0x0080
// Overflow is set if value overflows ((A XOR Result) & NOT(A XOR Fetched))
uint8_t CPU6502::ADC()
{
	fetch();
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

	SetFlag(C, temp > 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x80);
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

	a = temp & 0x00FF;
	return 1;
}

// Bitwise AND with Accumulator:
// Result is stored in Accumulator
// Zero is set if A == 0x00
// Negative is set if A MSB is set
uint8_t CPU6502::AND()
{
	fetch();
	a = a & fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Arithmetic Shift Left:
// Shift all bits one position left. 0 is shifted into LSB and MSB is shifted into Carry 
// Carry is set if val > 0x00FF
// Zero is set if val == 0x00
// Negative is set if val MSB is set
uint8_t CPU6502::ASL()
{
	fetch();
	temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

// Branch on Carry Clear:
// Branch if Carry bit is clear
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BCC()
{
	if (GetFlag(C) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))		
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Branch on Carry Set:
// Branch if Carry bit is set
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BCS()
{
	if (GetFlag(C) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Branch on Equal:
// Branch if Zero bit is set (is equal)
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Test Bits:
// Zero is set to (val AND A)
// Negative is set to bit 7 of value
// Overflow is set to bit 6 of value
uint8_t CPU6502::BIT()
{
	fetch();
	temp = a & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}

// Branch on Minus:
// Branch if Negative bit is set
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BMI()
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Branch on Not Equal:
// Branch if Zero bit is clear (is not equal)
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BNE()
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Branch on Plus:
// Branch if Negative bit is clear
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BPL()
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Break:
// Causes an Non-maskable Interrupt and increments Program Counter
uint8_t CPU6502::BRK()
{
	pc++;

	SetFlag(I, 1);
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 1);
	write(0x0100 + stkp, status);
	stkp--;
	SetFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

// Branch on Overflow Clear:
// Branch if Overflow bit is clear
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BVC()
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Branch on Overflow Set:
// Branch if Overflow bit is set
// Adds 1 cycle if branch occurs to the same page
// Adds 2 cycles if branch occurs to a different page
uint8_t CPU6502::BVS()
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Clear Carry:
// Clear the Carry bit
uint8_t CPU6502::CLC()
{
	SetFlag(C, false);
	return 0;
}

// Clear Decimal:
// Clear the Decimal bit
uint8_t CPU6502::CLD()
{
	SetFlag(D, false);
	return 0;
}

// Clear Interrupt:
// Clear the Interrupt bit
uint8_t CPU6502::CLI()
{
	SetFlag(I, false);
	return 0;
}

// Clear Overflow:
// Clear the Overflow bit
uint8_t CPU6502::CLV()
{
	SetFlag(V, false);
	return 0;
}

// Compare Accumulator:
// Compares supplied value with Accumulator
// Carry is set to (A >= val)
// Zero is set to (A == val)
// Negative is set to val MSB
uint8_t CPU6502::CMP()
{
	fetch();
	temp = (uint16_t)a - (uint16_t)fetched;
	SetFlag(C, a >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}

// Compare X register:
// Compares supplied value with X register
// Carry is set to (X >= val)
// Zero is set to (X == val)
// Negative is set to val MSB
uint8_t CPU6502::CPX()
{
	fetch();
	temp = (uint16_t)x - (uint16_t)fetched;
	SetFlag(C, x >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Compare Y register:
// Compares supplied value with Y register
// Carry is set to (Y >= val)
// Zero is set to (Y == val)
// Negative is set to val MSB
uint8_t CPU6502::CPY()
{
	fetch();
	temp = (uint16_t)y - (uint16_t)fetched;
	SetFlag(C, y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Decrement Memory:
// Decrements supplied value
// Zero is set to (val == 0x0000)
// Negative is set to val MSB
uint8_t CPU6502::DEC()
{
	fetch();
	temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Decrement X:
// Decrements X register
// Zero is set to (X == 0x00)
// Negative is set to X MSB
uint8_t CPU6502::DEX()
{
	x--;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Decrement Y:
// Decrements Y register
// Zero is set to (Y == 0x00)
// Negative is set to Y MSB
uint8_t CPU6502::DEY()
{
	y--;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// Bitwise Exclusive OR with Accumulator:
// Result is stored in Accumulator
// Zero is set to (A == 0x00)
// Negative is set to A MSB
uint8_t CPU6502::EOR()
{
	fetch();
	a = a ^ fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Increment Memory:
// Increments supplied value
// Zero is set to (val == 0x0000)
// Negative is set to val MSB
uint8_t CPU6502::INC()
{
	fetch();
	temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Increment X:
// Increments X register
// Zero is set to X == (0x00)
// Negative is set to X MSB
uint8_t CPU6502::INX()
{
	x++;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Increment Y:
// Increments Y register
// Zero is set to (Y == 0x00)
// Negative is set to Y MSB
uint8_t CPU6502::INY()
{
	y++;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// Jump:
// Moves Program Counter to address
uint8_t CPU6502::JMP()
{
	pc = addr_abs;
	return 0;
}

// Jump to Subroutine:
// Push Program Counter to stack, then change it to address
uint8_t CPU6502::JSR()
{
	pc--;

	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	pc = addr_abs;
	return 0;
}

// Load Accumulator:
// Loads supplied value into Accumulator
// Zero is set to (A == 0x00)
// Negative set to A MSB
uint8_t CPU6502::LDA()
{
	fetch();
	a = fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Load X register:
// Loads supplied value into X register
// Zero is set to (X == 0x00)
// Negative set to X MSB
uint8_t CPU6502::LDX()
{
	fetch();
	x = fetched;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 1;
}

// Load Y register:
// Loads supplied value into Y register
// Zero is set to (Y == 0x00)
// Negative set to Y MSB
uint8_t CPU6502::LDY()
{
	fetch();
	y = fetched;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 1;
}

// Logical shift right:
// Shift all bits one position right. 0 is shifted into MSB and LSB is shifted into Carry 
// Carry is set to (val AND 0x0001)
// Zero is set to (val == 0x0000)
// Negative is set to val MSB
uint8_t CPU6502::LSR()
{
	fetch();
	SetFlag(C, fetched & 0x0001);
	temp = fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

// No operation:
// There are various NOPs used in NES games, so far only some (popular) ones are covered here
uint8_t CPU6502::NOP()
{
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}

// Bitwise OR with Accumulator:
// Result is stored in Accumulator
// Zero is set to (A == 0x00)
// Negative is set to A MSB
uint8_t CPU6502::ORA()
{
	fetch();
	a = a | fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Push Accumulator:
// Push value from Accumulator to stack, offset by Stack Counter
uint8_t CPU6502::PHA()
{
	write(0x0100 + stkp, a);
	stkp--;
	return 0;
}

// Push Processor status
// Push value from Status register to stack, offset by Stack Counter
uint8_t CPU6502::PHP()
{
	write(0x0100 + stkp, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	stkp--;
	return 0;
}

// Pull Accumulator:
// Pull value from stack, offset by Stack Counter, to Accumulator
uint8_t CPU6502::PLA()
{
	stkp++;
	a = read(0x100 + stkp);
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}

// Pull Processor status
// Pull value from stack, offset by Stack Counter, to Status register
uint8_t CPU6502::PLP()
{
	stkp++;
	status = read(0x0100 + stkp);
	SetFlag(U, 1);
	return 0;
}

// Rotate Left:
// Shifts all bits one position left. Carry is shifted into LSB and MSB is shifted into Carry 
// Carry is set to (val > 0x00FF)
// Zero is set to (val == 0x0000)
// Negative is set to val MSB
uint8_t CPU6502::ROL()
{
	fetch();
	temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

// Rotate Right:
// Shift all bits one position right. Carry is shifted into MSB and LSB is shifted into Carry 
// Carry is set to (val AND 0x0001)
// Zero is set to (val == 0x0000)
// Negative is set to val MSB
uint8_t CPU6502::ROR()
{
	fetch();
	temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

// Return from Interrupt:
// Pops stack into Status register and 
// sets Program Counter to wherever it was before Interruption occured
uint8_t CPU6502::RTI()
{
	stkp++;
	status = read(0x0100 + stkp);
	status &= ~B;
	status &= ~U;

	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;
	return 0;
}

// Return from Subroutine:
// Sets Program Counter to wherever it was before Subroutine occured
uint8_t CPU6502::RTS()
{
	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;

	pc++;
	return 0;
}

// Subtract with Carry:
// Subtracts fetched value from Accumulator and adds Carry bit to it
// Here it inverts the fetched value and adds that to the Accumulator
// Carry is set to (val > 0x00FF)
// Zero is set to (val == 0x0000)
// Negative set to val MSB
// Overflow is set if value overflows, so to ((A XOR Result) & NOT(A XOR Fetched))
uint8_t CPU6502::SBC()
{
	fetch();

	uint16_t val = ((uint16_t)fetched) ^ 0x00FF;

	temp = (uint16_t)a + val + (uint16_t)GetFlag(C);

	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ val) & 0x0080);
	SetFlag(N, temp & 0x0080);

	a = temp & 0x00FF;
	return 1;
}

// Set Carry
uint8_t CPU6502::SEC()
{
	SetFlag(C, true);
	return 0;
}

// Set Decimal
uint8_t CPU6502::SED()
{
	SetFlag(D, true);
	return 0;
}

// Set Interrupt
uint8_t CPU6502::SEI()
{
	SetFlag(I, true);
	return 0;
}

// Store Accumulator:
// Store Accumulator at address
uint8_t CPU6502::STA()
{
	write(addr_abs, a);
	return 0;
}

// Store X register:
// Store X register at address
uint8_t CPU6502::STX()
{
	write(addr_abs, x);
	return 0;
}

// Store Y register:
// Store Y register at address
uint8_t CPU6502::STY()
{
	write(addr_abs, y);
	return 0;
}

// Transfer Accumulator to X:
// Store Accumulator in X register
// Zero is set to (X == 0x00)
// Negative set to X MSB
uint8_t CPU6502::TAX()
{
	x = a;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Transfer Accumulator to Y:
// Store Accumulator in Y register
// Zero is set to (Y == 0x00)
// Negative set to Y MSB
uint8_t CPU6502::TAY()
{
	y = a;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// Transfer Stack pointer to X:
// Store Stack Pointer in X register
// Zero is set to (X == 0x00)
// Negative set to X MSB
uint8_t CPU6502::TSX()
{
	x = stkp;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Transfer X to Accumulator:
// Store X register in Accumulator
// Zero is set to (A == 0x00)
// Negative set to A MSB
uint8_t CPU6502::TXA()
{
	a = x;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}

// Transfer X to Stack pointer
// Store X register in Stack pointer
// Zero is set to (S == 0x00)
// Negative set to S MSB
uint8_t CPU6502::TXS()
{
	stkp = x;
	return 0;
}

// Transfer Y to Accumulator
// Store Y register in Accumulator
// Zero is set to (A == 0x00)
// Negative set to A MSB
uint8_t CPU6502::TYA()
{
	a = y;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}

// Non-defined Opcode
uint8_t CPU6502::XXX()
{
	return 0;
}

// True if no cycles left - computation completed
bool CPU6502::complete()
{
	return cycles == 0;
}