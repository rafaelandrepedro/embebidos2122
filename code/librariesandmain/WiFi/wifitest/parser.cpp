#include "parser.h"

std::string getWord(std::string str, int pos)
{
    std::stringstream s(str);
    std::string word;
    for(int i=0;i<pos;i++){s >> word;}
    return word;
}

int getNumber(std::string str, int pos)
{
    int number;
    std::stringstream s(getWord(str, pos));
    s >> number;
    return number;
}

int getSize(std::string str)
{
    int count=0;
    std::stringstream s(str);
    std::string word;
    while(s){s >> word;}
    count++;
    return count;
}











