#ifndef HUFFMAN_DEFINITIONS_H
#define HUFFMAN_DEFINITIONS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define TABLE_SIZE_ASCII 256
#define SIZE_BITS_OF_BYTE 8

// Knot for Huffman Tree
typedef struct TRIE
{
    uint8_t symbol;// simbolo ou byte
    uint32_t frequency;// frequência de symbols
    struct TRIE *pLeft;
    struct TRIE *pRight;
} No_trie_t;

// Knot for Huffman Tree User
typedef struct NO_TRIE_LIST
{
    No_trie_t *ptrie_member;
    struct NO_TRIE_LIST *pNext_trie;
} No_trie_list_t;

// Type that counts the elements of a list and pointers to the beginning and end of the list
typedef struct PRIORITY_LIST
{
    No_trie_list_t* pBegin_List;
    No_trie_list_t* pEnd_List;
    uint32_t Elements;
}Priority_List_t;

// Enumeration for error codes
typedef enum ERRC_CODE
{
    HUFF_ERRC_SUCESS,
    HUFF_ERRC_FALIED,
    HUFF_ERRC_INVALID_PARAMETER,
    HUFF_ERRC_OPEN_FILE_INPUT,
    HUFF_ERRC_OPEN_FILE_OUTPUT,
    HUFF_ERRC_CLOSED_FILE,
    HUFF_ERRC_CREATE_TREE,
    HUFF_ERRC_INVALID_TYPEFILE,
    HUFF_ERRC_WITHOUT_MEMORY
}Huff_Errc_t;


// File compression function
Huff_Errc_t compact(const char*, const char*);

// Function to create the Huffman tree
No_trie_t* createTreeHuff(uint32_t*);

// Function that inserts and organizes us in a list according to the frequency of the bytes of the tries
bool sortList(Priority_List_t*, No_trie_list_t*);

// Function that creates one in the NO_TRIE_T type
No_trie_list_t* newNoList(No_trie_t*);

// Function that creates one on a trie
No_trie_t* newNotrie(int8_t, uint32_t, No_trie_t*, No_trie_t*);

// Function that removes the first list element in NO_TRIE_LIST_T
No_trie_t* removeFirst(Priority_List_t*);

// Function that takes the bit code to the hollow on Huffman's tree
bool codHuff(No_trie_t*, uint8_t, uint8_t*, uint32_t);

// File decompression function
Huff_Errc_t unzip(const char*, const char*);

//Function that releases all the memory allocated in Huffman's tree
void releaseTreeHuff(No_trie_t*);

// Function that read compares if bit more sketching a byte is 1 or 0
bool readBoolean(uint8_t*);

// Function that returns the difference between the current and end position of the file pointer
uint64_t currToEnd(FILE*);

// Function that prints the error message
void mensageError(Huff_Errc_t);

#endif
