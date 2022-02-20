#include "include/global.h"
#include "include/assembler.h"
#include "include/cpu.h"


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Error with filename\n");
        return 0;
    }

    const char* asmfile_path   = argv[1];
    const char  binfile_path[] = "binary";

    assembler(asmfile_path, binfile_path);
    processor(binfile_path);
    return 0;
}