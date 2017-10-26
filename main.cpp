#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _unix_
#include <dirent.h>
#include <sys/param.h>
#else
#include <direct.h>
#include <windows.h>
#define MAXPATHLEN 4096
#endif
#include <errno.h>
#include <vector>
#include <cstring>
#include <fstream>
#include "MP3tag.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <strsafe.h>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <fstream>
#include <vfw.h>
#include "Playlist.h"

#ifdef _unix_
using namespace std;
void open_dir(char* dir_name, std::vector<std::string>& file_names) {
	if (dir_name == NULL) {
		return;
	}

	string dir_name_ = string(dir_name);
	vector<string> files_;
	int getdir(string dir, vector <string> &files) {

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
#else
//wstring cd(string command, string &path)
//{
//	if (!path.empty() && command.find("C:") && command.find("D:"))
//	{
//		path.pop_back();
//		cout << path << endl;
//		if (command.find("..") == 0)
//		{
//			path.pop_back();
//			path = path.substr(0, path.rfind("\\"));
//		}
//		else
//		{
//			path += command;
//		}
//	}
//	else
//	{
//		path = command;
//	}
//	path += "\\";
//
//	cout << path << endl;
//	path.push_back('*');
//	return wstring(path.begin(), path.end());
//}
wstring cd(string directory,string operation)
{
	string buffer = directory;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	buffer += operation.substr(3, operation.length() - 4);
	buffer += "\\*";
	hFind = FindFirstFile(buffer.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		cout << "We couldn't find such directory" << endl;

	}

	if (operation.find("..") == 3)
	{
		if (directory.length() <= 4)
		{
			cout << "Operations can't be performed" << endl;
		}
		else
		{
			directory.erase((directory.substr(0, directory.length() - 2)).rfind("\\") + 1);
		}
	}
	return 0;
}
//	HANDLE hFind = INVALID_HANDLE_VALUE;
//	WIN32_FIND_DATA fdata;
//
//	if (dir_name_[dir_name_.size() - 1] == '\\' || dir_name_[dir_name_.size() - 1] == '/') {
//		dir_name_ = dir_name_.substr(0, dir_name_.size() - 1);
//	}
//
//	hFind = FindFirstFile(string(dir_name_).append("\\*").c_str(), &fdata);
//	if (hFind != INVALID_HANDLE_VALUE)
//	{
//		do
//		{
//			if (strcmp(fdata.cFileName, ".") != 0 &&
//				strcmp(fdata.cFileName, "..") != 0)
//			{
//				if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//				{
//					continue; // a diretory
//				}
//				else
//				{
//					files_.push_back(fdata.cFileName);
//				}
//			}
//		} while (FindNextFile(hFind, &fdata) != 0);
//	}
//	else {
//		cerr << "can't open directory\n";
//		return;
//	}
//
//	if (GetLastError() != ERROR_NO_MORE_FILES)
//	{
//		FindClose(hFind);
//		cerr << "some other error with opening directory: " << GetLastError() << endl;
//		return;
//	}
//
//	FindClose(hFind);
//	hFind = INVALID_HANDLE_VALUE;
#endif 
	

#ifdef _unix_
void ls() {

	DIR *dir = opendir(".");	if (dir) {
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
#else
void ls(string directory)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	directory += "\\*";
	hFind = FindFirstFile(directory.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		cout << "We couldn't find such directory" << endl;
	}
	else
	{
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				cout << ffd.cFileName << TEXT(" <DIR>\n");
			}
			else
			{
				cout << ffd.cFileName << TEXT(" \n");
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}
}
#endif
//void cd(string path) {
//	int rc = _chdir(path.c_str());
//	if (rc < 0) {
//		cout << "error" << endl;
//	}
//	DIR *dir = NULL;
//	 struct dirent entry;
//	 struct dirent *entryPtr = NULL;
//	 int retval = 0;
//	 unsigned count = 0;
//	 char pathName[PATH_MAX + 1];
//
//	 dir = opendir(theDir);
//	 if (dir == NULL) {
//	 cout << ("Error opening %s: %s", theDir);
//	 }
//	 retval = readdir_r(dir, &entry, &entryPtr);
//	 while (entryPtr != NULL) {
//	 ;
//
//	 if ((strncmp(entry.d_name, ".", PATH_MAX) == 0)
//	 || (strncmp(entry.d_name, "..", PATH_MAX) == 0)) {
//	 retval = readdir_r(dir, &entry, &entryPtr);
//	 continue;
//	 }
//	 }
//}
void pwd() {
	char buffer[MAXPATHLEN];
#ifdef _unix_
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
	const int mp3TagSize = 128;
	TAGdata tagStruct;
	//char *fileName=new char[fileNameLength + 1];
	char* fileName = new char[fileNameLength + 1];
	fileName = (char*) path;
	ifstream mp3File;
	char buffer[mp3TagSize + 1];
	//strcpy_s_s_s_s_s_s_s(path, fileName.c_str());
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
	string dir = "C://";
	string command;
	while (command != "exit") {
		getline(cin, command);
		if (command.find("ls") == 0) {
#ifdef _unix_
			ls();
#else 
			ls(dir);
#endif 
		}
		else if (command == "pwd") {
			pwd();
		} 
		else if (command.find("cd") == 0) {
			cd(dir,command.substr(command.find(" ") + 1));

		}
		if (command.find("info") == 0) {
			//	const char *d=();
			//	cout<<command.substr(command.find(" ") + 1).c_str();
			readmp3(command.substr(command.find(" ") + 1).c_str());
		}

	}
	return 0;
}
