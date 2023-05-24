/*
    Clay Molitor
    CS 145: 
    Towers Of Hanoi: 

    Towers of Hanoi is a puzzle problem involving moving disk between three towers.
    The first tower starts with n disks of graduated sizes, with smallest on the top.
    You must move all the disks to another tower.
    Larger disks can't be stacked on smaller disks and only one disk can be moved at a time.
    My solution bellow demonstrates a recursive solution, based on wikipedia's example:
        https://en.wikipedia.org/wiki/Tower_of_Hanoi#Recursive_implementation

    For Extra credit:
        I used C

*/

#include <stdio.h>


#define TOWER_SIZE 20


// Returns and zeros the value at the index
int pop(int array[], int n)
{
    int output = array[n];
    array[n] = 0;
    return output;
}
// Sets the value at the index to val
void push(int val, int array[], int n)
{
    array[n] = val;
}
// Recursivly sorts three arrays following tower of hanoi's rules
void HanoiTower(int n, int source[], int target[], int other[])
{
    if(n != 0)
    {   
        HanoiTower(n - 1, source, other, target);

        push(pop(source, n), target, n);

        HanoiTower(n-1, other, target, source);
    }
}

// Prints the three towers to console
void printTowers(int A[], int B[], int C[])
{
    printf("\nA: ");
    for(int i = 0; i < TOWER_SIZE; i++)
    {
        printf("%i, ", A[i]);
    }
    printf("\nB: ");
    for(int i = 0; i < TOWER_SIZE; i++)
    {
        printf("%i, ", B[i]);
    }
    printf("\nC: ");
    for(int i = 0; i < TOWER_SIZE; i++)
    {
        printf("%i, ", C[i]);
    }
    return;
}

void main()
{
    // A, B, and C represent the three poles
    int A[TOWER_SIZE];
    int B[TOWER_SIZE];
    int C[TOWER_SIZE];

    // Zero B and C, "stack" numbers on A
    for(int i = 0; i < TOWER_SIZE; i++)
    {
        A[i] = i; //0 - 19
        B[i] = 0;
        C[i] = 0;
    }

    // Prints tower's original position
    printTowers(A, B, C); 
    printf("\n\nStarting sorting");

    HanoiTower(TOWER_SIZE-1, A, C, B);

    printf("\n\nFinished Tower:");
    // Prints sorted position
    printTowers(A,B,C);

}