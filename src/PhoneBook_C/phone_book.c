// Clay Molitor
// Troy Barker

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "phone_book.h"



contact* newContact(char* firstName, char* lastName, char* city, char* address, char* phoneNumber, char* SSN)
{
    contact* cont = (contact*)malloc(sizeof(contact));

    if(cont == NULL)
    {
        printf("Failed to allocate memory");
    }
    cont->next = NULL;
    
    cont->firstName = firstName;
    cont->lastName = lastName;
    cont->city = city;
    cont->address = address;
    cont->phoneNumber = phoneNumber;
    cont->SSN = SSN;

}

// Pass root as reference
// Adds newContact to end
void insertContact(contact* root, contact* newContact) {
    
    contact* current = root;
    
    //if(root == NULL) {
    //    root = newContact;
    //    printf("Z "); // TODO this never runs, why? 
    //}

        printf("X ");
        while(current->next != NULL) // Iterate to last contact
        {
            printf("Y ");
            current = current->next;
        }
        current->next = newContact;
        
    
    printf("\n");
}
// Pass root by reference
// Pass NULL for any values that will not be used in the search
contact* searchContact(contact* root, char* firstName, char* lastName, char* city, char* address, char* phoneNumber, char* SSN)
{

    // array of all char* fields in contact.
    char* searches[(sizeof(contact)-sizeof(contact*)) / sizeof(char*) + 1] = 
        {firstName, lastName, city, address, phoneNumber, SSN, NULL};

    // Iterates all contacts.
    for(contact* tempContact = root; tempContact != NULL; tempContact = tempContact->next)
    {
        
        int i = 0;
        char* searchTemp = searches[i];
        bool match = true;
        // Iterates all elements in seaches array.
        for(int i = 0; searchTemp != NULL; searchTemp = searches[++i])
        {
            // Pointer to each field in current contact 
            char** data = &tempContact->firstName + i;


            if(strcmp(searchTemp, "") != 0) // If current search is not empty
            {
                if(strcmp(searchTemp, *data) == 0) {
                    // printf("\ti: %i Search: %s, \t Data: %s\n", i, searchTemp, *data); // print matched 
                }
                else { // any non
                    match = false;
                }
            }
        }
        if(match)
        {
            return tempContact;
        }
        printf("\n");
    }
    return NULL;
}
// Pass root by reference
// Removes the passed contact from the linked list.
void removeContact(contact* root, contact* target){

    contact* current = root;

    // If root is to be removed, edit the root pointer to be a reference.
    if(root == target)
    {
        current = &root;
    }

    while(current->next != NULL)
    {
        if(current->next == target)
        {
            contact* toRemove = current->next;
            current->next = current->next->next; 
            free(toRemove); // Deletes removed node from memory
        }
        current = current->next; // Go to next element
    }
}
// Prints the fields of the contact to terminal on one line.
// All values labeled and separated by a comma
void printContact(contact* data)
{
    printf("Full Name: %s %s, City: %s, Address: %s, Phone Number %s, SSN: %s\n", 
    data->firstName, data->lastName, data->city, data->address, data->phoneNumber, data->SSN);
}
 //  ---------------------------------- Code Tests ----------------------------------
// A user interface to test the functionality of the code above.
void userInterface()
{

    // I wanted to use GOTOs once and this is the least bad place I could find for them.

    contact* root = NULL;
    contact* current = NULL;
    char* inputs[6][256]; // Max input length of 128
    char prompts[6][20] = {"First Name: ", "Last Name: ", "City: ", "Address: ", "Phone Number: ", "SSN: "};

    bool remove = false; // if true search_contact will remove contacts 
    bool add = false; // if true search_contact will jump back to add_contact 
    // goto user_choice // Skip default entries
    current = newContact("Alf", "Arnold", "Boston", "132 3465th Ave.", "+1(456)-764-3455", "541-45-1234");
    insertContact(&root, current);
    current = newContact("Bob", "Bobert", "Seattle", "432 15th St.", "+1(123)-667-6675", "565-72-5432");
    insertContact(&root, current);
    current = newContact("Chuck", "Cuff", "Mt. Vernon", "1010 States Rd.", "+1(336)-764-0044", "987-95-9432");
    insertContact(&root, current);
    current = newContact("Dev", "Dorm", "Alberta", "645 States Rd.", "+1(336)-764-0044", "987-95-9432");
    insertContact(&root, current);

    goto user_choice;

search_contact:
    printf("Enter The information of the contact you would like to select, Leave blank any fields not to be searched for\n");

user_input: // Begin add_contact jump 
    // For each field in contacts, prompt and get a user input
    for(int i = 0; i < 6; i++) 
    {
        printf("%s", &prompts[i]);
       
        gets( inputs[i]);
        
    }
    if(add) { goto add_contact_b;} // End add_contact jump 


    current = searchContact(root, inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

    if(current == NULL) // If no contact found
    {
        printf("This entry was not found");
    }
    else // Contact found
    {
        if(remove)
        {
            printf("Removing: \n\t");
            printContact(current);
            removeContact(&root, current);
        }
        else
        {
            printf("Found: \n\t");
            printContact(current);
        }
    }
    printf("\n");
    remove = false;

user_choice:

    int choice = 0;
    printf("1: Print contacts, 2: Search contacts, 3: Remove contact, 4: Add contact, 5: quit > ");
    scanf("%i", &choice);
    gets(inputs[0]); // Clear buffer after scanf usage
    switch (choice)
    {
    case 1:
        goto print_all;
    case 2:
        remove = false;
        goto search_contact;
    case 3:
        remove = true;
        goto search_contact;
    case 4:
        goto add_contact;
    case 5:
        goto end;
    
    default:
        goto user_choice;
    }

add_contact: // Prompts the user for contact info and adds it to the array
    add = true; // Flags user_input to jump back to add_contact_b
    printf("Enter The contact information\n");

    goto user_input;
add_contact_b: // part two of add contact, called from user_input.
    insertContact(&root, 
        newContact(inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]));

    add = false;
print_all:

    // For each in list, print full name.
    printf("\n");
    for (contact* temp = root; temp != NULL; temp = temp->next)
    {
        printContact(temp);
    }
    printf("\n");
    goto user_choice;
end:

}

int main()
{
    userInterface();
    
    return 0;
}
