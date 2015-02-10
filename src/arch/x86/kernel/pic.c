/*
 *   PIC functions
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

#include <stdlib.h>
#include <text.h>

#include <andromeda/panic.h>
#include <andromeda/error.h>

#include <arch/x86/pic.h>
#include <arch/x86/irq.h>

void pic_8259_remap(uint32_t offset1, uint32_t offset2)
{
        uint8_t mask1, mask2;

        mask1 = inb(X86_8259_PIC1_DATA);
        mask2 = inb(X86_8259_PIC2_DATA);

        // send the initialise commands to the master and slave pic
        outb(X86_8259_PIC1_COMMAND, X86_8259_ICW1_INIT | X86_8259_ICW1_ICW4);
        outb(X86_8259_PIC2_COMMAND, X86_8259_ICW1_INIT | X86_8259_ICW1_ICW4);
        iowait();

        // define the vectors
        outb(X86_8259_PIC1_DATA, offset1);
        outb(X86_8259_PIC2_DATA, offset2);
        iowait();

        // connect to the slave
        outb(X86_8259_PIC1_DATA, X86_8259_ICW3_MASTER);
        outb(X86_8259_PIC2_DATA, X86_8259_ICW3_SLAVE);
        iowait();

        outb(X86_8259_PIC1_DATA, X86_8259_ICW4_8086);
        outb(X86_8259_PIC2_DATA, X86_8259_ICW4_8086);
        iowait();

        outb(X86_8259_PIC1_DATA, mask1);
        outb(X86_8259_PIC2_DATA, mask2);
        iowait();

        outb(0x21, 0x3c);
        outb(0xa1, 0x3e);
}

void pic_8259_disable()
{
        outb(X86_8259_PIC1_DATA, X86_8259_PIC_DISABLE);
        outb(X86_8259_PIC2_DATA, X86_8259_PIC_DISABLE);
}

void pic_8259_eoi(uint8_t irq)
{
        if (irq >= 8) {
                outb(X86_8259_PIC2_COMMAND, X86_8259_PIC_EOI);
        }
        outb(X86_8259_PIC1_COMMAND, X86_8259_PIC_EOI);
}

int pic_8259_clear_irq_mask(uint8_t irq)
{
        uint16_t port = (irq < 8) ? X86_8259_PIC1_DATA : X86_8259_PIC2_DATA;
        uint8_t mask = inb(port) & ~(BIT((irq < 8) ? irq : (irq - 8)));
        outb(port, mask);
        printf("Masking irq: %x - new mask: %x\n", irq, inb(port));

        return -E_NOFUNCTION;
}

void pic_8259_init()
{
        pic_8259_remap(X86_8259_INTERRUPT_BASE, X86_8259_INTERRUPT_BASE + 8);
}
