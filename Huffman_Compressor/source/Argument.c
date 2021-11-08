#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "..\header\Argument.h"

bool ValidComand(const char* comand, const char** ComSuport)
{
    for(uint8_t Count = 0; Count < SIZECOMAND; Count++)
    {
        if(!strcmp(comand, ComSuport[Count]))
            return true;
    }
    return false;
}

bool ValidPath(const char* path)
{
    if(!path)
        return false;
    
    if((strlen(path) + 1) > _MAX_PATH)
        return false;
    
    return true;
}

void Help()
{
    printf("\n\t\t\t\t   COMPACTOR\n\n"
           "-c --compact [input file] [output file]      Compact the input file in the output file\n"
           "-u --unzip   [input file] [output file]      Unzips an .hf file to an output file\n\n"
           "[example]\n"
           "CompactHuff.exe -c \"file1.txt\" \"file2.hf\"\n"
           "CompactHuff.exe --compact \"file1.txt\" \"file2.hf\"\n"
           "CompactHuff.exe -u \"file1.hf\" \"file2.txt\"\n"
           "CompactHuff.exe --unzip \"file1.hf\" \"file2.txt\"\n");
}

bool Tocompare(const char* str1, const char* str2)
{
    if(!str1 || !str2)
        return false;

    return !strcmp(str1, str2);
}