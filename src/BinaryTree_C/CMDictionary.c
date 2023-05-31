/*
    Clay Molitor
    CS 145
    Lab 06: Binary search tree

    My program manages a binary search tree. 
    Each node in the tree has a string firstName, lastName, address, city, state, email, and zip.
    Each node also has pointers to up to two more nodes.

    As a binary search tree, child nodes with a lesser key are stored on "lesser" and nodes with a bigger key on "greater".
    There is no balancing per assignment instructions and printing the tree uses 

*/
#include <stdio.h>
#include <inttypes.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "CMDictionary.h"


// Returns the dictionary with the key of key.
// Returns NULL if not dictionary with such key exists in base.
Dictionary* searchTree(Dictionary* base, int key)
{
    if(base == NULL) {
        return NULL;
    }
    else if(base->key ==key)   {
        return base;
    }
    else if(base->key < key) { // current node less than target.
        return searchTree(base->greater, key);
    }
    else if(base->key > key) { // current node greater than target.
        return searchTree(base->lesser, key);
    }
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
    const char** label = DICT_FIELDS; // Null terminated array of strings 
    // For every string field in dictionary 
    for(int i = 0; label[i] != NULL; i++) 
    {
        // print the label and the corrsponding dictionary field
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

    const char** label = DICT_FIELDS;
    // Get all string
    for(int i = 0; label[i] != NULL; i++) 
    {
        char* str = &data->DICT_BEGIN + i;
        //*str = calloc(1, STR_LENG);
        printf("Enter the %s: ", label[i]);
        scanf("%sSTR_LENG", str);
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
// Base must be passed as reference
void deleteDictionary(Dictionary** base, Dictionary* data)
{
    // Base must be passed as reference, so to avoid messy pointer syntax temp is used.
    // All assignments must be made to base.
    Dictionary* temp = *base;

    if(*base == NULL || data == NULL)
    {
        // Break if either node is null.
    }
    else if(temp->key == data->key)
    {
        if(temp->lesser != NULL && temp->greater != NULL) // Two children
        {
            // Move right node under left node.
            temp->lesser->greater = temp->greater;
            *base = &((*base)->lesser); // left up, replacing base;
        }
        else if(temp->lesser != NULL) { // If only left child
            *base = &(temp->lesser);
        }
        else if(temp->greater != NULL) { // If only right child
            *base = &(temp->greater);
        }
        else { // Leaf
            *base = NULL;
        }
        
        // This program leaks memory whenever a node is deleted.
        // TODO fix this, getting "Unknown signal error"
        // free(*base);
    }
    else // If target is deeper, then recurse.
    {
        if(data->key > temp->key) {
            deleteDictionary(&(temp->greater), data);
        }
        if(data->key < temp->key) {
            deleteDictionary(&(temp->lesser), data);
        }
    }
}
void userInterface()
{

    Dictionary defaultNodes[] = // For demonstrating code
    {
        {"Joe", "Miller", "1263 18th ave", "Seattle", "WA", "joe@mail.biz", "98121", 1001, NULL, NULL}, 
        {"Bob", "Miller", "Address", "City", "State", "Email", "Zip", 1002, NULL, NULL},
        {"Jim", "Fisher", "Address", "City", "State", "Email", "Zip", 1003, NULL, NULL},
        {"Kim", "Baker", "Address", "City", "State", "Email", "Zip", 1004, NULL, NULL},
        NULL,
    }; 

    // While user input != q
    char input[STR_LENG] = "";
    do
    {

        printf(
            "Please select an option: \\
            \n\tAdd a node (a)\\
            \n\tPrint all nodes (p)\\
            \n\tSearch for a specific node (s)\\
            \n\tDelete a node (d)\\
            \n\tLoad pre-populated nodes (l)\\
            \n\tQuit the program (q)\n"
        );

        // Get input
        scanf("%1s", input);
        char first = tolower(input[0]);

        switch (first)
        {
        case 'a': // Add a node
            addNode(&baseNode, createDictionary(true, baseNode));
            break;
        case 'p': // print whole tree
            printTree(baseNode);
            break;
        case 's': // search
            int k;
            printf("Enter the key of the node you would like to remove: ");
            scanf("%i", k);
            printDictionary(searchTree(baseNode, k));

            break;
        case 'd': // Delete
            int k;
            printf("Enter the key of the node you would like to remove: ");
            scanf("%i", k);
            deleteDictionary(&baseNode, searchTree(baseNode, k));

            break;
        case 'l':
            addNode(&baseNode, &defaultNodes[0]);
            addNode(&baseNode, &defaultNodes[1]);
            addNode(&baseNode, &defaultNodes[2]);
            addNode(&baseNode, &defaultNodes[3]);
            break;
        case 'q': // silently break
            break;
    
        default:
            printf("Invalid input, try again\n");
            break;
        }
    }while(input[0] != 'q');

}


int main()
{

    userInterface();
    //addNode(&baseNode, createDictionary(true, baseNode));


}