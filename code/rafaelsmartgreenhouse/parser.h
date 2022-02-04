#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <sstream>
#include <vector>

std::string getWord(std::string str, int pos);

int getNumber(std::string str, int pos);

int getSize(std::string str);

class Parser{
private:
	std::vector <void* (*)(void*)> commandList;
	std::vector <std::string> names;
public:
	void add(void*(*ptr)(void*), std::string name){
		commandList.push_back(ptr);
		names.push_back(name);
	}
	void search(std::string command){
		for(int i=0;i<commandList.size();i++)
			if(!getWord(command, 1).compare(names.at(i)))
				commandList.at(i)(&command);
	}
};


#endif

