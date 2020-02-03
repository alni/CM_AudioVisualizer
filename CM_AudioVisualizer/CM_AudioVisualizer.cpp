// CM_AudioVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>
#include <iostream>
#include "SDKDLL.h"

class InputParser {
public:
	InputParser(int &argc, char **argv) {
		for (int i = 1; i < argc; ++i)
			this->tokens.push_back(std::string(argv[i]));
	}
	/// @author iain
	const std::string& getCmdOption(const std::string &option) const {
		std::vector<std::string>::const_iterator itr;
		itr = std::find(this->tokens.begin(), this->tokens.end(), option);
		if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
			return *itr;
		}
		static const std::string empty_string("");
		return empty_string;
	}
	/// @author iain
	bool cmdOptionExists(const std::string &option) const {
		return std::find(this->tokens.begin(), this->tokens.end(), option)
			!= this->tokens.end();
	}
private:
	std::vector <std::string> tokens;
};

void initCMDevice()
{
	SetControlDevice(DEV_MKeys_M_White);
	EnableLedControl(true);
	SetFullLedColor(51, 51, 51);
}

void deinitCMDevice()
{
	EnableLedControl(false);
}

typedef struct MyData {
	unsigned int counter;
	float threshold;
} MYDATA, *PMYDATA;

DWORD WINAPI myMusicThread(LPVOID lpParameter)
{
	PMYDATA pData;

	pData = (PMYDATA)lpParameter;
	//unsigned int& myCounter = *((unsigned int*)lpParameter);
	unsigned int myCounter = pData->counter;
	float threshold = pData->threshold;
	float fVolValue = GetNowVolumePeekValue();
	initCMDevice();
	while (myCounter < 0xFFFFFFFF)
		//while(true)
	{
		//++myCounter;
		float fPeekValue = GetNowVolumePeekValue();
		//if (fVolValue - fPeekValue > 0.1f || fPeekValue - fVolValue > 0.1f)
		if (fPeekValue > threshold) //0.30f)
		{
			//fVolValue = fPeekValue + 0.0f;
			BYTE bVol = BYTE(fPeekValue * 255);
			SetFullLedColor(bVol, bVol, bVol);
			//Sleep(450);
		}
		else
		{
			SetFullLedColor(51, 51, 51);
			//Sleep(450);
		}
		Sleep(100);
	}
	deinitCMDevice();
	return 0;
}

void setupMusicThreads(float threshold)
{
	using namespace std;

	unsigned int myCounter = 0;
	PMYDATA pData;
	DWORD myThreadID;

	pData = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
	pData->counter = myCounter;
	pData->threshold = threshold + 0.0f;

	HANDLE myHandle = CreateThread(0, 0, myMusicThread, pData, 0, &myThreadID);
	char myChar = ' ';
	while (myChar != 'q')
	{
		cout << myCounter << endl;
		myChar = getchar();
	}
	myCounter = 0xFFFFFFFF;

	CloseHandle(myHandle);
	deinitCMDevice();
}

#define DEF_BRIGHTNESS 170
#define ALT_BRIGHTNESS 255

int main(int argc, char *argv[], char *envp[])
{
	std::cout << "Hello World!\n";

	InputParser input(argc, argv);

	float fThreshold = 0.30f;

	if (input.cmdOptionExists("threshold"))
	{
		fThreshold = std::strtof((input.getCmdOption("threshold")).c_str(), 0);
	}
	setupMusicThreads(fThreshold);
	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
