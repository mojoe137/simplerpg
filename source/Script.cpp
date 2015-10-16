#include <fstream>
#include <windows.h>

#include "Script.h"

Script::Script(char * filename)
{
	std::string line;
	std::string entryText;

	line.clear();
	entryText.clear();

	std::ifstream myfile (filename);
    if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,line);
			if( line.length() <= 1 )
			{
				if( entryText.length() != 0 )
				{
					unsigned index = entryText.find_first_of(" \n");
					if( index != std::string::npos )
					{
						//OutputDebugString(entryText.substr(0,index).c_str());
						//OutputDebugString("\n");
						entry temp;
						temp.name = new std::string(entryText.substr(0,index));
						temp.str = new std::string(entryText.substr(index));
						stringEntries.push_back(temp);
					}
				}
				entryText.clear();
			}
			else
			{
				entryText.append(line);
				entryText.append("\n");
			}
		}

    myfile.close();
	}
}

Script::~Script()
{

}

std::string Script::GetString(std::string name)
{
	for( int i = 0; i < stringEntries.size(); i++ )
	{
		if( stringEntries[i].name->compare(name) == 0 )
		{
			return *stringEntries[i].str;
		}
	}
	return NULL;
}

int Script::GetInt(std::string name)
{
	for( int i = 0; i < stringEntries.size(); i++ )
	{
		if( stringEntries[i].name->compare(name) == 0 )
		{
			return atoi(stringEntries[i].str->c_str());
		}
	}
	return 0;
}