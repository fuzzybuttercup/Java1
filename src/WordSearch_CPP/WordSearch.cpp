
/*
    Clay Molitor
    CS 145: Assignment 1, Word Search

    My program will prompt users to enter words and then display a word search puzzle.
    The puzzle can ve displayed with entered words highlighted.
    The program will ALWAYS overlap words if it is possible, however this thoroughness comes at the cost of performance.

    For extra credit I:
        used C++.
        Overlapped my words:
            If there is a possible way to overlap every word my program will find it.
            It will also overlap words multiple times some of the time, but no optimally.

    Example output 
        Entered words:  whatcom, university, cpluscplus, java, school
        Output:
            # # # # # # # # # # # # # # # #
            # b e h k o r u x c f i m p S #
            # s v a d g j n q t w b e U h #
            # l o r u y c f i m p s L v a #
            # d g k n q t x b e h P l o r #
            # u L y c f j m p s S w a d g #
            # k n O q t x b e U h l o r v #
            # y c f O j m p L s w a d g k #
            # n q u x H b P e i l o r v y #
            # c W H A T C O M f j m p t w #
            # U N I V E R S I T Y a d h k #
            # n q u A x b e i l o s v y c #
            # g j m J p t w a d h k n q u #
            # # # # # # # # # # # # # # # #
        Capital letters are solutions, the actual program prints solutions in a contrasting color as well
*/

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <cstdlib>
#include <windows.h> // For Consol Colors

#include "WordSearch.hpp"
#include "Cluster.hpp"

using namespace std;




void setColor(int color)
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
}
// Gets words from the user
// Changes console color
list<string> getWords()
{
    auto words = list<string>(); // Contains the list of user input strings.

    setColor(10); // Green
    cout << "Input words for your word search. More than five words may lead to poor performance." << endl;
    setColor(9); // Blue

    // ----------------------- Get user inputs -----------------------
    // Get words until q is entered;
    string input = "";
    do {
        setColor(9); // Blue
        cout << "Enter a word to search for (Enter 'q' to finish): ";

        setColor(7); // Off White        
        cin >> input;
        words.push_back(input);

    } while(input != "q");

    return words;
}

// Generates a word search array from the passed words
// If multiple optimal arrays are generated then seed will be used to choose one.
// More and Longer words will exponentially increase performance cost, recommend 5 medium length words.
vector<vector<char>> getWordSearch(list<string> words, int seed)
{
    // Cluster.Run is very performance heavy. More and Longer words will exponentially increase performance cost.
    auto optimalClusters = Cluster::run(words); // Stores all optimal clusters
    auto chosenCluster = optimalClusters[seed % optimalClusters.size()]; // Uses seed to choose a cluster.
    auto chosenArray = Cluster::ToArray(chosenCluster); // Turns the chosen cluster into an array.
    return chosenArray;
}
// Returns a character and increments seed. 
// The returned chars are deterministic, the same seed will return the same char.
char getFillerChar(int& seed)
{
    srand(seed);
    seed++;
    return (rand() % ('z' - 'a')) + 'a'; // Returns a char between a and z.
}
// Prints the word search to console.
// Highlights the solutions with color if showAnswer is true.
// Seed must be reset to its original value before recalling this function.
void printWordSearch(vector<vector<char>> wordArray, int& seed, bool showAnswer)
{
    const int defaultColor = 9, // Blue
        boarderColor = 2,       // Dark green
        highlightColor = 12;    // Red
    setColor(defaultColor); 
    // Print top boarder
    setColor(boarderColor); 
    for (int x = 0; x < wordArray.size() + 2; x++) { 
        cout << "# ";
    }   
    cout << endl; 
    setColor(defaultColor); 

    // For each element in 2d array wordArray
    for(int y = 0; y < wordArray[0].size(); y++)
    {
        setColor(boarderColor); 
        cout << "# "; // Print left boarder
        setColor(defaultColor); 
        
        for (int x = 0; x < wordArray.size(); x++)
        {
            
            char val = wordArray[x][y];
            if(val == ' ') { // If empty
                cout << getFillerChar(seed);
            }
            else { // If wordArray has value here print it.
                if(showAnswer) { 
                    setColor(highlightColor);
                    cout << (char)toupper(val); 
                    setColor(defaultColor);
                } else{
                    cout << val;
                }
                setColor(defaultColor);
            }
            cout << ' '; // pad with spaces to make word search more square.
        }
        setColor(boarderColor); 
        cout << "# "; // Print right boarder
        setColor(defaultColor); 
        cout << endl;  
    }
    // Print bottom boarder
    setColor(boarderColor); 
    for (int x = 0; x < wordArray.size() + 2; x++) { 
        cout << "# ";
    }   
    cout << endl; 
    setColor(defaultColor);
        
}
void userInterface()
{
    // Gets words for the word search and returns a word search array.
    int printSeed = DEFAULT_SEED; // Must be reset before calling getWordSearch();
    vector<vector<char>> wordArray;

    setColor(13); // Pink
    std::cout << "Welcome Clay's To Word Search" << std::endl;
    setColor(15); // White


    // While user input != q
    string input;
    do
    {
        setColor(7); // Off White
        cout << "Please select an option: \n" <<
            "\tGenerate a new word search (g)\n" <<
            "\tPrint out your word search (p)\n" <<
            "\tShow the solution to the word search (s)\n" <<
            "\tQuit the program (q)\n";
        setColor(14); // Yellow
        cin >> input;

        char first = input[0];
        switch (first)
        {
        case 'g': // Generate new word search
            wordArray = getWordSearch(getWords(), DEFAULT_SEED);
            break;
        case 'p': // print word search
            printSeed = DEFAULT_SEED; 
            printWordSearch(wordArray, printSeed, false);
            break;
        case 's': // print word search w/ answers
            printSeed = DEFAULT_SEED; 
            printWordSearch(wordArray, printSeed, true);
            break;
        case 'q': // silently break
            break;
    
        default:
            cout << "Invalid input, try again" << endl; 
            break;
        }
    }while(input[0] != 'q');

    setColor(13); // Pink
    std::cout << "Finished" << std::endl;
    setColor(15); // White
}



int main()
{ 
    userInterface();
    return 0;
}


