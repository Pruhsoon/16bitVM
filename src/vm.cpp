#include "vm.h"

void __ste(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) |= 0x08;
}
void __stg(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) |= 0x04;
}
void __sth(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) |= 0x02;
}
void __stl(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) |= 0x01;
}
void __cle(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) &= 0x07;
}
void __clg(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) &= 0x0c;
}
void __clh(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) &= 0x0d;
}
void __cll(VM *vm, Opcode opcode, Args a1, Args a2) {
    FLAGS(vm) &= 0x0e;
}

static IM instrmap[] = {
    { nop, 0x01 },
    { hlt, 0x01 },
    { mov_ax, 0x03 },
    { mov_bx, 0x03 },
    { mov_cx, 0x03 },
    { mov_dx, 0x03 },
    { mov_sp, 0x03 },
    { mov_mem_ax, 0x03 },
    { mov_mem_bx, 0x03 },
    { mov_mem_dx, 0x03 },
    { ste, 0x01 },
    { stg, 0x01 },
    { stl, 0x01 },
    { sth, 0x01 },
    { cle, 0x01 },
    { clg, 0x01 },
    { cll, 0x01 },
    { clh, 0x01 },
    {push, 0x03 },
    { pop, 0x03 }
};

void __push(VM *vm, Opcode opcode, Args a1, Args a2) {
    int16 src;
    int16 *dst;
    void *mem;

    if (higher((vm)) || lower(vm))
        error(vm, ErrInstr);
    if (SP(vm) < 2)
        error(vm, ErrInstr);
    /* (xx) What if b < 2?*/
    if (SP(vm) < (vm->b-2))
        error(vm, ErrSegv);

    switch (a1) {
        /* push ax = 00 00 */
        case 0x00:
            src = AX(vm);
            break;
         /* push bx = 00 01 */
        case 0x01:
            src = BX(vm);
            break;
         /* push cx = 00 10 */
        case 0x02:
            src = CX(vm);
            break;
         /* push dx = 00 11 */
        case 0x03:
            src = DX(vm);
            break;
        default:
            error(vm, ErrInstr);
    }

    SP(vm) -= 2;
    copy(vm->m + SP(vm), (int8*)&src, 2);

    return;
}

void __pop(VM *vm, Opcode opcode, Args a1, Args a2) {
    int16 *src;
    void *mem;

    if (higher(vm) || lower(vm))
        error(vm, ErrInstr);
    if (SP(vm) > 0xfffd)
        error(vm, ErrInstr);

    copy((int8*)&src, vm->m + SP(vm), 2); 

    switch (a1) {
        /* push ax = 00 00 */
        case 0x00:
            AX(vm) = *src;
            break;
         /* push bx = 00 01 */
        case 0x01:
            BX(vm) = *src;
            break;
         /* push cx = 00 10 */
        case 0x02:
            CX(vm) = *src;
            break;
         /* push dx = 00 11 */
        case 0x03:
            DX(vm) = *src;
            break;
        default:
            error(vm, ErrInstr);
    }
    SP(vm) += 2;

    return;
}

void __mov(VM *vm, Opcode opcode, Args a1, Args a2) {
    int16 dst;

    dst  = $2 a1;
    if (higher(vm) && lower(vm))
        error(vm, ErrInstr);

    switch (opcode) {
        /* mov ax      ;1000 */
        case 0x08:
            if (higher(vm))
                AX(vm) = (((Reg)a1 <<8) | (AX(vm) & 0xFF));
            else if (lower(vm))
                AX(vm) = ((Reg)a1 | (AX(vm) & 0xFF00));
            else
                AX(vm) = (Reg)a1;
            break;

        /* mov bx      ; 1001 */
        case 0x09:
            if (higher(vm))
                BX(vm) = (((Reg)a1 <<8) | (BX(vm) & 0xFF));
            else if (lower(vm))
                BX(vm) = ((Reg)a1 | (BX(vm) & 0xFF00));
            else
                BX(vm) = (Reg)a1;
            break;

        /* mov cx      ; 1010 */
        case 0x0a:
            if (higher(vm))
                CX(vm) = (((Reg)a1 <<8) | (CX(vm) & 0xFF));
            else if (lower(vm))
                CX(vm) = ((Reg)a1 | (CX(vm) & 0xFF00));
            else
                CX(vm) = (Reg)a1;
            break;

        /* mov dx      ; 1011 */
        case 0x0b:
            if (higher(vm))
                DX(vm) = (((Reg)a1 <<8) | (DX(vm) & 0xFF));
            else if (lower(vm))
                DX(vm) = ((Reg)a1 | (DX(vm) & 0xFF00));
            else
                DX(vm) = (Reg)a1;
            break;

        /* mov sp      ; 1100 */
        case 0x0c:
                SP(vm) = (Reg)dst;
                break;

        /* mov [addr],ax    ; 1101 */
        case 0x0d:
            break;

        /* mov [addr],bx    ; 1110 */
        case 0x0e:
            break;

        /* mov [addr],dx     ; 1111 */
        case 0x0f:
            break;

        default:
            error(vm, ErrInstr);
        break;
}
// 1 0000
// 15 = 1111 = mov [0xaabb],0x05
// 1 - 15

return;
}

