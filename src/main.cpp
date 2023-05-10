#include "../include/translation.h"
#include "../include/parser.h"
#include "../include/optimizer.h"
#include "../include/write_in_elf.h"



int main()
{
    TranslatorInfo TranslatorInfo = {};
    TranslatorCtor (&TranslatorInfo);

    ParseOnStructs (&TranslatorInfo);

    DumpRawCmds (&TranslatorInfo);
    OptimizeCmdFlow (&TranslatorInfo);

    FillCmdIp (&TranslatorInfo);
    FillJumpLables (&TranslatorInfo);
    DumpRawCmds (&TranslatorInfo);

    StartTranslation (&TranslatorInfo);

    Dump86Buffer (&TranslatorInfo);

    // WriteInelf (&TranslatorInfo);

    // RunCode (&TranslatorInfo);
}


void RunCode (TranslatorInfo* self)
{
    int flag = mprotect (self->dst_x86.content, self->dst_x86.len + 1, PROT_EXEC);
    
    printf ("Address: %p\n", RunCode);

    if (flag == -1)
    {
        return;
    }

    void (* god_save_me)(void) = (void (*)(void))(self->dst_x86.content);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    god_save_me();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    printf ("Elapsed time(mcr. s): %lu\n", std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}