#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../header/Argument.h"
#include "../header/Huffman_definitions.h"

const char* ComandSuport[] = 
{
    "--help",
    "--compact",
    "-c",
    "--unzip",
    "-u"
};

int main(int argc, char* argv[]){
    if ( argc > 4 || argc < 2)
    {
        printf("\n[ERROR] Number of arguments is invalid\n");
        return 0;
    }
    else
    {
        if (ValidComand(argv[1], ComandSuport))
        {
            if (Tocompare(argv[1], ComandSuport[0]))
            {
                Help();
                return 0;
            }
            else if (Tocompare(argv[1], ComandSuport[1]) || Tocompare(argv[1], ComandSuport[2]))
            {
                if(!argv[2] || !argv[3])
                {
                    printf("\n[ERROR] Two valid paths are needed\n");
                    return 0;
                }

                if(!ValidPath(argv[2])){
                    printf("\n[ERROR] The path of the file \"%.256s\" is invalid\n", argv[2]);
                    return 0;
                }

                if(!ValidPath(argv[3])){
                    printf("\n[ERROR] The path of the file \"%.256s\" is invalid\n", argv[3]);
                    return 0;
                }

                if (!strstr(argv[3], ".hf"))
                {
                    strcat(argv[3], ".hf");
                }

                Huff_Errc_t Errc = compact(argv[2], argv[3]);
                if(Errc != HUFF_ERRC_SUCESS){
                    mensageError(Errc);
                    return 0;
                }

                printf("\nThe file \"%s\" was compacted successfully\n", argv[2]);
            }
            else if (Tocompare(argv[1], ComandSuport[3]) || Tocompare(argv[1], ComandSuport[4]))
            {
                if(!argv[2] || !argv[3])
                {
                    printf("\n[ERROR] Two valid paths are needed\n");
                    return 0;
                }

                if(!ValidPath(argv[2])){
                    printf("\n[ERROR] The path of the file \"%.256s\" is invalid\n", argv[2]);
                    return 0;
                }

                if(!ValidPath(argv[3])){
                    printf("\n[ERROR] The path of the file \"%.256s\" is invalid\n", argv[3]);
                    return 0;
                }

                if (!strstr(argv[2], ".hf"))
                {
                    printf("\n[ERROR] The \"%.256s\" file does not have the .hf extension\n", argv[2]);
                    return 0;
                }

                Huff_Errc_t Errc = unzip(argv[2], argv[3]);
                if(Errc != HUFF_ERRC_SUCESS){
                    mensageError(Errc);
                    return 0;
                }

                printf("\nThe file \"%s\" was uncompressed successfully\n", argv[2]);
            }
        }
        else
        {
            printf("\n[ERROR] The \"%.256s\" command is invalid\n", argv[1]);
            return 0;
        }
    }

    return 0;
}