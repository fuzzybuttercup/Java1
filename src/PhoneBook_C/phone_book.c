// Clay Molitor
// Troy Barker
// Aiden Lim

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>




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
// Pass NULL or "" for any values that will not be used in the search
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
        
        // Iterates all fileds in seaches.
        for(int i = 0; searchTemp != NULL; searchTemp = searches[++i])
        {
            // Pointer to each field in current contact 
            char** data = &tempContact->firstName + i;

            //TODO compare and return any matches.
            printf("\ti: %i Search: %s, \t Data: %s\n", i, searchTemp, *data);
            
        }
        printf("\n");
    }
    return root;
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


int main()
{
    printf("Starting...\n");

    contact* a = newContact("Alf", "Armold", "Boston", "132 3465th Ave.", "+1(456)-764-3455", "541-45-1234");
    contact* b = newContact("Bob", "Cilton", "Seattle", "432 15th St.", "+1(123)-667-6675", "565-72-5432");
    contact* c = newContact("Chuck", "Ctuff", "Mt. Vernon", "1010 States Rd.", "+1(336)-764-0044", "987-95-9432");
    contact* d = newContact("Dev", "Dorm", "place", "645 States Rd.", "+1(336)-764-0044", "987-95-9432");

    contact* first = NULL;

    insertContact(&first, a);
    insertContact(&first, b);
    insertContact(&first, c);
    insertContact(&first, d);

    //removeContact(&first, a);
    //removeContact(&first, c);

    searchContact(first, "First", "Last", "Town", "123 Street LN.", "(123)123-1234", "123-12-1234");

    // For each in list
    contact* temp = first;
    while (temp != NULL)
    {
        printf("Name: %s %s\n", temp->firstName, temp->lastName);
        
        temp = temp->next;
    }
    
    
    return 0;
}

