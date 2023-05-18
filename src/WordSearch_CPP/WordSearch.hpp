#ifndef WORD_SEARCH_H // include guard
#define WORD_SEARCH_H



#include <string>
#include <map>



enum DIRECTION 
{
    NONE, UPLEFT, UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT
};



class Word {
public:
    Word* prev;
    std::string data;
    DIRECTION dir;
    int prevIntersect; // Index on the current string, where it intersects the previous string.
    int nextIntersect; // Index on the current string, where it intersects the next string. 0 = no intersection
    
    Word(Word* prev, std::string word, DIRECTION dir, int prevIntersect, int currentIntersect);
    
    
};






void getIndexFromDirection(DIRECTION, int&, int&);

void generateCluster(Word* , std::map<int, std::map<int, char>>*, int, int, int& );




#endif