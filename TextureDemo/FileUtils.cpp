#include "FileUtils.h"

std::string FileUtils::LoadTextFile(const char *filename) {
	// Open file
	std::ifstream f;
	f.open(filename);
	if (f.fail()) {
		throw(std::ios_base::failure(std::string("Error opening file ") + std::string(filename)));
	}

	// Read file
	std::string content;
	std::string line;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	// Close file
	f.close();

	return content;
}

std::vector<std::string> FileUtils::LoadVectorTextFile(const char *filename) {
	std::vector < std::string>  strings;


	std::ifstream f;
	f.open(filename);
	if (f.fail()) {
		throw(std::ios_base::failure(std::string("Error opening file ") + std::string(filename)));
	}

	// Read file
	std::string content;
	std::string line;
	while (std::getline(f, line)) {
		content += line + "\n";
		if (line == "") {
			strings.push_back(content);
			content = "";
		}
	}

	// Close file
	f.close();

	return strings;


}

std::pair<int, int> FileUtils::loadMapSize(std::string fname) {
	int wid = 0;
	int height=0;
	std::ifstream in(fname);
	std::string line, field;
	while (getline(in, line)) {
		if (!height) {
			std::stringstream ss(line);
			while (getline(ss, field, ','))  // break line into comma delimitted fields
			{
				++wid;
			}
		}
		++height;
	}
	in.close();

	return std::make_pair(wid, height);

}
