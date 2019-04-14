/*
Class: FileUtils
Purposes: Loads a file
Contributers: Sharjeel Ali,Raul Rodriguez
*/
#pragma once

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class FileUtils {
public:
	//loads a regular text file into a string
	static std::string LoadTextFile(const char *filename);
	//loads a text file into a vector of strings
	static std::vector<std::string> LoadVectorTextFile(const char *filename);
	//loads the map size coordinates from the csv file.
	static std::pair<int, int> loadMapSize(std::string fname);
};