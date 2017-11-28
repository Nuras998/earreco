#include "tools.h"

using namespace std;


void getFilesInDirectory(const string& dirName, vector<string>& fileNames) {
	//size_t extensionLocation;
	struct dirent **namelist;
	int n = 0;
	n = scandir(dirName.c_str(), &namelist, 0, alphasort);

	if (n > 0) {
		for (int i=2; i<n; i++) {
  			fileNames.push_back((string) dirName + "/" + namelist[i]->d_name);
		}
	}
}

