/*

Máquina Sencilla / Simple Machine (MS/SM)

Features:

- Instructions of 16 bits
- No registers manipulation
- Addressing of 7 bits
- Memory 128x16
- Flag Zero flip-flop (FZ in RFZ)

ALU:

- Sum
- XOR

Memory:

- 128 Positions x 16 bits of each block
- RAM Type

Buses:

- 7 bits of address bus
- 16 bits of data bus

Machine instruction description

Direct addressing mode

Instruction Format:

 /_ _/_ _ _ _ _ _ _/_ _ _ _ _ _ _/
OPCOD  Address A     Address B

OPCOD (Operation Code): 2 Bits
Address A: 7 Bits
Address B: 7 Bits

INSTRUCTIONS:

Sum:

ADD (A),(B)
(B)<-RA+RB

FZ=1 -> RA+RB=0
FZ=0 -> RA+RB~=0

Move:

MOV (A), (B)
(B)<-RA

Compare:

CMP (A),(B)
RA xor RB
FZ=1 -> RA xor RB=0
FZ=0 -> RA xor RB~=0

Branch Equal (BEQ):

BEQ (B)

FZ=1 -> RI<-(RB)
        PC<-RB+1
FZ=0 -> RI<-(PC)
        PC<-PC+1

µinstructions:

ADD:

RI<-(PC); PC <- PC+1
Decode instruction
RB<-(B)
RA<-(A)
(B)<-A+B; Change FZ

CMP:

RI<-(PC); PC <- PC+1
Decode instruction
RB<-(B)
RA<-(A)
RA xor RB; Change FZ

MOV:

RI<-(PC); PC <- PC+1
Decode instruction
RA<-(A)
(B)<-RA

BEQ:

RI<-(PC); PC <- PC+1
Decode instruction
FZ evaluation
if FZ=1; RI<-(B); PC<-B+1

*/

#include <stdio.h>

// Memory of 128 positions of 16 bits
unsigned short memory[0x7F]=
{
0x9030, 0x9031, 0x58b4, 0xc008, 0x1ab0, 0x10b1, 0x4810, 0xc002, 0x4810, 0xc009,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1
};

unsigned short PC, IR, RA, RB; // Registers
unsigned char RFZ; // Flag when zero (Boolean)
unsigned char BEQ; // For BEQ special state

int main ()
{
	memory[0x34]=0x3;
	memory[0x35]=0xd;
	PC=0;
	RFZ=0;
	BEQ=0;

	printf("Inicio Emulación\n==================================\n\n");

        while(1)
	{
		// Instructions Execution
                if (BEQ == 0)
                {
					IR=memory[PC];
					PC+=1;
                }
                else
                {
					BEQ=0;
                }
		switch(IR >> 14)
		{
			case 0:
                // ADD
				RA = memory[(IR & 0x3F80)>>7];
				RB = memory[IR & 0x7F];
				memory[IR&0x7F] += RA + RB;
				if (memory[IR & 0x7F] == 0)
				{
					RFZ=1;
				}
				else
				{
					RFZ=0;
				}
				break;
			case 1:
				// CMP
				RA = memory[(IR & 0x3F80)>>7];
				RB = memory[IR & 0x7F];

				if ((RB ^ RA) == 0)
				{
					RFZ=1;
				}
				else
				{
					RFZ=0;
				}
                break;
			case 2:
				// MOV
                RB=memory[IR & 0x7F];
				memory[(IR & 0x3F80)>>7] = RB;
                break;
			case 3:
				// BEQ
				if(RFZ == 1)
				{
                    PC=(IR & 0x7F) + 1;
					IR=memory[IR & 0x7F];
                    BEQ=1;
				}
                break;
		}

		printf("PC: 0x%x\n", PC);
		printf("RA: 0x%x\n", RA);
		printf("RB: 0x%x\n", RB);
		printf("IR: 0x%x\n", IR);
		printf("RFZ: 0x%x\n\n", RFZ);
        //system("pause");
	}
}