void execinstr(VM* vm, Program *p) {
    Args a1, a2;
    int16 size;

    a1=a2 = 0;
    size = map((Opcode)*p);

    switch (size) {
        case 1:
            break;

        case 2:
            a1 = *(p+1);
            break;

        case 3:
            a1 = (
                (((int16)*(p+2) & 0xff) << 8)
                    | ((int16)*(p+1) & 0xff)
            );

            break;

        case 5:
            a1 = (
                (((int16)*(p+2) & 0xff) << 8)
                    | ((int16)*(p+1) & 0xff)
            );

            a2 = (
                (((int16)*(p+4) & 0xff) << 8)
                    | ((int16)*(p+3) & 0xff)
            );

            break;

        default:
            segfault(vm);
            break;
    }

    switch (*p) {
        case mov_ax: 
        case 0x09 ... 0x0f:
            __mov(vm, (Opcode)*p, a1, a2);
            break;

        case nop:
            break;
        
        case hlt:
            error(vm, SysHlt);
            break;

        case ste:    __ste(vm, (Opcode)*p, a1, a2); break;
        case stg:    __stg(vm, (Opcode)*p, a1, a2); break;
        case sth:    __sth(vm, (Opcode)*p, a1, a2); break;
        case stl:    __stl(vm, (Opcode)*p, a1, a2); break;
        case cle:    __cle(vm, (Opcode)*p, a1, a2); break;
        case clg:    __clg(vm, (Opcode)*p, a1, a2); break;
        case clh:    __clh(vm, (Opcode)*p, a1, a2); break;
        case cll:    __cll(vm, (Opcode)*p, a1, a2); break;
        case push:  __push(vm, (Opcode)*p, a1, a2); break;
        case pop:    __pop(vm, (Opcode)*p, a1, a2); break;
    }

    return;
}

void execute(VM *vm) {
    int32 brkaddr;
    Program *pp;
    int16 size;
    Instruction ip;

    assert(vm && *vm->m);
    size = 0;
    brkaddr = ((uintptr_t)vm->m + vm->b);
    if ((uintptr_t)pp > brkaddr)

    /* instr1 arg1 instr2 instr3 */
    /* mov ax,0x05; nop; hlt; */
    /* 0x01 0x00 0x05 */
    /* 0x02 */
    /* 0x03 */

    // pp -> 3
    // brk -> 5
    // brkaddr = 0x01 0x00 0x05
    //           0x02 0x03 X

    do {
        IP(vm) += size;
        pp += size;

       if ((int32)pp > brkaddr)
             segfault(vm);
            size = map((Opcode)*pp);
        execinstr(vm, pp);
    } while (*pp != (Opcode)hlt);

    return;
}

void error(VM* vm, Errorcode e) {
    int8 exitcode;

    exitcode = -1;
    switch(e) {
        case ErrSegv:
            fprintf(stderr, "%s\n", "VM Segmentation fault");
            break;

        case ErrInstr:
            fprintf(stderr, "%s\n", "VM Illegal instruction");
            break;

        /* (xx) */
        case SysHlt:
            fprintf(stderr, "%s\n", "System halted");
            exitcode = 0;
            printf("ax = %.04hx\n", $i AX(vm));
            printf("bx = %.04hx\n", $i BX(vm));
            printf("sp = %.04hx\n", $i SP(vm));
            if (equal(vm))
                printf("E flag set\n");
            if (gt(vm))
                printf("GT flag set\n");
            
            printhex(vm->m + 0xffff - 32, 32, 0);

            break;

        default:
            break;
    }
    if (vm)
        free(vm);

    exit($i exitcode);
}

int8 map(Opcode o) {
    int8 n, ret;
    IM *p;

    ret = 0;
    for (n=IMs, p=instrmap; n; n--, p++)
        if (p->o == o) {
            ret = p->s;
            break;
        }
    
    return ret;
}

VM *virtualmachine() {
    VM *p;
    int16 size;

    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p) {
        errno = ErrMem;
        return (VM *)0;
    }
    zero($1 p, size);
    SP(p) = 0xffff;

     return p;
}

Program *exampleprogram(VM *vm, ...) {
    Program *p, *instr;
    va_list ap;
    Opcode opc;
    int8 size;

    va_start(ap, vm);
    p = vm->m;

    do {
        instr = va_arg(ap, Program*);
        opc = (Opcode)*instr;
        size = map(opc);
        copy($1 p, $1 instr, size);
        p += size;
        vm->b += size;
    } while(opc != hlt);
    va_end(ap);

    return p;
}

Instruction *i0(Opcode op) {
    Instruction *i;
    int8 size;

    size = map(op);
    i = (Instruction *)malloc(size);
    assert(i);
    zero($1 i, size);
    i->o = op;

    return i;
}

Instruction *i1(Opcode op, Args a1) {
    Instruction *i;
    int8 size;

    size = map(op);
    i = (Instruction *)malloc(size);
    assert(i);
    zero($1 i, size);
    i->o = op;
    i->a[0] = a1;

    return i;
}

Instruction *i2(Opcode op, Args a1, Args a2) {
    Instruction *i;
    int8 size;

    size = map(op);
    i = (Instruction *)malloc(size);
    assert(i);
    zero($1 i, size);
    i->o = op;
    i->a[0] = a1;
    i->a[1] = a2;

    return i;
}

Program *i(Instruction *i) {
    Program *p;
    int8 size;

    size = map(i->o);
    p = (Program *)malloc($i size);
    assert(p);
    copy(p, $1 i, 1);
    copy((p+1), $1 i->a, (size-1));

    return p;
}

int main(int argc, char *argv[]) {
    Program *prog;
    VM *vm;

    vm = virtualmachine();
    prog = exampleprogram(vm,
        i(i1(mov_ax, 0x04)), i(i0(ste)), i(i1(push, 0x00)),
        i(i1(mov_bx, 0x5005)), i(i1(pop, 0x01)),
        
        
        i(i0(hlt)) 
    );
    printf("vm   = %p (sz: %zu)\n", (void*)vm, sizeof(struct s_vm));


    //prog = exampleprogram(vm);
    printf("prog = %p\n", prog);

    execute(vm);

    printhex($1 prog, (map(mov_ax)+map(nop)+map(hlt)), ' ');

    return 0;
}

#pragma GCC diagnostic pop