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


            if(searchTemp != NULL || searchTemp != "" || searchTemp != 0) // If current search is not NULL
            {
                if(strcmp(searchTemp, *data) == 0) {
                    // printf("\ti: %i Search: %s, \t Data: %s\n", i, searchTemp, *data);
                }
                else {
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

    // If root is to be removed, edit the root pointer
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
 //  ---------------------------------- Code Tests ----------------------------------
// A user interface to test the functionality of the code above.
void userInterface()
{

    contact* root = NULL;
    contact* current = NULL;
    char* inputs[6][256]; // Max input length of 128
    char prompts[6][20] = {"First Name: ", "Last Name: ", "City: ", "Address: ", "Phone Number: ", "SSN: "};

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
    printf("Enter The information of the contact you would like to find, Leave blank any fields not to be searched for\n");

get_contact:

    
    // For each field in contacts, prompt and get a user input
    for(int i = 0; i < 6; i++) 
    {
        printf("%s", &prompts[i]);
       
        gets( &inputs[i]);
        
    }
    current = searchContact(root, inputs[0], inputs[1], inputs[2], inputs[3], inputs[4], inputs[5]);

    if(current == NULL)
    {
        printf("This entry was not found");
    }
    else
    {
        printf("Name: %s %s\n", current->firstName, current->lastName);
    }
user_choice:


    goto search_contact;

print_all:

    // For each in list, print full name.
    for (contact* temp = root; temp != NULL; temp = temp->next)
    {
        printf("Name: %s %s\n", temp->firstName, temp->lastName);
        
    }
    
end:

}

int main()
{
    userInterface();
    //removeContact(&first, c);
    
    return 0;
}

void userInput(){



    return;
}
