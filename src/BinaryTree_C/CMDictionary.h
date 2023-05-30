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
    char* firstName;
    char* lastName;
    char* address;
    char* city;
    char* state;
    char* email;
    char* zip;

    // Unique key for sorting nodes
    int key;

    // Binary search tree nodes 
    Dictionary* greater;
    Dictionary* lesser;
} *baseNode, // baseNode is the root of our dictionary
defaultNodes[] = // For demonstrating code
{
    {"Joe", "Miller", "Address", "City", "State", "Email", "Zip", 1001, NULL, NULL}, 
    {"Bob", "Miller", "Address", "City", "State", "Email", "Zip", 1002, NULL, NULL},
    {"Jim", "Fisher", "Address", "City", "State", "Email", "Zip", 1003, NULL, NULL},
    {"Kim", "Baker", "Address", "City", "State", "Email", "Zip", 1004, NULL, NULL},

}; 

















#endif