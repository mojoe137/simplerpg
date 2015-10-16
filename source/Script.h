#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <string>

class Script
{
public:
	Script(char * filename);
	~Script();

	std::string GetString(std::string name);
	int GetInt(std::string name);

private:
	typedef struct
	{
		std::string * name;
		std::string * str;
	} entry;
	std::vector <entry> stringEntries;
};

#endif