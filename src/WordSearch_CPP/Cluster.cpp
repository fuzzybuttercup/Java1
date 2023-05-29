#include <iostream>
#include "Cluster.hpp"


using namespace std;

// ---------------------------------- Cluster ------------------------------------
// Cluster and it's nested class Word are used to generate word searches.
// A list of words is passed to run() and an array with as many possible overlapping words is returned.

Cluster::Cluster(map<int, map<int, char>> clusterMap, int score)
{
    this->clusterMap = clusterMap;
    this->score = score;
}

 // ----------- Accessors -------------
int Cluster::getScore() {
    return score;
}
int Cluster::getX() {
    return x;
}
int Cluster::getY() {
    return y;
}
map<int, map<int, char>> Cluster::getMap()
{
    return clusterMap;
}

// --------- Public Methods ------------
// Called after constructor to generate a list of overlapping strings.
std::vector<Cluster> Cluster::run(list<string> words){

    std::vector<Cluster> output = std::vector<Cluster>();

    // Pop one of the strings from list and make a word for chainBuilder.
    string firstWord = words.front();
    words.pop_front();

    Word theGood = Word(NULL, firstWord, RIGHT, 0, 0);

    chainBuilder(&theGood, words, output);


    return output;
}
// Turns a cluster's map into a 2d vector, starting at 0,0. 
// The vector is as small as possible while containing the whole cluster.
// All elements of an axis are the same length.
// Empty characters are set to ' '.
vector<vector<char>> Cluster::ToArray(Cluster cluster)
{
    map<int, map<int, char>> clusterMap = cluster.getMap();
    auto results = vector<vector<char>>();

    // Gets Y dimensions.
    int xOffset = clusterMap.begin()->first;
    int xDist = distance(clusterMap.begin(), clusterMap.end());

    // Gets X dimensions.
    int minY, maxY, yOffset, yDist;
    minY = clusterMap.begin()->second.begin()->first; // First X index of the first row.
    maxY = minY; 
    for(auto i = clusterMap.begin(); i != clusterMap.end(); ++i) // For each row.
    {
        for(auto j = i->second.begin(); j != i->second.end(); ++ j) // For each element in column
        {
            int val =  j->first; // Index of X element in row.
            minY = min(minY, val);
            maxY = max(maxY, val);
        }
    }
    yOffset = minY;
    yDist = (maxY - minY) + 1;

    // Initiate results array
    results = vector<vector<char>>(xDist, vector<char>(yDist, ' '));

    for(int y = 0; y < yDist; y++)
    {
        for(int x = 0; x < xDist; x++)
        {
            char val = clusterMap[xOffset + x][yOffset + y];
            results[x][y] =  (val == '\0')? ' ' : val; // Set space if character is null
        }
    }

    return results;
}
// Turns a direction into an x and a y direction.
// Up and right are positive
void Cluster::getIndexFromDirection(DIRECTION dir, int& x, int& y)
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


// --------- Private Methods ------------

// Chains words into linked lists of Words.
// These chains are passed to generateCluster to generate arrays.
void Cluster::chainBuilder(Word* prev, list<string> words, vector<Cluster> &output)
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
                            
                            chainBuilder(&next, remainingWords, output); // Recurse, with the current word being removed from words
                        }
                    }
                }
            }
        }
    }

    if(deadEnd)
    {   
        storeCluster(prev, words, output);
    }

}



// Saves highest scoring clusters to optimalClusters
// Called for every possible cluster found by chainBuilder
// Calls generateCluster to turn linked words into word search ready arrays.
void Cluster::storeCluster(Word* word, list<string> leftOvers, vector<Cluster> &output)
{
    try
    {
        auto mapResults = map<int, map<int, char>>();
        int score = 0;

        generateCluster(word, &mapResults, 0, 0, score); // Throws error if words don't forma a valid cluster

        // Only store high scoring clusters, for performance.
        int highScore = 0;
        
        if(output.size() > 0)
        {
            highScore = output.front().getScore(); 
        }
        if(score > highScore && !output.empty()) // If new score is higher then leading old score
        {   
                output.clear();
        }
        if(score >= highScore || output.empty()) // If score ties existing high score
        {
            output.push_back(Cluster(mapResults, score));
        }

    }
    catch(exception e) // Bad cluster was generated
    {
        // Do nothing
    }

}


// Turns chained words into an array of characters usable in a word search game
// The map is returned by editing cluster, cluster must be initialized.
// Map is indexed with 2 ints, x and y
// x and y offset map's indexes
// Non character spaces are not mapped and will return '\0'
// Indexes can be negative, index 0,0 is the beginning of the word chain.
// Throws error if the cluster would result overlaps overwriting existing words
void Cluster::generateCluster(Word* word, map<int, map<int, char>>* cluster, int x, int y, int& score)
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


// ----------------------------- Word --------------------------------
Cluster::Word::Word(Word* prev, string word, DIRECTION dir, int prevIntersect, int currentIntersect)
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




