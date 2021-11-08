#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <stdbool.h>
#include "..\header\Huffman_definitions.h"

No_trie_t* newNotrie(int8_t symbol, uint32_t freq, No_trie_t* left, No_trie_t* right)
{
    No_trie_t* New_No = (No_trie_t*)malloc(sizeof(No_trie_t));
    if (!New_No)
    {
        fprintf(stderr, "Without memory to allocate a new NO\n");
        exit(EXIT_FAILURE);
    }
    
    New_No->frequency = freq;
    New_No->symbol    = symbol;
    New_No->pLeft     = left;
    New_No->pRight    = right;

    return New_No;
}

No_trie_list_t* newNoList(No_trie_t* No_Member)
{
    No_trie_list_t* New_Member_list = (No_trie_list_t*)malloc(sizeof(No_trie_list_t));
    if (!New_Member_list)
    {
        fprintf(stderr, "Without memory to allocate a new member of the list\n");
        exit(EXIT_FAILURE);
    }
    
    New_Member_list->ptrie_member = No_Member;
    New_Member_list->pNext_trie   = NULL;

    return New_Member_list;
}

bool sortList(Priority_List_t* List, No_trie_list_t* Member_List)
{
    if (!Member_List)
        return false;
    
    if (!List->pBegin_List && !List->pEnd_List)
    {
        List->pBegin_List = Member_List;
        List->pEnd_List   = Member_List;
    }
    else if (Member_List->ptrie_member->frequency <= List->pBegin_List->ptrie_member->frequency)
    {
        Member_List->pNext_trie = List->pBegin_List;
        List->pBegin_List       = Member_List;
    }
    else if (Member_List->ptrie_member->frequency >= List->pEnd_List->ptrie_member->frequency)
    {
        Member_List->pNext_trie     = NULL;
        List->pEnd_List->pNext_trie = Member_List;
        List->pEnd_List             = Member_List;
    }
    else
    {
        No_trie_list_t* Holder = List->pBegin_List;

        while (Holder->pNext_trie->ptrie_member->frequency <= Member_List->ptrie_member->frequency)
        {
            Holder = Holder->pNext_trie;
        }
        
        Member_List->pNext_trie = Holder->pNext_trie;
        Holder->pNext_trie      = Member_List;
    }

    List->Elements++;
    return true;
}

No_trie_t* removeFirst(Priority_List_t* list)
{
    No_trie_t* toRemove  = list->pBegin_List->ptrie_member;
    No_trie_list_t* temp = list->pBegin_List;

    list->pBegin_List = list->pBegin_List->pNext_trie;
    if (!list->pBegin_List)
        list->pEnd_List = NULL;

    free(temp);
    temp = NULL;
    list->Elements--;

    return toRemove;
}

No_trie_t* createTreeHuff(uint32_t* ListFrequenc)
{
    Priority_List_t List_Prt = {.pBegin_List = NULL,
                                .pEnd_List = NULL,
                                .Elements = 0U      };
    bool verific = 0;

    for(uint16_t Count = 0U; Count < TABLE_SIZE_ASCII; Count++)
    {
        if (ListFrequenc[Count] > 0)
        {
            verific = sortList
            (
                &List_Prt,
                newNoList(
                    newNotrie((uint8_t)Count, ListFrequenc[Count], 0, 0)
                )
            );

            if(!verific)
                return NULL;
        }
    }

    No_trie_t  *left, *right;

    while(List_Prt.Elements > 1){
        left  = removeFirst(&List_Prt);
        right = removeFirst(&List_Prt);
        
        verific = sortList
            (
                &List_Prt, 
                newNoList(
                    newNotrie(0, left->frequency + right->frequency, left, right)
                )
            );
        if (!verific)
            return NULL;
    }

    return removeFirst(&List_Prt);
}

