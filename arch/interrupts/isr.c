/*
 *   GoldenEagle Bootloader C entry point.
 *   Copyright (C) 2011  Michel Megens
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/stdlib.h>
#include <error/panic.h>
#include "include/interrupts.h"
#include <textio.h>

bool inKernelRing()
{
	gebl_segments_t * segs = getsegs();
	uint8_t * error = "Not in kernel ring!";
	
	if((segs->cs & 0x8) == 0)
	{
		panic(error);
	}
	if((segs->ds & 0x10) == 0)
	{
		panic(error);
	}
	
	return TRUE;
}

void cDivByZero(gebl_isr_stack regs)
{
	print("\nI define you as idiot. You just tried to divede by zero. Code \nfailed at EIP: ");
	printnum(regs.eip, 16, FALSE, FALSE);
	putc(0xa);
	panic("");
}
void cNmi()
{
	panic("Unknown interrupt!");
}
void cBreakp()
{
	panic("Not yet implemented");
}
void cOverflow()
{
	panic("Not yet implemented");
}
void cBound()
{
	panic("Not yet implemented");
}
void cInvalOp()
{
	panic("Not yet implemented");
}
void cNoMath()
{
	panic("Not yet implemented");
}
void cDoubleFault()
{
	panic("Double fault!");
}
void cDepricated()
{
	println("depricated");
}
void cInvalidTSS()
{
	panic("Not yet implemented");
}
void cSnp()
{
	panic("Not yet implemented");
}
void cStackFault()
{
	panic("Not yet implemented");
}
void cGenProt(gebl_isr_stack regs)
{
	print("General protection fault! Fault occured at: ");
	printnum(regs.eip, 16, FALSE, FALSE);
	putc(0xa);
	panic("");
}
void cPageFault()
{
	panic("Not yet implemented");
}
void cFpu()
{
	panic("Not yet implemented");
}
void cAlligned()
{
	panic("Not yet implemented");
}
void cMachine()
{
	panic("Not yet implemented");
}
void cSimd()
{
	panic("Not yet implemented");
}