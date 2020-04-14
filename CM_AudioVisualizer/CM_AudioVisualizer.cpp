// CM_AudioVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>
#include <iostream>
#include "SDKDLL.h"
#include <unordered_map>

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

#define DEF_BRIGHTNESS 51

#define DEF_THRESHOLD 0.01f // 0.30f
#define MIN_THRESHOLD 0.01f
#define MAX_THRESHOLD 0.99f

#define DEF_MULTIPLIER 1.0f
#define MIN_MULTIPLIER 0.01f
#define MAX_MULTIPLIER 10.0f

// String to enum lookup.
// Adapted from example code on StackOverflow:
// https://stackoverflow.com/a/7163130 
static std::unordered_map<std::string, DEVICE_INDEX> const deviceTable = {
		{"CK372", DEV_CK372},
		{"CK530", DEV_CK530},
		{"CK550_552", DEV_CK550_552},
		{"CK551", DEV_CK551},
		{"MK750", DEV_MK750},
		{"MK850", DEV_MK850},
		{"MKeys_L", DEV_MKeys_L},
		{"MKeys_L_White", DEV_MKeys_L_White},
		{"MKeys_M", DEV_MKeys_M},
		{"MKeys_M_White", DEV_MKeys_M_White},
		{"MKeys_S", DEV_MKeys_S},
		{"MKeys_S_White", DEV_MKeys_S_White},
		{"MM520", DEV_MM520},
		{"MM530", DEV_MM530},
		{"MM830", DEV_MM830},
		{"MMouse_L", DEV_MMouse_L},
		{"MMouse_S", DEV_MMouse_S}
};

void initCMDevice(DEVICE_INDEX device = DEV_DEFAULT)
{
	// TODO: Change to a non hardcoded value
	SetControlDevice(device); // DEV_MKeys_M_White);
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
	float multiplier;
} MYDATA, *PMYDATA;

DWORD WINAPI myMusicThread(LPVOID lpParameter)
{
	PMYDATA pData;

	pData = (PMYDATA)lpParameter;
	//unsigned int& myCounter = *((unsigned int*)lpParameter);
	unsigned int myCounter = pData->counter;
	float threshold = pData->threshold;
	float multiplier = pData->multiplier;
	float fVolValue = GetNowVolumePeekValue();
	BYTE bThreshold = BYTE(threshold * 255);
	//initCMDevice();
	while (myCounter < 0xFFFFFFFF)
		//while(true)
	{
		//++myCounter;
		float fPeekValue = GetNowVolumePeekValue();
		fPeekValue *= multiplier;
		if (fPeekValue > 1.0f)
		{
			fPeekValue = 1.0f;
		}
		//if (fVolValue - fPeekValue > 0.1f || fPeekValue - fVolValue > 0.1f)
		if (fPeekValue >= threshold) //0.30f)
		{
			//fVolValue = fPeekValue + 0.0f;
			BYTE bVol = BYTE(fPeekValue * 255);
			SetFullLedColor(bVol, bVol, bVol);
			//Sleep(450);
		}
		else
		{
			// Check if Default (Idle) Brightness is less than the threshold
			if (DEF_BRIGHTNESS < bThreshold)
			{
				// Use it only if it is less than the threhold
				SetFullLedColor(DEF_BRIGHTNESS, DEF_BRIGHTNESS, DEF_BRIGHTNESS);
			}
			else
			{
				// Otherwise, use the half of the threshold (divided by 2) as the idle Brightness
				SetFullLedColor(bThreshold / 2, bThreshold / 2, bThreshold / 2);
			}
			
			//Sleep(450);
		}
		Sleep(100);
	}
	deinitCMDevice();
	return 0;
}

void setupMusicThreads(float threshold, float multiplier = 1.0f, DEVICE_INDEX device = DEV_DEFAULT)
{
	using namespace std;
	
	initCMDevice(device);

	unsigned int myCounter = 0;
	PMYDATA pData;
	DWORD myThreadID;

	pData = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
	pData->counter = myCounter;
	pData->threshold = threshold + 0.0f;
	pData->multiplier = multiplier + 0.0f;

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

int main(int argc, char *argv[], char *envp[])
{
	std::cout << "Hello World!\n"; // TODO: Change to something more useful

	InputParser input(argc, argv);

	float fThreshold = DEF_THRESHOLD, //0.30f,
		fMultiplier = DEF_MULTIPLIER; // 1.0f;

	if (input.cmdOptionExists("threshold"))
	{
		fThreshold = std::strtof((input.getCmdOption("threshold")).c_str(), 0);
	}
	if (fThreshold < MIN_THRESHOLD || fThreshold > MAX_THRESHOLD)
	{
		fThreshold = DEF_THRESHOLD;
	}
	std::cout << (input.getCmdOption("multiplier")).c_str() << "\n";
	if (input.cmdOptionExists("multiplier"))
	{
		fMultiplier = std::strtof((input.getCmdOption("multiplier")).c_str(), 0);
	}
	if (fMultiplier < MIN_MULTIPLIER || fMultiplier > MAX_MULTIPLIER)
	{
		// Value is out of range. Only allow values between 0.01 and 10.0
		fMultiplier = DEF_MULTIPLIER; // Reset value to default
	}
	
	DEVICE_INDEX device = DEV_DEFAULT;
	if (input.cmdOptionExists("device"))
	{
		std::string deviceCmd = input.getCmdOption("device");

		// String to enum lookup.
		// Adapted from example code on StackOverflow:
		// https://stackoverflow.com/a/7163130 
		auto it = deviceTable.find(deviceCmd);
		if (it != deviceTable.end())
		{
			device = it->second;
		}
		else
		{
			device = DEV_DEFAULT;
		}
	}
	setupMusicThreads(fThreshold, fMultiplier, device);
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
