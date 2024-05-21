#include "interpreter.hpp"

// TODO: Add support to read memory

uintptr_t ENTRY_POINT_ADDRESS = 0x100000;
uintptr_t MEMORY_ALLOCATION_SIZE = 2 * 1024 * 1024;
uintptr_t STACK_ADDRESS = 0x300000;
uint64_t CODE_BUF_SIZE = 0x3000;
uintptr_t STACK_SIZE = 5 * 1024 * 1024;

uc_engine *uc = nullptr;

int regNameToConstant(std::string name){
    std::unordered_map<std::string, int> regMap = {
        {"rax", UC_X86_REG_RAX},
        {"rbx", UC_X86_REG_RBX},
        {"rcx", UC_X86_REG_RCX},
        {"rdx", UC_X86_REG_RDX},
        {"rsi", UC_X86_REG_RSI},
        {"rdi", UC_X86_REG_RDI},
        {"rbp", UC_X86_REG_RBP},
        {"rsp", UC_X86_REG_RSP},
        {"r8", UC_X86_REG_R8},
        {"r9", UC_X86_REG_R9},
        {"r10", UC_X86_REG_R10},
        {"r11", UC_X86_REG_R11},
        {"r12", UC_X86_REG_R12},
        {"r13", UC_X86_REG_R13},
        {"r14", UC_X86_REG_R14},
        {"r15", UC_X86_REG_R15},
        {"rip", UC_X86_REG_RIP},
        {"ah", UC_X86_REG_AH},
        {"al", UC_X86_REG_AL},
        {"ax", UC_X86_REG_AX},
        {"bh", UC_X86_REG_BH},
        {"bl", UC_X86_REG_BL},
        {"bx", UC_X86_REG_BX},
        {"ch", UC_X86_REG_CH},
        {"cl", UC_X86_REG_CL},
        {"cx", UC_X86_REG_CX},
        {"dh", UC_X86_REG_DH},
        {"dl", UC_X86_REG_DL},
        {"dx", UC_X86_REG_DX},
        {"si", UC_X86_REG_SI},
        {"di", UC_X86_REG_DI},
        {"bp", UC_X86_REG_BP},
        {"sp", UC_X86_REG_SP},
        {"r8d", UC_X86_REG_R8D},
        {"r9d", UC_X86_REG_R9D},
        {"r10d", UC_X86_REG_R10D},
        {"r11d", UC_X86_REG_R11D},
        {"r12d", UC_X86_REG_R12D},
        {"r13d", UC_X86_REG_R13D},
        {"r14d", UC_X86_REG_R14D},
        {"r15d", UC_X86_REG_R15D},
        {"r8w", UC_X86_REG_R8W},
        {"r9w", UC_X86_REG_R9W},
        {"r10w", UC_X86_REG_R10W},
        {"r11w", UC_X86_REG_R11W},
        {"r12w", UC_X86_REG_R12W},
        {"r13w", UC_X86_REG_R13W},
        {"r14w", UC_X86_REG_R14W},
        {"r15w", UC_X86_REG_R15W},
        {"r8b", UC_X86_REG_R8B},
        {"r9b", UC_X86_REG_R9B},
        {"r10b", UC_X86_REG_R10B},
        {"r11b", UC_X86_REG_R11B},
        {"r12b", UC_X86_REG_R12B},
        {"r13b", UC_X86_REG_R13B},
        {"r14b", UC_X86_REG_R14B},
        {"r15b", UC_X86_REG_R15B},
        {"ds", UC_X86_REG_DS},
        {"es", UC_X86_REG_ES},
        {"fs", UC_X86_REG_FS},
        {"gs", UC_X86_REG_GS},
        {"ss", UC_X86_REG_SS},
        {"eflags", UC_X86_REG_EFLAGS},
        {"fs_base", UC_X86_REG_FS_BASE},
        {"gs_base", UC_X86_REG_GS_BASE},
        {"flags", UC_X86_REG_FLAGS},
        {"idtr", UC_X86_REG_IDTR},
        {"ldtr", UC_X86_REG_LDTR},
        {"tr", UC_X86_REG_TR},
        {"mm0", UC_X86_REG_MM0},
        {"mm1", UC_X86_REG_MM1},
        {"mm2", UC_X86_REG_MM2},
        {"mm3", UC_X86_REG_MM3},
        {"mm4", UC_X86_REG_MM4},
        {"mm5", UC_X86_REG_MM5},
        {"mm6", UC_X86_REG_MM6},
        {"mm7", UC_X86_REG_MM7},
        {"xmm0", UC_X86_REG_XMM0},
        {"xmm1", UC_X86_REG_XMM1},
        {"xmm2", UC_X86_REG_XMM2},
        {"xmm3", UC_X86_REG_XMM3},
        {"xmm4", UC_X86_REG_XMM4},
        {"xmm5", UC_X86_REG_XMM5},
        {"xmm6", UC_X86_REG_XMM6},
        {"xmm7", UC_X86_REG_XMM7},
        {"ymm0", UC_X86_REG_YMM0},
        {"ymm1", UC_X86_REG_YMM1},
        {"ymm2", UC_X86_REG_YMM2},
        {"ymm3", UC_X86_REG_YMM3},
        {"ymm4", UC_X86_REG_YMM4},
        {"ymm5", UC_X86_REG_YMM5},
        {"ymm6", UC_X86_REG_YMM6},
        {"ymm7", UC_X86_REG_YMM7},
        {"zmm0", UC_X86_REG_ZMM0},
        {"zmm1", UC_X86_REG_ZMM1},
        {"zmm2", UC_X86_REG_ZMM2},
        {"zmm3", UC_X86_REG_ZMM3},
        {"zmm4", UC_X86_REG_ZMM4},
        {"zmm5", UC_X86_REG_ZMM5},
        {"zmm6", UC_X86_REG_ZMM6},
        {"zmm7", UC_X86_REG_ZMM7},
        {"cr0", UC_X86_REG_CR0},
        {"cr1", UC_X86_REG_CR1},
        {"cr2", UC_X86_REG_CR2},
        {"cr3", UC_X86_REG_CR3},
        {"cr4", UC_X86_REG_CR4},
        {"cr8", UC_X86_REG_CR8},
        {"dr0", UC_X86_REG_DR0},
        {"dr1", UC_X86_REG_DR1},
        {"dr2", UC_X86_REG_DR2},
        {"dr3", UC_X86_REG_DR3},
        {"dr4", UC_X86_REG_DR4},
        {"dr5", UC_X86_REG_DR5},
        {"dr6", UC_X86_REG_DR6},
        {"dr7", UC_X86_REG_DR7},
        {"dil", UC_X86_REG_DIL},
        {"edi", UC_X86_REG_EDI},
        {"sil", UC_X86_REG_SIL},
        {"esi", UC_X86_REG_ESI},
        {"bpl", UC_X86_REG_BPL},
        {"ebp", UC_X86_REG_EBP},
        {"spl", UC_X86_REG_SPL},
        {"esp", UC_X86_REG_ESP},
        {"r8d", UC_X86_REG_R8D},
        {"r8", UC_X86_REG_R8},
        {"r9d", UC_X86_REG_R9D},
        {"r9", UC_X86_REG_R9},
        {"r10d", UC_X86_REG_R10D},
        {"r10", UC_X86_REG_R10},
        {"r11d", UC_X86_REG_R11D},
        {"r11", UC_X86_REG_R11},
        {"r12d", UC_X86_REG_R12D},
        {"r12", UC_X86_REG_R12},
        {"r13d", UC_X86_REG_R13D},
        {"r13", UC_X86_REG_R13},
        {"r14d", UC_X86_REG_R14D},
        {"r14", UC_X86_REG_R14},
        {"r15d", UC_X86_REG_R15D},
        {"r15", UC_X86_REG_R15},
        {"eflags", UC_X86_REG_EFLAGS},
        {"flags", UC_X86_REG_FLAGS},
    };
    return regMap[name];
}