bool codHuff(No_trie_t* Tree, uint8_t symb, uint8_t* path, uint32_t Curr)
{

    if (Tree->symbol == symb && !Tree->pLeft && !Tree->pRight)
    {
        path[Curr] = '\0';
        return true;
    }

    bool condition = false;

    if (Tree->pLeft)
    {
        path[Curr] = '0';
        condition  = codHuff(Tree->pLeft, symb, path, Curr + 1);
        
        if(condition)
            return true;
    }
    if (Tree->pRight)
    {
        path[Curr] = '1';
        condition  = codHuff(Tree->pRight, symb, path, Curr + 1);

        if(condition)
            return true;
    }

    return condition;
}

bool readBoolean(uint8_t* byte)
{
    uint8_t mask = 1 << 7;

    return (mask & *byte);
}

void releaseTreeHuff(No_trie_t* Tree)
{
    if(!Tree)
        return;
    if(Tree->pLeft)
        releaseTreeHuff(Tree->pLeft);
    if(Tree->pRight)
        releaseTreeHuff(Tree->pRight);
    
    free(Tree);
}

uint64_t currToEnd(FILE* file)
{
    uint64_t size_cur = (uint64_t)ftell(file);
    fseek(file, 0L, SEEK_END);
    
    uint64_t size_end = (uint64_t)ftell(file);
    fseek(file, size_cur, SEEK_SET);

    return size_end - size_cur;
}

Huff_Errc_t compact(const char* InputFile, const char* OutputFile)
{
    FILE* In_file = fopen(InputFile, "rb");
    if(!In_file)
        return HUFF_ERRC_OPEN_FILE_INPUT;
    
    FILE* Out_file = fopen(OutputFile, "wb");
    if(!Out_file){
        fclose(In_file);    
        return HUFF_ERRC_OPEN_FILE_OUTPUT;
    }

    uint64_t SizeFile = currToEnd(In_file);
    uint32_t ListFrequency[TABLE_SIZE_ASCII] = {0};
    uint16_t typefile = 0x4648;

    uint8_t* BuffFileIn = (uint8_t*)malloc(SizeFile);
    if(!BuffFileIn)
        return HUFF_ERRC_WITHOUT_MEMORY;
    
    rewind(In_file);
    fread(BuffFileIn, SizeFile, 1, In_file);
    
    if((fclose(In_file)) != 0)
        return HUFF_ERRC_CLOSED_FILE;
    
    for
    (
        uint8_t* Count = BuffFileIn;
        Count != BuffFileIn + SizeFile;
        Count++
    )
    {
        ListFrequency[*Count]++;
    }

    fwrite(&typefile, sizeof(typefile), 1, Out_file);
    fwrite(ListFrequency, TABLE_SIZE_ASCII * sizeof(uint32_t), 1, Out_file);
    fwrite(&SizeFile, sizeof(SizeFile), 1, Out_file);

    No_trie_t* source = createTreeHuff(ListFrequency);
    if(!source){
        fclose(In_file);
        fclose(Out_file);
        return HUFF_ERRC_CREATE_TREE;
    }

    uint8_t pathCaract[257] = {0},
            NumberBits      = 0U,
            byte            = 0U,
           *pTrack          = BuffFileIn;

    while (pTrack != BuffFileIn + SizeFile)
    {   
        bool test = codHuff(source, *pTrack, pathCaract, 0U);
        if (!test)
            return HUFF_ERRC_FALIED;
            
        uint8_t* pPath = pathCaract;

        while(*pPath)
        {
            if (*pPath == '1')
            {
                byte <<= 1;

                byte = byte | 1;
            }

            if (*pPath == '0')
            {
                byte <<= 1;
            }

            NumberBits++;

            if (!(NumberBits % 8))
            {
                NumberBits = 0;
                fwrite(&byte, sizeof(byte), 1, Out_file);
                byte = 0U;
            }

            pPath++;
        }
    pTrack++;
    }

    if (NumberBits % 8)
    {
        NumberBits = 8 - (NumberBits % 8);
        for(uint8_t rest = 0; rest < NumberBits; rest++)
        {
            byte <<= 1;
        }
        fwrite(&byte, sizeof(byte), 1, Out_file);
    }

    releaseTreeHuff(source);
    free(BuffFileIn);

    if((fclose(Out_file)) != 0)
        return HUFF_ERRC_CLOSED_FILE;

    return HUFF_ERRC_SUCESS;
}

