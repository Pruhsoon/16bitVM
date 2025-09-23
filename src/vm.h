#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <cstdint>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include "utils.h"

#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic push


#define NoErr       0x00    /* 00 00 */
#define SysHlt      0x01    /* 00 01 */
#define ErrMem      0x02    /* 00 10 */
#define ErrSegv     0x04    /* 01 00 */
#define ErrInstr    0x08    /* 10 00 */

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)
#define $c (char *)
#define $i (int)
#define MEMSIZE 65536


#define AX(vm) ((vm)->c.r.ax)
#define BX(vm) ((vm)->c.r.bx)
#define CX(vm) ((vm)->c.r.cx)
#define DX(vm) ((vm)->c.r.dx)
#define SP(vm) ((vm)->c.r.sp)
#define IP(vm) ((vm)->c.r.ip)
#define FLAGS(vm) ((vm)->c.r.flags)

#define equal(vm)    (!!((FLAGS(vm) & 0x08) >> 3))
#define gt(vm)       (!!((FLAGS(vm) & 0x04) >> 2))
#define higher(vm)   (!!((FLAGS(vm) & 0x02) >> 1))
#define lower(vm)    (!!( FLAGS(vm) & 0x01))


#define segfault(x)     error((x), ErrSegv)

/*

16 bit
AX 16bit
- AL 8bit
- AH 8bit
BX
CX
DX
SP
IP
FLAGS

1010
^----
Zero flag
Carry flag

CMP AX,0x05

65 KB memory
(Serial COM port)
(Floppy drive)

*/

typedef unsigned short int Reg;
struct s_registers {
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
    Reg flags;
    /*  P1 E - Equal flag
    P2 G - Greater-than flag
    P3 H - Higher part of reg
    P4 L - Lower part of reg    */
};
typedef struct s_registers Registers;

struct s_cpu {
    Registers r;
};
typedef struct s_cpu CPU;

/*
mov ax, 0x05 //(0x01 OR ax)
// 0000 0011 = mov
// 0000 0000
// 0000 0101 = 0x05
0001-ax
0010 - bx
0100 -cx
1011 -sp
*/

enum e_opcode {
    nop = 0x01,
    hlt = 0x02, 
    mov_ax = 0x08, /* 0x08 - 0x0f*/
    mov_bx = 0x09,
    mov_cx = 0x0a,
    mov_dx = 0x0b,
    mov_sp = 0x0c,
    mov_mem_ax = 0x0d,
    mov_mem_bx = 0x0e,
    mov_mem_dx = 0x0f,
    ste = 0x10,
    cle = 0x11,
    stg = 0x12,
    clg = 0x13,
    sth = 0x14,
    clh = 0x15,
    stl = 0x16,
    cll = 0x17,
     /*
        reserved = 0x18,
        reserved = 0x19
     */
    push = 0x1a,
    pop = 0x1b
};
typedef enum e_opcode Opcode;

struct s_instrmap {
    Opcode o;
    int8 s;
};
typedef struct s_instrmap IM;

typedef int16 Args;

struct s_instruction {
    Opcode o;
    Args a[]; /* 0-2 bytes */
};
typedef struct s_instruction Instruction;

typedef unsigned char Errorcode;
typedef int8 Memory[MEMSIZE];
typedef int8 Program;
/*
a program is a seq of instruction,
a instruction is a series of opcode
*/

struct s_vm {
    CPU c;
    Memory m;
    int16 b; /* break line */
};
typedef struct s_vm VM;
typedef Memory *Stack;


#define IMs (sizeof(instrmap) / sizeof(struct s_instrmap))

void __ste(VM*,Opcode,Args,Args);
void __stg(VM*,Opcode,Args,Args); 
void __sth(VM*,Opcode,Args,Args); 
void __stl(VM*,Opcode,Args,Args); 
void __cle(VM*,Opcode,Args,Args); 
void __clg(VM*,Opcode,Args,Args); 
void __clh(VM*,Opcode,Args,Args); 
void __cll(VM*,Opcode,Args,Args); 
void __push(VM*,Opcode,Args,Args);
void __pop(VM*,Opcode,Args,Args);
void __mov(VM*,Opcode,Args,Args);

void error(VM*,Errorcode);
void execinstr(VM*,Program*);
void execute(VM*);
Program *i(Instruction*);
Instruction *i0(Opcode);
Instruction *i1(Opcode,Args);
Instruction *i2(Opcode,Args,Args);
// Program *exampleprogram(VM*);
int8 map(Opcode);
VM *virtualmachine(void);
int main(int,char**);

/*
Section .text (code) +read +exec -write
 ########
 #######
 ------------------------------

 ****
 *********
 **********
Section .data +read -exec +write
*/