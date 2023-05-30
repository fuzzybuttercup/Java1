#include <stdio.h>
#include <inttypes.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "CMDictionary.h"


// Returns the dictionary with the key of key.
// Returns NULL if not dictionary with such key exists in base.
Dictionary* searchTree(Dictionary* base, int key)
{
    return NULL;
}
// Adds a node to the dictionary
// baseNode should be passed as target
bool addNode(Dictionary** target, Dictionary* data)
{
    Dictionary* base = *target;
    if(base == NULL) {    
        *target = data; // Overwrites variable passed as target with data
    }
    else if(base->key == data->key) {
        return false; // Failed to add
    }
    else if(data->key > base->key) // Data is Greater
    {
        if(base->greater != NULL) {
            return addNode(&base->greater, data);
        }
        else { // target->greater is Null
            base->greater = data;
        }
    }
    else // Data is Less
    {
        if(base->lesser != NULL) {
            return addNode(&base->lesser, data);
        }
        else { // target->lesser is Null
            base->lesser = data;
        }
    }
    return true;
}
// Prints the values of the passed dictionary to console
void printDictionary(Dictionary* data)
{
    char** label = DICT_FIELDS; // Null terminated array of strings 
    // For every string field in dictionary 
    for(int i = 0; label[i] != NULL; i++) 
    {
        char** str = &data->DICT_BEGIN + i;
        printf("%s: %s\n", label[i], *(&data->DICT_BEGIN + i));
    }
    printf("Key: %i\n\n", data->key);
}
// Prints the passed dictionary and all sub-dictionaries
void printTree(Dictionary* base)
{
    if(base != NULL)
    {
        printDictionary(base);
        if(base->greater != NULL) {
            printTree(base->greater);
        } 
        if(base->lesser != NULL) {
            printTree(base->lesser);
        }
    }
}
// Prompts the use to enter the data to create a new dictionary.
// Returns a pointer to the dictionary, which must be deleted manually.
// If check Key is true then the entered key will be check against the baseNode tree.
// keyCheckTree may be NULL if checkKey is false
Dictionary* createDictionary(bool checkKey, Dictionary* keyCheckTree)
{
    Dictionary* data = calloc(1, sizeof(Dictionary));

    char** label = DICT_FIELDS;
    // Get all string
    for(int i = 0; label[i] != NULL; i++) 
    {
        char** str = &data->DICT_BEGIN + i;
        *str = calloc(1, STR_LENG);
        printf("Enter the %s: ", label[i]);
        scanf("%sSTR_LENG", *str);
    }

    // Get the key
    printf("Enter the Key: ");
    scanf("%i", &data->key);
    while(checkKey && searchTree(keyCheckTree, data->key) != NULL) // If a tree is found with the current key
    {
        printf("That Key value is already taken.\n");
        printf("Enter the Key: ");
        scanf("%i", &data->key);
    }

    data->greater = NULL;
    data->lesser = NULL;

    return data;
}
// Clears the passed dictionary from memory
void deleteDictionary(Dictionary* data)
{
    char** label = DICT_FIELDS;
    for(int i = 0; label[i] != NULL; i++) 
    {
        free(data->DICT_BEGIN + i); // Free all strings
    }
    free(data);
}




int main()
{

    addNode(&baseNode, &defaultNodes[0]);
    addNode(&baseNode, &defaultNodes[3]);
    addNode(&baseNode, &defaultNodes[2]);
    addNode(&baseNode, &defaultNodes[1]);
    Dictionary* foo = createDictionary(false, baseNode);

    addNode(&baseNode, foo);

    printTree(baseNode);
}