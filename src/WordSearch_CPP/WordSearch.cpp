#include <string>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <vector>
#include <memory>
#include <windows.h> // For Consol Colors

#include "WordSearch.hpp"

using namespace std;

//


Word::Word(Word* prev, string word, DIRECTION dir, int prevIntersect, int currentIntersect)
{
    // Adds current intersection to the previous word.
    if(prev != NULL)
    {
        prev->nextIntersect = prevIntersect ;
    }
    this->prevIntersect = currentIntersect ;
    this->nextIntersect = 0;

    this->prev = prev;
    this->data = word;
    this->dir = dir;
}
    
    

// Stores overlaid words for crossword puzzle
// Larger score indicate more overlapping.
class Cluster {
    map<int, map<int, char>> clusterMap;
    int score;

public:
    Cluster(map<int, map<int, char>> clusterMap, int score)
    {
        this->clusterMap = clusterMap;
        this->score = score;
    }
    int getScore()
    {
        return score;
    }
    map<int, map<int, char>> getMap()
    {
        return clusterMap;
    }
};

vector<Cluster> optimalClusters = vector<Cluster>();

// Saves highest scoring clusters to optimalClusters
// Called for every possible cluster found by chainBuilder
// Calls generateCluster to turn linked words into word search ready arrays.
void storeCluster(Word* word, list<string> leftOvers)
{

    try
    {
        auto mapResults = map<int, map<int, char>>();
        int score = 0;

        generateCluster(word, &mapResults, 7, 7, score); // Throws error if words don't forma a valid cluster

        // Only store high scoring clusters, for performance.
        int highScore = 0;
        
        if(optimalClusters.size() > 0)
            highScore = optimalClusters.front().getScore(); 
        

        if(score > highScore && !optimalClusters.empty()) // If new score is higher then leading old score
        {   
                optimalClusters.clear();
        }
        if(score >= highScore || optimalClusters.empty()) // If score ties existing high score
        {
            optimalClusters.push_back(Cluster(mapResults, score));
        }

    }
    catch(exception e) // Bad cluster was generated
    {
        // Do nothing
    }

}

// Chains words into overlapping clusters of words
// Results are stored in optimalClusters indirectly through storeCluster()
static void chainBuilder(Word* prev, list<string> words)
{
    static int count = 0;
    string a = prev->data;
    bool deadEnd = true; // Will remain true if there are no more matches in this tree
    if(words.size() >= 1) // While there are words left to be added
    {
        for(string b: words) // For each remaining word
        {
            for(int dir = UPLEFT; dir < LEFT; dir++) // for each direction
            {
                list<string> remainingWords = words;
                remainingWords.remove(b); // Remove the current word from remaining words.
                for (int x = 0; x < a.length(); x++)
                {
                    for (int y = 0; y < b.length(); y++)
                    {
                        if(a[x] == b[y]) // If the current word can intersect the previous word
                        {
                            deadEnd = false;
                            Word next = Word(prev, b, (DIRECTION)dir, x, y);
                            //cout << a << ":" << x << ", " << b << ":" << y <<endl;
                            
                            chainBuilder(&next, remainingWords); // Recurse, with the current word being removed from words
                        }
                    }
                }
            }

        }
    }

    if(deadEnd)
    {   
        storeCluster(prev, words);
    }

}

// Turns chained words into an array of characters usable in a word search game
// The map is returned by editing cluster, cluster must be initialized.
// Map is indexed with 2 ints, x and y
// x and y offset map's indexes
// Non character spaces are not mapped and will return '\0'
// Indexes can be negative, index 0,0 is the beginning of the word chain.
// Throws error if the cluster would result overlaps overwriting existing words
void generateCluster(Word* word, map<int, map<int, char>>* cluster, int x, int y, int& score)
{
    /* Example output given the linked words "hat", "search", "words"
    s
    d e   
    r   a 
    o     r   t
    w       c a
              h
    */
    int xdir, ydir;
    getIndexFromDirection(word->dir, xdir, ydir); // Sets xdir and ydir as increments for iterating through a 2d array

    // We are iterating through the words backwards, branch to root, so we need to flip the strings around front to back.
    
    
    // Backtrack index to word intersection point
    for(int i = word->nextIntersect; i > 0; i--)
    {
        x -= xdir;
        y -= ydir;
    }
    for(int i = 0; i < word->data.length(); i++)
    {
        // Recurs into printing the next word
        if(word->prevIntersect == i && word->prev != NULL) 
        {
            score += 1; // Score one point for an intersection
            generateCluster(word->prev, cluster, x, y, score);
        }
        // ---- Write to cluster ----   
        if((*cluster)[x][y] == '\0') // If cluster is empty
        {
            (*cluster)[x][y] = word->data[i]; // Write
        }
        else if((*cluster)[x][y] == word->data[i]) { // If overwriting matching character
            score += 16; // Add 16 points for doubling back on itself 
        }
        else { // Trying to overwrite non-matching character.
            throw invalid_argument("Bad cluster");
        }
        x += xdir;
        y += ydir;
    }
    // Print
    

}
// Sets x and y to allow for indexing along the given direction.
// Up and right are positive
void getIndexFromDirection(DIRECTION dir, int& x, int& y)
{
    // EX: RIGHT> x:1, y:0    DOWNLEFT> x:-1, y:-1 

    switch(dir)
    {
        case UPLEFT:
            x = -1; y = -1;
        break;
            case UP:
            x = 0; y = -1;
        break;
            case UPRIGHT:
            x = 1; y = -1;
        break;
            case RIGHT:
            x = 1; y = 0;
        break;
            case DOWNRIGHT:
            x = 1; y = 1;
        break;
            case DOWN:
            x = 0; y = 1;
        break;
            case DOWNLEFT:
            x = -1; y = 1;
        break;
            case LEFT:
            x = -1; y = 0;
        break;
            default:
            throw invalid_argument("getIndexFromDirection only takes Directions as input.");
    }


}
int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);   

    std::cout << "Calculating" << std::endl;
    SetConsoleTextAttribute(hConsole, 9);


    list<string> words = list<string>();

    //words.push_back("walrus");
    words.push_back("cactus");
    words.push_back("lizard");
    words.push_back("dolphin");
    words.push_back("cplusplus");

    Word theGood = Word(NULL, "walrus", RIGHT, 0, 0);
    chainBuilder(&theGood, words);

    // Word a = Word(NULL, "walrus", RIGHT,  0, 4);
    // Word b = Word(&a, "cactus", UPLEFT, 4, 4);
    // Word c = Word(&b, "antics", LEFT, 3, 2);

    int score = 0;
    //auto mapped = generateCluster2(&b, 10, 10, w, h, score);
    auto mapped = map<int, map<int, char>>();
    //generateCluster(&c, &mapped, 10, 10, score);

    cout << "Clusters: " << optimalClusters.size() << endl;

    int maxScore = 0;
    map<int, map<int, char>> bestCluster;
    for( auto cluster : optimalClusters)
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

    SetConsoleTextAttribute(hConsole, 13);
    std::cout << "Finished" << std::endl;
    SetConsoleTextAttribute(hConsole, 15);

    return 0;
}


