# 16-Bit Virtual Machine (VM)

This project implements a simple **16-bit virtual machine**, complete with its own CPU registers, stack, memory, instruction set, assembler helpers, and execution engine. It is built to emulate a small, custom low-level CPU architecture that can execute machine-level programs.

---

## Overview

This VM simulates a tiny CPU with:

* 4 general-purpose **16-bit registers**: `AX`, `BX`, `CX`, `DX`
* A **16-bit Stack Pointer (SP)`**
* A **16-bit Instruction Pointer (IP)`**
* A **FLAGS register** with simple CPU flags
* **64 KB of memory**
* A **bytecode instruction set** (custom ISA)
* **Instruction handlers** written in C to emulate CPU behavior

Programs are written using helper macros that generate machine code automatically.

---

##  Instruction Set Highlights

The VM currently supports instructions such as:

* **mov ax, imm16** — load a 16-bit value into AX
* **mov bx, imm16**, etc.
* **push reg** — push a register onto the stack
* **pop reg** — restore a register from the stack
* **ste** — set Equal flag
* **cle** — clear Equal flag
* **jmp, je, jne** — control flow
* **hlt** — halt execution

Each instruction has:

* an **opcode**
* a **size**
* a **handler function**

When the VM runs, it:

1. Fetches opcode from memory
2. Decodes it using an opcode table
3. Reads arguments (if any)
4. Calls the corresponding handler (C function)
5. Updates registers/memory/flags
6. Moves to next instruction

This simulates a real processor pipeline.

---

##  How Execution Works

When you run the VM:

1. Memory is initialized
2. CPU registers are reset
3. A program is assembled into bytecode using helper macros
4. The bytecode is loaded into VM memory at address 0
5. The main execution loop starts:

   * Fetch → Decode → Execute → Repeat
6. Execution stops when `hlt` is encountered

The VM prints out register state when halting.

---

##  Project Structure

* **vm.h / vm.c** — Core VM implementation
* **opcode.h / opcode.c** — Instruction definitions and handlers
* **assembler.h** — Helper macros for generating bytecode
* **example.c** — Shows how to build and run a sample program
* **types.h** — Shared types for arguments and opcode mapping

---

##  Tech Stack

This project is implemented using:

### **Languages**

* **C** — Core implementation of VM, memory model, registers, and instructions

### **Tools & Concepts**

* **Custom Instruction Set Architecture (ISA)**
* **Function pointer-based dispatch table** (for opcodes)
* **Manual memory & stack simulation**
* **Endian-aware byte reading/writing**
* **Assembler-like macros to generate bytecode programs**

No external libraries are required — it's a pure C, self-contained VM.

---

##  Example Program

```c
mov ax, 0x04
ste
push ax
mov bx, 0x5005
pop bx
hlt
```

This program sets AX to 4, pushes it, pops into BX, and halts.

When the VM halts, the final registers are:

```
AX = 0004
BX = 0004
SP = FFFF
FLAGS = 0008
```

---

##  Running the VM

Compile and run using:

```bash
gcc -o vm main.c vm.c opcode.c assembler.c
./vm
```

---

##  Extend the VM

You can easily add new instructions:

1. Add a new opcode value to the enum
2. Create a handler function
3. Add it to the instruction map
4. Use assembler macros to generate bytecode

For example, adding `add ax, bx` takes only a few lines.

---

##  Summary

This project is a complete educational CPU emulator, demonstrating:

* how processors interpret opcodes
* how registers and stacks work internally
* how to create a custom instruction set
* how to build bytecode programs
* how interpreters execute low-level instructions


