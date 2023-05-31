#ifndef DICTIONARY_H // include guard
#define DICTIONARY_H

#include <inttypes.h> 


// First and last strings in Dictionary, used for iterating.
#define DICT_BEGIN firstName
#define DICT_END zip
#define STR_LENG 64
typedef struct dictionary Dictionary;

// The human readable names of all the string fields in dictionary, in the order of the struct. 
const char* DICT_FIELDS[] = {"First Name", "Last Name", "Address", "City", "State", "Email", "Zip", NULL};
struct dictionary // Binary search tree node
{
    char firstName[STR_LENG];
    char lastName[STR_LENG];
    char address[STR_LENG];
    char city[STR_LENG];
    char state[STR_LENG];
    char email[STR_LENG];
    char zip[STR_LENG];

    // Unique key for sorting nodes
    int key;

    // Binary search tree nodes 
    Dictionary* greater;
    Dictionary* lesser;
} *baseNode; // baseNode is the root of our dictionary

















#endif