void showRegs(){
    int rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rip,
        ah, al, ax, bh, bl, bx, ch, cl, cx, dh, dl, dx, si, di, bp, sp, r8d, r9d, r10d, r11d, r12d,
        r13d, r14d, r15d, r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w, r8b, r9b, r10b, r11b, r12b, r13b, r14b,
        r15b, ds, es, fs, gs, ss, eflags, fs_base, gs_base, flags, idtr, ldtr, tr, mm0, mm1, mm2, mm3, mm4, mm5, mm6,
        mm7, xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7, zmm0,
        zmm1, zmm2, zmm3, zmm4,zmm5, zmm6, zmm7, cr0, cr1, cr2, cr3, cr4, cr8,
        dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7, dil, edi, sil, esi, bpl, ebp, spl, esp, cs;

    uc_reg_read(uc, UC_X86_REG_RAX, &rax);
    uc_reg_read(uc, UC_X86_REG_RBX, &rbx);
    uc_reg_read(uc, UC_X86_REG_RCX, &rcx);
    uc_reg_read(uc, UC_X86_REG_RDX, &rdx);
    uc_reg_read(uc, UC_X86_REG_RSI, &rsi);
    uc_reg_read(uc, UC_X86_REG_RDI, &rdi);
    uc_reg_read(uc, UC_X86_REG_RBP, &rbp);
    uc_reg_read(uc, UC_X86_REG_RSP, &rsp);
    uc_reg_read(uc, UC_X86_REG_R8, &r8);
    uc_reg_read(uc, UC_X86_REG_R9, &r9);
    uc_reg_read(uc, UC_X86_REG_R10, &r10);
    uc_reg_read(uc, UC_X86_REG_R11, &r11);
    uc_reg_read(uc, UC_X86_REG_R12, &r12);
    uc_reg_read(uc, UC_X86_REG_R13, &r13);
    uc_reg_read(uc, UC_X86_REG_R14, &r14);
    uc_reg_read(uc, UC_X86_REG_R15, &r15);
    uc_reg_read(uc, UC_X86_REG_RIP, &rip);
    uc_reg_read(uc, UC_X86_REG_EFLAGS, &eflags);
    uc_reg_read(uc, UC_X86_REG_CS, &cs);
    uc_reg_read(uc, UC_X86_REG_SS, &ss);
    uc_reg_read(uc, UC_X86_REG_ES, &fs);
    uc_reg_read(uc, UC_X86_REG_FS, &ds);
    uc_reg_read(uc, UC_X86_REG_GS, &ds);
    uc_reg_read(uc, UC_X86_REG_FS_BASE, &fs_base);
    uc_reg_read(uc, UC_X86_REG_GS_BASE, &gs_base);

    printf("RAX = 0x%x\t\t", rax);
    printf("RBX = 0x%x\n", rbx);
    printf("RCX = 0x%x\t\t", rcx);
    printf("RDX = 0x%x\n", rdx);
    printf("RSI = 0x%x\t\t", rsi);
    printf("RDI = 0x%x\n", rdi);
    printf("RBP = 0x%x\t\t", rbp);
    printf("RSP = 0x%x\n", rsp);
    printf("R8 = 0x%x\t\t", r8);
    printf("R9 = 0x%x\n", r9);
    printf("R10 = 0x%x\t\t", r10);
    printf("R11 = 0x%x\n", r11);
    printf("R12 = 0x%x\t\t", r12);
    printf("R13 = 0x%x\n", r13);
    printf("R14 = 0x%x\t\t", r14);
    printf("R15 = 0x%x\n", r15);
    printf("RIP = 0x%x\t\t", rip);
    printf("EFLAGS = 0x%x\n", eflags);
    printf("CS = 0x%x\t\t", cs);
    printf("SS = 0x%x\n", ss);
    printf("DS = 0x%x\t\t", ds);
    printf("FS = 0x%x\n", fs);
    printf("GS = 0x%x\t\t", ds);
    printf("FS_BASE = 0x%x\n", fs_base);
    printf("GS_BASE = 0x%x\n", gs_base);
}

