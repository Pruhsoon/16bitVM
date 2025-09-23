16bitVM

A lightweight 16-bit Virtual Machine implemented in C++, simulating registers, memory, and instruction execution.
This project demonstrates low-level system design concepts such as instruction decoding, stack operations, and program execution.

📂 Project Structure
VM/
├── SRC/
│   ├── vm.cpp      # Core implementation of the VM (CPU, execution, instructions)
│   ├── vm.h        # VM data structures, opcodes, instruction set
│   ├── utils.h     # Utility functions (copy, helpers, etc.)
├── Makefile        # Build automation for Linux/WSL/Unix

⚙️ Functionality

Implements a 16-bit CPU model with registers: AX, BX, CX, DX, SP, IP.

Supports instruction set (e.g., mov, push, pop, hlt, conditional ops).

Provides a stack-based memory model with push/pop functionality.

Executes programs represented as sequences of instructions.

Modular design: instructions are mapped via instrmap for easy extension.

🚀 How to Build & Run
1️⃣ Clone the repository
git clone https://github.com/Pruhsoon/16bitVM.git
cd 16bitVM

2️⃣ Build the project (using Make)

On Linux/WSL/macOS:

make


On Windows (PowerShell/CMD) (with MinGW or similar):

mingw32-make

3️⃣ Run the VM
./16bitVM


Clean build artifacts:

make clean

🛠️ Tech Stack

C++11

Make (build system)

Designed for cross-platform use (Linux, WSL, Windows via MinGW)

📘 Example Usage

The VM can execute programs defined as sequences of instructions, for example:

prog = exampleprogram(vm,
    i(i1(mov_ax, 0x04)),
    i(i0(ste)),
    i(i1(push, 0x00)),
    i(i1(pop, 0x01)),
    i(i0(hlt))
);

✨ Future Improvements

Add more instructions (arithmetic, branching).

Implement I/O devices (e.g., serial, floppy simulation).

Extend to support assembly parsing instead of hardcoded instruction arrays.