Huff_Errc_t unzip(const char* InputFile, const char* OutputFile)
{
    FILE* In_file = fopen(InputFile, "rb");
    if(!In_file)
        return HUFF_ERRC_OPEN_FILE_INPUT;
    
    FILE* Out_file = fopen(OutputFile, "wb");
    if(!Out_file){
        fclose(In_file);
        return HUFF_ERRC_OPEN_FILE_OUTPUT;
    }
    uint16_t typeFile = 0U;
    fread(&typeFile, sizeof(typeFile), 1, In_file);

    if(typeFile != 0x4648){
        fclose(In_file);
        fclose(Out_file);
        return HUFF_ERRC_INVALID_TYPEFILE;
    }

    uint32_t table_frequency[TABLE_SIZE_ASCII] = {0};

    fread(table_frequency, sizeof(uint32_t) * TABLE_SIZE_ASCII, 1, In_file);

    uint64_t Size_FileOri = 0U, Size_Currfile = 0U;

    fread(&Size_FileOri, sizeof(uint64_t), 1, In_file);
    Size_Currfile = currToEnd(In_file);

    uint8_t *BuffIn = (uint8_t*)malloc(Size_Currfile),
            *track  = BuffIn;

    if(!BuffIn){
        fclose(In_file);
        fclose(Out_file);
        return HUFF_ERRC_WITHOUT_MEMORY;
    }

    fread(BuffIn, Size_Currfile, 1, In_file);
    
    if((fclose(In_file)) != 0)
        return HUFF_ERRC_CLOSED_FILE;

    No_trie_t *source = createTreeHuff(table_frequency),
              *hold   = source;
    
    if(!source){
        fclose(In_file);
        fclose(Out_file);
        return HUFF_ERRC_CREATE_TREE;
    }

    uint8_t  CountBits, byte = 0;
    uint64_t CountElement    = 0;

    while (CountElement < Size_FileOri)
    {
        byte = *track;
        for( CountBits = 0; CountBits < SIZE_BITS_OF_BYTE; CountBits++)
        {
            if(CountElement == Size_FileOri)
                break;

            if(readBoolean(&byte)){
                hold = hold->pRight;
            }else{
                hold = hold->pLeft;
            }

            byte <<= 1;

            if(!hold->pRight && !hold->pLeft)
            {
                fwrite(&(hold->symbol), sizeof(uint8_t), 1, Out_file);
                CountElement++;
                hold = source;
            }
        }
        track++;
    }

    releaseTreeHuff(source);
    free(BuffIn);

    if((fclose(Out_file)) != 0)
        return HUFF_ERRC_CLOSED_FILE;

    return HUFF_ERRC_SUCESS;
}
void mensageError(Huff_Errc_t Errc)
{
    switch (Errc)
    {
    case HUFF_ERRC_FALIED:
        printf("\n[ERROR] The program failed for some reason\n");
        break;
    case HUFF_ERRC_INVALID_PARAMETER:
        printf("\n[ERROR] The program failed, invalid parameters\n");
        break;
    case HUFF_ERRC_OPEN_FILE_INPUT:
        printf("\n[ERROR] The program failed when trying to open the input file\n");
        break;
    case HUFF_ERRC_OPEN_FILE_OUTPUT:
        printf("\n[ERROR] The program failed to open the output file\n");
        break;
    case HUFF_ERRC_CLOSED_FILE:
        printf("\n[ERROR] The program failed when trying to close the file\n");
        break;
    case HUFF_ERRC_CREATE_TREE:
        printf("\n[ERROR] The program failed to create the Huffman tree\n");
        break;
    case HUFF_ERRC_INVALID_TYPEFILE:
        printf("\n[ERROR] The program failed. The .hf type file\n");
        break;
    case HUFF_ERRC_WITHOUT_MEMORY:
        printf("\n[ERROR] The program failed. Without memory\n");
        break;
    default:
        printf("\n[ERROR] The program failed. Unknown error\n");
        break;
    }
}