// Clay Molitor
// Troy Barker
// Aiden Lim

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

struct contact {
    struct contact* next;

    char* firstName;
    char* lastName;
    char* city;
    char* address;
    char* phoneNumber;

    char* SSN;
    int age;
};

// sets "contact" as a shorthand of "struct contact".
typedef struct contact contact;


void insertContact(contact*, contact* );
contact* newContact(char*, char*, char*, char*, char*, char*, int);
contact* searchContact(contact*, char*, char*);
void removeContact(contact*, contact*);


contact* newContact(char* firstName, char* lastName, char* city, char* address, char* phoneNumber, char* SSN, int age)
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
    cont->age = age;

}

// Pass first as reference
void insertContact(contact* first, contact* newContact) {
    
    contact* current = first;
    
    //if(first == NULL) {
    //    first = newContact;
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
// Pass first by reference
contact* searchContact(contact* first, char* firstName, char* lastName)
{

    return first;
}
// Pass first by reference
void removeContact(contact* first, contact* target){

    contact* current = first;

    // If first is to be removed
    if(first == target)
    {
        current = &first;
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

    contact* a = newContact("Alf", "Armold", "Boston", "132 3465th Ave.", "+1(456)-764-3455", "541-45-1234", 71);
    contact* b = newContact("Bob", "Milton", "Seattle", "432 15th St.", "+1(123)-667-6675", "565-72-5432", 12);
    contact* c = newContact("Chuck", "Stuff", "Mt. Vernon", "1010 States Rd.", "+1(336)-764-0044", "987-95-9432", 102);
    contact* d = newContact("Dev", "Norm", "place", "645 States Rd.", "+1(336)-764-0044", "987-95-9432", 12);

    contact* first = NULL;

    insertContact(&first, a);
    insertContact(&first, b);
    insertContact(&first, c);
    insertContact(&first, d);

    removeContact(&first, a);
    removeContact(&first, c);

    printf("a: %s\n", first->firstName);
    printf("b: %s\n", first->next->firstName);
    //printf("d: %s\n", first->next->next->firstName);
    
    return 0;
}

