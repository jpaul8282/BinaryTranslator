#include "../include/translator.h"

int main()
{
    TranslatorInfo TranslatorInfo = {};
    TranslatorCtor (&TranslatorInfo);

    ParseOnStructs (&TranslatorInfo);

    DumpRawCmds (&TranslatorInfo);

    FillJumpLables (&TranslatorInfo);
    DumpRawCmds (&TranslatorInfo);

    StartTranslation (&TranslatorInfo);

    Dump86Buffer (&TranslatorInfo);

    RunCode (&TranslatorInfo);
}


void RunCode (TranslatorInfo* self)
{
    int flag = mprotect (self->dst_x86.content, self->dst_x86.len + 1, PROT_EXEC);
        // flag = mprotect (self->dst_x86.content, self->dst_x86.len + 1, PROT_WRITE);
        // flag = mprotect (self->dst_x86.content, self->dst_x86.len + 1, PROT_READ);
    
    printf ("Address: %p\n", RunCode);

    if (flag == -1)
    {
        return;
    }

    void (* god_save_me)(void) = (void (*)(void))(self->dst_x86.content);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i = 0; i < 1000; i++)
    {
        god_save_me();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    printf ("Elapsed time(mcrs): %lu\n", std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    // printf ("Bruh: %d\n", kek);
}