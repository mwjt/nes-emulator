#pragma once
#include <vector>
#include <string>
#include <map>

class Bus;

class CPU6502
{
public:
	CPU6502();
	~CPU6502();

	// Status register flags
	enum FLAGS
	{
		C = (1 << 0),	// Carry
		Z = (1 << 1),	// Zero
		I = (1 << 2),	// IRQ Disable
		D = (1 << 3),	// Decimal Mode - not used in NES
		B = (1 << 4),	// BRK Command
		U = (1 << 5),	// Unused
		V = (1 << 6),	// Overflow
		N = (1 << 7),	// Negative
	};

	uint8_t a = 0x00;		// Accumulator
	uint8_t x = 0x00;		// Index register
	uint8_t y = 0x00;		// Index register
	uint16_t pc = 0x0000;	// Program Counter
	uint8_t stkp = 0x00;	// Stack Counter
	uint8_t status = 0x00;	// Status register

	void Connect(Bus* bus)
	{
		this->bus = bus;
	}

	void clock();		// 1 clock cycle
	void reset();		// Reset signal
	void irq();			// Interrupt Request signal
	void nmi();			// Non-maskable Interrupt Request signal

	bool complete();

private:
	// Access status register
	uint8_t		GetFlag(FLAGS flag);
	void		SetFlag(FLAGS flag, bool val);

	uint8_t		fetched		= 0x00;			// Working input value
	uint16_t	temp		= 0x0000;		// Helping variable
	uint16_t	addr_abs	= 0x0000;		// Read location 
	uint16_t	addr_rel	= 0x00;			// Branch location
	uint8_t		opcode		= 0x00;			// Store executed Opcode number
	uint8_t		cycles		= 0;			// Store cycles left for duration of instruction

	Bus			*bus		= nullptr;
	uint8_t		read(uint16_t addr);
	void		write(uint16_t addr, uint8_t data);

	uint8_t fetch();

	// Opcode entry in lookup table
	struct INSTRUCTION
	{
		std::string name;
		uint8_t(CPU6502::*operate)(void) = nullptr;
		uint8_t(CPU6502::*addrmode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	// Lookup table, to find cycles count for opcodes
	std::vector<INSTRUCTION> lookup;

	// Addressing Modes
	uint8_t IMM();		// Immediate Addressing
	uint8_t ABS();		// Absolute Addressing
	uint8_t ZP0();		// Zero Page Addressing
	uint8_t ZPX();		// Indexed Zero Page Addressing
	uint8_t ZPY();		// Indexed Zero Page Addressing
	uint8_t ABX();		// Indexed Absolute Addressing
	uint8_t ABY();		// Indexed Absolute Addressing
	uint8_t IMP();		// Implied Addressing
	uint8_t REL();		// Relative Addressing
	uint8_t IZX();		// Indexed Indirect Addressing
	uint8_t IZY();		// Indirect Indexed Addressing
	uint8_t IND();		// Absolute Indirect

	// Opcodes
	uint8_t ADC();		// Add with Carry
	uint8_t AND();		// Bitwise AND with Accumulator
	uint8_t ASL();		// Arithmetic Shift Left
	uint8_t BCC();		// Branch on Carry Clear
	uint8_t BCS();		// Branch on Carry Set
	uint8_t BEQ();		// Branch on Equal
	uint8_t BIT();		// Test Bits
	uint8_t BMI();		// Branch on Minus
	uint8_t BNE();		// Branch on Not Equal
	uint8_t BPL();		// Branch on Plus
	uint8_t BRK();		// Break
	uint8_t BVC();		// Branch on Overflow Clear
	uint8_t BVS();		// Branch on Overflow Set
	uint8_t CLC();		// Clear Carry
	uint8_t CLD();		// Clear Decimal
	uint8_t CLI();		// Clear Interrupt
	uint8_t CLV();		// Clear Overflow
	uint8_t CMP();		// Compare Accumulator
	uint8_t CPX();		// Compare X register
	uint8_t CPY();		// Compare Y register
	uint8_t DEC();		// Decrement Memory
	uint8_t DEX();		// Decrement X
	uint8_t DEY();		// Decrement Y
	uint8_t EOR();		// Bitwise Exclusive OR
	uint8_t INC();		// Increment Memory
	uint8_t INX();		// Increment X
	uint8_t INY();		// Increment Y
	uint8_t JMP();		// Jump
	uint8_t JSR();		// Jump to Subroutine
	uint8_t LDA();		// Load Accumulator
	uint8_t LDX();		// Load X register
	uint8_t LDY();		// Load Y register
	uint8_t LSR();		// Logical shift right
	uint8_t NOP();		// No operation
	uint8_t ORA();		// Bitwise OR with Accumulator
	uint8_t PHA();		// Push Accumulator
	uint8_t PHP();		// Push Processor status
	uint8_t PLA();		// Pull Accumulator
	uint8_t PLP();		// Pull Processor status
	uint8_t ROL();		// Rotate Left
	uint8_t ROR();		// Rotate Right
	uint8_t RTI();		// Return from Interrupt
	uint8_t RTS();		// Return from Subroutine
	uint8_t SBC();		// Subtract with Carry
	uint8_t SEC();		// Store Accumulator
	uint8_t SED();		// Set Decimal
	uint8_t SEI();		// Set Interrupt
	uint8_t STA();		// Store Accumulator
	uint8_t STX();		// Store X register
	uint8_t STY();		// Store Y register
	uint8_t TAX();		// Transfer Accumulator to X
	uint8_t TAY();		// Transfer Accumulator to Y
	uint8_t TSX();		// Transfer X to Stack pointer
	uint8_t TXA();		// Transfer X to Accumulator
	uint8_t TXS();		// Transfer Stack pointer to X
	uint8_t TYA();		// Transfer Y to Accumulator

	uint8_t XXX();		// Non-defined Opcode

};
