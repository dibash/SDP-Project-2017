/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Dimitar Bashliev
* @idnumber 45145
* @task 0
* @compiler VC
*
*/

// Uncomment following line to use implicit concatenation.
// This allows to use the '.' symbol in regular expression as actual .
// instead of concatenation.
// #define USE_IMPLICIT_CONCAT_ONLY

#include"NFA.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<Windows.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

void GetFilesInDirectorySystem(std::vector<string> &out, const string &directory);

int main(int argc, char* argv[])
{

	if (argc != 3)
	{
		cout <<
			"Usage:\n\t"
			"RegEx <file|directory> \"<regular expression>\"\n"
			"See docs for more information.\n\n"
			"******Project for Data Structures class in FMI******\n"
			"******************Dimitar Bashliev******************\n"
			<< endl;
		exit(0);
	}


	try 
	{
		NFA nfa(argv[2]);

		std::ifstream fileToProcess;
		string line;
		std::vector<string> fileList;
		GetFilesInDirectorySystem(fileList, argv[1]);


		for(const string& fileName : fileList)
		{
			fileToProcess.open(fileName.c_str());
			if (fileToProcess)
			{
				size_t lineNumber = 0;
				while (std::getline(fileToProcess, line))
				{
					lineNumber++;
					if (nfa.accepts(line))
						cout << "\"" << fileName << "\":" << lineNumber << ":" << line << endl;
				}

			}
			fileToProcess.clear();
			fileToProcess.close();
		}

	}
	catch (std::runtime_error& e)
	{
		cerr << "You entered wrong input. Exception caught:\n";
		cerr << e.what() << endl;
		exit(0);
	}
	catch (std::logic_error& e)
	{
		cerr << "Ouch! Something went wrong with the program. Exception caugth:\n";
		cerr << e.what() << endl;
		exit(1);
	}
	catch (std::exception& e)
	{
		cerr << "Unexpected error occured:\n";
		cerr << e.what() << endl;
		exit(1);
	}
	catch (...)
	{
		cerr << "Unknown exception occured!" << endl;
		exit(1);
	}

	return 0;
}

void GetFilesInDirectorySystem(std::vector<string>& out, const string& directory)
{
	string tempFileName = "out.tmp";
	system(("dir \"" + directory + "\" /b /s /a-h-s-d >" + tempFileName).c_str());

	string fileName;
	std::ifstream file(tempFileName);
	
	if (!file)
	{
		cerr << "Couldn't open temp file with the list of files to search.\n";
		cerr << "Maybe the process doesn't have writing permissions?" << endl;
		exit(1);
	}
	while (std::getline(file, fileName))
		out.push_back(fileName);

	file.close();
	remove(tempFileName.c_str());
}