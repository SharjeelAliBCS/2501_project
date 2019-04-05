#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <stdexcept>
#include <string>

#include <map>

#pragma comment(lib, "Winmm.lib")

class Audio
{
public:
	Audio();
	
	wchar_t* strToLPCWSTR(const std::string& s);
	void addAudio(std::string file, std::string alias);
	void pause(std::string alias);
	void resume(std::string alias);
	void play(std::string alias);
	void playRepeat(std::string alias);
	void playAgain(std::string alias);
	void stop(std::string alias);
	void close(std::string alias);
	void volume(std::string alias, int volume);


private:
	std::map < std::string, std::string> audiofiles;
};


	

