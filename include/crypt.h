#pragma once

#include <iostream>
#include <fstream>
#include <string>

bool fileExist(const std::string& filename);
void encrypt(std::filesystem::path pathname, bool isEncrypt = true);
void decrypt(std::filesystem::path pathname, bool isDecrypt = true);