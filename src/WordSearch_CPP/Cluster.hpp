
#ifndef CLUSTER_H // include guard
#define CLUSTER_H

#include <string>
#include <map>
#include <vector>
#include <list>

#include "WordSearch.hpp"

// Stores words for crossword puzzle in 2d dictionary, attempting to overlap them.
// Larger score indicate more overlapping.
class Cluster {
    std::map<int, std::map<int, char>> clusterMap;
    int score; // How optimal the cluster is
    //std::list<std::string> allWords; // Stores the words input to the constructor
    int x = 0, y = 0; // Width and height of cluster

    enum DIRECTION 
    {
        NONE, UPLEFT, UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT
    };

    class Word; //  needed for the header to compile. 
public: 
    Cluster(std::map<int, std::map<int, char>> clusterMap, int score);
    int getScore();
    int getX();
    int getY();
    std::map<int, std::map<int, char>> getMap();

    // Returns a list all clusters tied for highest score
    static std::vector<Cluster> run(std::list<std::string> allWords);
    static std::vector<std::vector<char>> ToArray(Cluster cluster);
    static void getIndexFromDirection(DIRECTION, int&, int&);

private:
    static void generateCluster(Word* , std::map<int, std::map<int, char>>*, int, int, int& );
    static void chainBuilder(Word* prev, std::list<std::string> words, std::vector<Cluster> &output);
    static void storeCluster(Word* word, std::list<std::string> leftOvers,
        std::vector<Cluster> &output);


    
        // ---------------------- Word nested class --------------------------
private:
    class Word {
        public:
        Word* prev;
        std::string data;
        DIRECTION dir;
        int prevIntersect; // Index on the current string, where it intersects the previous string.
        int nextIntersect; // Index on the current string, where it intersects the next string.
        
        Word(Word* prev, std::string word, DIRECTION dir, int prevIntersect, int currentIntersect);
    };
};



#endif