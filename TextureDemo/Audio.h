/*
Class: Audio
Purposes: Plays the game audio using micsendstring
Contributers: Sharjeel Ali
*/
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

	//converts the string to a micsendstring appropritate lpcwstr variable. 
	wchar_t* strToLPCWSTR(const std::string& s);
	//adds a new audio component to the game. 
	void addAudio(std::string file, std::string alias);
	//pauses the audio
	void pause(std::string alias);
	//reumes the audio
	void resume(std::string alias);
	//plays the audio only once
	void play(std::string alias);
	//plays the audio on repeat
	void playRepeat(std::string alias);
	//plays the audio again from any point
	void playAgain(std::string alias);
	//stops the audio
	void stop(std::string alias);
	//closes and removes the audio
	void close(std::string alias);
	//adjusts audio track volume. 
	void volume(std::string alias, int volume);


private:
	std::map < std::string, std::string> audiofiles;
};




