#include <string>
#include <iostream>
#include <list>
#include <map>
//#include <algorithm>
#include <vector>
#include <memory>
#include <windows.h> // For Consol Colors

#include "WordSearch.hpp"
#include "Cluster.hpp"

using namespace std;


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
    words.push_back("cplusplus");
    
    optimalClusters = Cluster::run(words);


    cout << "High value clusters:       " << optimalClusters.size() << endl;
    //cout << "Total clusters calculated: " << counter <<endl;
    int maxScore = 0;
    map<int, map<int, char>> bestCluster;
    /*for( auto cluster : optimalClusters)
    {
        if(cluster.getScore() > maxScore)
        {
            maxScore = cluster.getScore();
            bestCluster = cluster.getMap();
        }

        for(int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                char output; 
                try
                {
                    output = cluster.getMap()[x][y];
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
    }
    */
    SetConsoleTextAttribute(hConsole, 13);
    std::cout << "Finished" << std::endl;
    SetConsoleTextAttribute(hConsole, 15);

    return 0;
}


