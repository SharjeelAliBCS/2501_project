#pragma once

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class FileUtils {
public:
	static std::string LoadTextFile(const char *filename);
	static std::vector<std::string> LoadVectorTextFile(const char *filename);
};