#include <iostream>
#include <Windows.h>
#include <string>
#include <wchar.h>
#include <filesystem>

char driveBuffer[100];
std::string systemDrive[30];
std::vector<std::string> dirSkipList{"Windows", "Program Files", "Program Files (x86)", "Program Data", "Temp", "AppData", "SteamLibrary"};

void getDrive() {
	if (strlen(driveBuffer) != 0) {
		for (int i = 0, j = 0, k = 0; i < 100; i++) {
			if (driveBuffer[i] != '\0') {
				systemDrive[k] += driveBuffer[i];
				j++;
			}
			if (j == 3){
				j = 0;
				k++;
			}
		}
	}
	else {
		DWORD driveStat = GetLogicalDriveStringsW(sizeof(driveBuffer), (LPWSTR)driveBuffer);
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

void searchInDir(std::filesystem::path &path) {
	try {
		if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
			std::error_code ec;
			std::filesystem::recursive_directory_iterator dir(path, std::filesystem::directory_options::skip_permission_denied, ec), end;

			while (dir != end) {
				if (std::filesystem::is_directory(dir->path()) && (std::find(dirSkipList.begin(), dirSkipList.end(), dir->path().filename()) != dirSkipList.end())) {
					dir.disable_recursion_pending();
				}
				else if (dir->path().has_extension() && (dir->path().extension() == ".jpg" || dir->path().extension() == ".png")) {
					//targetFile.push_back(dir->path());
					std::cout << dir->path().string() << std::endl;
				}

				dir++;
				if (ec) {
					std::cerr << "Error While Accessing : " << dir->path().string() << " :: " << ec.message() << '\n';
				}
			}
		}
	}
	catch (std::system_error& e){
		std::cerr << "Exception :: " << e.what();
	}
}

int main() {

}