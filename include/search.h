#pragma once


#include <iostream>
#include <fstream>
#include <string>

void getDrive(std::string(*systemDrive)[30]);
void isCSysDir(std::string(*systemDrive)[30]);
void searchInDir(std::filesystem::path& path, bool isEncrypt = true);