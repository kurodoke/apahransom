#pragma once

#include <iostream>
#include <fstream>
#include <string>

void getDrive();
void isCSysDir();
void searchInDir(std::filesystem::path& path, bool isEncrypt = true);