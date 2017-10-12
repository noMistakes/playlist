#include <iostream>
#include <stdio.h>
//#include " /usr/include/x86_64-linux-gnu/sys/stat.h"
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>
#include <sys/param.h>
#include <unistd.h>


using namespace std;
int getdir(string dir, vector<string> &files) {
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
void ls() {
	DIR *dir = opendir(".");
	if (dir) {
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL) {
			cout << (ent->d_name) << endl;
		}
	} else {
		cout << "Error opening directory" << endl;
	}
	/*
	 string dir1 = string(".");
	 vector<string> files = vector<string>();

	 getdir(dir1, files);

	 for (unsigned int i = 0; i < files.size(); i++) {
	 cout << files[i] << endl;
	 }*/
}
void cd(char *theDir) {
	DIR *dir = NULL;
	struct dirent entry;
	struct dirent *entryPtr = NULL;
	int retval = 0;
	unsigned count = 0;
	char pathName[PATH_MAX + 1];

	dir = opendir(theDir);
	if (dir == NULL) {
		cout << ("Error opening %s: %s", theDir);
	}
	retval = readdir_r(dir, &entry, &entryPtr);
	while (entryPtr != NULL) {;

		if ((strncmp(entry.d_name, ".", PATH_MAX) == 0)
				|| (strncmp(entry.d_name, "..", PATH_MAX) == 0)) {
			retval = readdir_r(dir, &entry, &entryPtr);
			continue;
		}
	}
}

int main() {
	string command, path;
	while (command != "exit") {
		getline(cin, command);
		if (command.find("ls") == 0) {
			ls();
		}
		if (command == "curr") {
			char buffer[MAXPATHLEN];
			char *path = getcwd(buffer, MAXPATHLEN);
			if (!path) {
			cout<<"error"<<endl;
			} else {
				string CurrentPath;
				CurrentPath = path;
				cout << CurrentPath << endl;
			}
		} else if (command == "cd") {
			string newDir;
			cin >> newDir;
			int rc = chdir(newDir.c_str());
			if (rc < 0) {
				cout<<"error"<<endl;
			}
		}

	}
	return 0;
}
