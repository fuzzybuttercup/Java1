// Clay Molitor
// Troy Barker
// Aiden Lim

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
void removeContact(contact*, char*);


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


void insertContact(contact* first, contact* newContact) {
    
    contact* current = first;
    
    if(first == NULL) {
        first = newContact;
        printf("Z "); // TODO this never runs, why? 
    }
    else {
        printf("X ");
        while(current->next != NULL) // Iterate to last contact
        {
            printf("Y ");
            current = current->next;
        }
        current->next = newContact;
    }
    printf("\n");
}

void removeContact(contact* first, char* search){

    //if(first != NULL)
}


int main()
{
    printf("Starting...\n");

    contact* a = newContact("Bob", "Armold", "Boston", "132 3465th Ave.", "+1(456)-764-3455", "541-45-1234", 71);
    contact* b = newContact("Susan", "Milton", "Seattle", "432 15th St.", "+1(123)-667-6675", "565-72-5432", 12);
    contact* c = newContact("Rob", "Stuff", "Mt. Vernon", "1010 States Rd.", "+1(336)-764-0044", "987-95-9432", 102);

    contact* first = NULL;

    insertContact(&first, a);
    insertContact(&first, b);
    insertContact(&first, c);

    printf("a: %s\n", first->firstName);
    printf("b: %s\n", first->next->firstName);
    printf("c: %s\n", first->next->next->firstName);
    
    return 2;
}

