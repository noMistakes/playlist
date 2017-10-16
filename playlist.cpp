#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __unix__
#include <dirent.h>
#else
#include <direct.h>
#include <windows.h>
#define MAXPATHLEN 4096
#endif
#include <errno.h>
#include <vector>
#include <cstring>
#include <sys/param.h>
#include <unistd.h>
#include <fstream>
#include "MP3tag.h"

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
void cd(string path) {
	int rc = chdir(path.c_str());
	if (rc < 0) {
		cout << "error" << endl;
	}
	/*DIR *dir = NULL;
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
	 while (entryPtr != NULL) {
	 ;

	 if ((strncmp(entry.d_name, ".", PATH_MAX) == 0)
	 || (strncmp(entry.d_name, "..", PATH_MAX) == 0)) {
	 retval = readdir_r(dir, &entry, &entryPtr);
	 continue;
	 }
	 }*/
}
void pwd() {
	char buffer[MAXPATHLEN];
#ifdef __unix__
	char *path = getcwd(buffer, MAXPATHLEN);
#else
	char *path = _getcwd(buffer, MAXPATHLEN);
#endif
	if (!path) {
		cout << "error" << endl;
	} else {
		string CurrentPath;
		CurrentPath = path;
		cout << CurrentPath << endl;
	}
}
int readmp3(const char *path) {
	ofstream fout("info.txt");
	int fileNameLength = 1024;
	int mp3TagSize = 128;
	TAGdata tagStruct;
	//char *fileName=new char[fileNameLength + 1];
	char* fileName = new char[fileNameLength + 1];
	fileName = (char*) path;
	ifstream mp3File;
	char buffer[mp3TagSize + 1];
	//strcpy(path, fileName.c_str());
	//cout<<fileName;

	//cin.getline(fileName, fileNameLength);
	mp3File.open(path);
	buffer[mp3TagSize] = '\0';  //just a precaution

	if (mp3File) {
		cout << "Successful open of " << fileName << endl;
	} else {
		cerr << " Could not open " << fileName << endl;
		return 1;
	}

	mp3File.seekg(-(mp3TagSize), ios::end);
	if (mp3File.fail()) {
		cerr << "Could not seek " << endl;
		return 1;
	}

	mp3File.read(reinterpret_cast<char *>(&tagStruct), mp3TagSize);
	if (mp3File.fail()) {
		cerr << "Could not read after seeking" << endl;
		return 1;
	}
	fout << "tag:" << tagStruct.tag << endl;
	fout << "title:" << tagStruct.title << endl;
	fout << "artist:" << tagStruct.artist << endl;
	fout << "album:" << tagStruct.album << endl;
	fout << "year:" << tagStruct.year << endl;
	fout << "comment:" << tagStruct.comment << endl;
	fout << "genre:" << tagStruct.genre << endl;
	fout << endl;
	mp3File.close();
}
int main() {
	string command;
	while (command != "exit") {
		getline(cin, command);
		if (command.find("ls") == 0) {
			ls();
		}
		if (command == "pwd") {
			pwd();
		} else if (command.find("cd") == 0) {
			cd(command.substr(command.find(" ") + 1));

		}
		if (command.find("info") == 0) {
			//	const char *d=();
			//	cout<<command.substr(command.find(" ") + 1).c_str();
			readmp3(command.substr(command.find(" ") + 1).c_str());
		}

	}
	return 0;
}
