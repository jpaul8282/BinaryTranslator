#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>

#include "file_utils.h"

// ===============================================
// Defines   ||
//           \/

#define LOG(...) fprintf (LOG_FILE, __VA_ARGS__);

// ===============================================


// Constants ||
//           \/

const int MAX_IP = 1000;

const int MAX_LABELS = 100;

const int MAX_OPCODE_LEN = 6;

const char* INPUT_FILE_PATH = "../../Processor/data/cmds.bin";

const int HEADER_OFFSET = 8;

const int PAGESIZE = 4096;

const char* NotFound = "Not found\n";

// ===============================================


// ===============================================
// Enums     ||
//           \/

#define DEF_CMD(CMD, id) CMD = id, 

enum EnumCommands
{
    HLT = 0,

    #include "../codegen/cmds.h"
};

#undef DEF_CMD


enum BitMasks
{
    CMD_BITMASK  = 0b00011111,
    SPEC_BITMASK = 0b11100000,
    ARG_IMMED    = 0b00100000,
    ARG_REG      = 0b01000000,
    ARG_MEM      = 0b10000000,
};


enum OffsetsAndFlags
{
    ZERO_OFFSET = 0,
    MULTI_BYTE_OFFSET = 9,
    BYTE_OFFSET = 1,
    JMP_OFFSET = 6,

    END_OF_PROG = -1,
};


enum ExitCodes
{
    SUCCESS,
    ALLOCATION_FAILURE,
};


enum PushPopVariations
{
    VOID = 0,
    IMM = 1,
    REG = 3,
    RAM = 5,

    IMM_REG = 4,
    IMM_RAM = 6,
    REG_RAM = 8,
    IMM_REG_RAM = 9,
};


enum PushPopSizes
{
    POP_REG_SIZE = 1, 
};


enum Registers
{
    RAX = 0,
    RBX,
    RCX,
    RDX,
};

// ===============================================


// ===============================================
// Structs   ||
//           \/

typedef double elem_t;

struct InstructionSizes
{
    EnumCommands id;
    
    int original_size;
    int x86_size;
};

const struct InstructionSizes InstrSizes[30] =
{
    {HLT,  1,   1},
    {PUSH, 10,  1}, 
    {},
    {},
    {},
    {},
    {POP,  10,  1},
    {},
    {},
    {JMP, 10,  5},
    {JG,  10,  11},
    {JGE, 10,  11},
    {JA,  10,  11},
    {JAE, 10,  11},
    {JE,  10,  11},
    {JNE, 10,  11},
};


struct CmdsBuffer
{
    char* content;
    size_t len;
};


struct Command 
{
    EnumCommands name;
    int orig_ip;
    int x86_ip;
    
    int reg_index;
    elem_t value;

    bool is_immed;
    bool use_reg;
    bool use_mem;
};




struct TranslatorMain
{
    CmdsBuffer src_cmds;
    CmdsBuffer dst_x86;

    Command** cmds_array;
    int cmds_counter;

    int orig_ip_counter;
    int x86_ip_counter;
};




// ===============================================

// ===============================================
// Functions ||
//           \/

void FillJumpLables (TranslatorMain* self);

int FindJumpIp (TranslatorMain* self, int orig_ip);

char* GetNameFromId (EnumCommands id);

void DumpRawCmds (TranslatorMain* self);

void Dump86Buffer (TranslatorMain* self);

int FillCmdInfo (const char* code, TranslatorMain* self);

void TranslatorCtor (TranslatorMain* self);

void ParseOnStructs (TranslatorMain* self);

void ReadFileToStruct (TranslatorMain* self, FILE* file);

int CmdToStruct (const char* code, TranslatorMain* self);

int AllocateCmdArrays (TranslatorMain* self);

void RunCode (TranslatorMain* self);

void StartTranslation (TranslatorMain* self);

bool IsJump (int cmd);

//-- Translation Units:

int CalcVariationSum (Command* cur_cmd);

void LoadToX86Buffer (TranslatorMain* self, char* op_code, size_t len);

void HandlePushPopVariation (TranslatorMain* self, Command* cur_cmd);

void TranslatePop (TranslatorMain* self, Command* cur_cmd);

void TranslatePopReg (TranslatorMain* self, Command* cur_cmd);

void TranslateJmp (TranslatorMain* self, Command* cur_cmd);

void TranslateConditionJmp (TranslatorMain* self, Command* jmp_cmd);

// ===============================================

#endif