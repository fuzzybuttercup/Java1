

struct contact {
    struct contact* next;

    char* firstName;
    char* lastName;
    char* city;
    char* address;
    char* phoneNumber;

    char* SSN;
};

// sets "contact" as a shorthand of "struct contact".
typedef struct contact contact;


void insertContact(contact*, contact*);
contact* newContact(char*, char*, char*, char*, char*, char*);
contact* searchContact(contact*, char*, char*, char*, char*, char*, char*);
void removeContact(contact*, contact*);