uint64_t getRegister(std::string name){
    int reg = regNameToConstant(name);
    uint64_t value;
    uc_reg_read(uc, reg, &value);
    return value;
}

int runCode(const std::string& code_in, int instructionCount)
{
    uc_err err;
    uint8_t codeBuf[CODE_BUF_SIZE];
    uint8_t zeroBuf[STACK_SIZE];

    uint8_t* code;
    code = (uint8_t*)(code_in.c_str());

    memcpy(codeBuf, code, code_in.length());
    printf("Emulate i386 code\n");

    err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);
    if (err) {
        printf("Failed on uc_open() with error returned: %u\n", err);
        return -1;
    }

    memset(zeroBuf, 0, STACK_SIZE);
    uc_mem_map(uc, STACK_ADDRESS, STACK_SIZE, UC_PROT_READ | UC_PROT_WRITE);
    if (uc_mem_write(uc, STACK_ADDRESS, zeroBuf, STACK_SIZE)) {
        printf("Failed to write stack to memory, quit!\n");
        return -1;
    }

    uc_mem_map(uc, ENTRY_POINT_ADDRESS, MEMORY_ALLOCATION_SIZE, UC_PROT_READ | UC_PROT_WRITE | UC_PROT_EXEC);
    if (uc_mem_write(uc, ENTRY_POINT_ADDRESS, codeBuf, sizeof(codeBuf) - 1)) {
        printf("Failed to write emulation code to memory, quit!\n");
        return -1;
    }
    uint64_t stackBase = STACK_ADDRESS + STACK_SIZE;
    if (uc_reg_write(uc, UC_X86_REG_RSP, &stackBase)){
        printf("Failed to write stack pointer to memory, quit!\n");
        return -1;
    }

    err = uc_emu_start(uc, ENTRY_POINT_ADDRESS, ENTRY_POINT_ADDRESS + sizeof(codeBuf), 0, instructionCount);
    if (err) {
        printf("Failed on uc_emu_start() with error returned %u: %s\n",
               err, uc_strerror(err));
    }

    showRegs();
    uc_close(uc);
    return 0;
}