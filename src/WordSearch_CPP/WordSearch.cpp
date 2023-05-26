#include <string>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <cstdlib>
#include <windows.h> // For Consol Colors

#include "WordSearch.hpp"
#include "Cluster.hpp"

using namespace std;


int seed = 2465496; // used to pick one of the clusters semi-randomly


int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);   

    std::cout << "Calculating" << std::endl;
    SetConsoleTextAttribute(hConsole, 9);



    auto optimalClusters = vector<Cluster>();
    list<string> words = list<string>();

    words.push_back("walrus");
    words.push_back("cactus");
    words.push_back("lizard");
    words.push_back("dolphin");
    
    optimalClusters = Cluster::run(words);
    // one of the highest scoring clusters is chosen with seed. Randomize seed to choose a random cluster.
    Cluster chosenCluster = optimalClusters[seed % optimalClusters.size()];
    auto chosenArray = Cluster::ToArray(chosenCluster);


    cout << "High value clusters:       " << optimalClusters.size() << endl;
    for(int y = 0; y < chosenArray[0].size(); y++)
    {
        for (int x = 0; x < chosenArray.size(); x++)
        {
            
            char val = chosenArray[x][y];
            cout << ((val == '\0')? ' ' : val) << ' ';
        }
        cout << endl;  
    }
    /*
    for(int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            char output; 
            try
            {
                output = chosenCluster[x][y];
            }
            catch(const std::exception& e)
            {
                output = ' ';
            }
            cout << " " << ((output == '\0')? ' ' : output);
        }
        cout << '|' <<endl << '|';
    }
    for (int x = 0; x < 20*2; x++)
        cout << '=';
    cout <<endl;
    */
    
    SetConsoleTextAttribute(hConsole, 13);
    std::cout << "Finished" << std::endl;
    SetConsoleTextAttribute(hConsole, 15);

    return 0;
}


