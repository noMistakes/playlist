#ifdef _unix_
#include <dirent.h>
#include <sys/param.h>
#endif
#include <errno.h>
#include <vector>
#include <cstring>
#include <fstream>
#include "MP3tag.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <strsafe.h>
#include <fstream>
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

void ls() {

	DIR *dir = opendir(".");	if (dir) {
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL) {
			cout << (ent->d_name) << endl;
		}
	} else {
		cout << "Error opening dir" << endl;
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
void ls(string dir)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	dir += "\\*";
	hFind = FindFirstFile(dir.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		cout << "We couldn't find such dir" << endl;
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

void main()
{

	string path = "C:\\";

	//B.get_playlist();

	string command = "";
	while (command != "exit")
	{
		cout << path << ' ';
		getline(cin, command);
		if (command.find("ls") == 0)
		{
			ls(path);
		}
		else if (command.find("cd") == 0)
		{
			if (command.find(":\\") == 4 || command.find("\\") == command.length() - 2)
			{
				string buffer = "";
				if (command.find(":\\") != 4)
				{
					buffer = path;
				}
				WIN32_FIND_DATA ffd;
				HANDLE hFind = INVALID_HANDLE_VALUE;
				buffer += command.substr(3, command.length() - 4);
				buffer += "\\*";
				hFind = FindFirstFile(buffer.c_str(), &ffd);
				if (INVALID_HANDLE_VALUE == hFind)
				{
					cout << "We couldn't find such path" << endl;
					continue;
				}
			}
			if (command.find("..") == 3)
			{
				if (path.length() <= 4)
				{
					cout << "commands can't be performed" << endl;
				}
				else
				{
					path.erase((path.substr(0, path.length() - 2)).rfind("\\") + 1);
				}
			}
			else if (command.find(":\\") == 4)
			{
				path = command.substr(3, command.length() - 4);
			}
			else if (command.find("\\") == command.length() - 2)
			{
				path += command.substr(3, command.length() - 4);
			}
			else
			{
				cout << "Syntax error!" << endl;
			}
		}
		else if (command.find("create") == 0)
		{
			if (command.find("create_null") == 0)
			{
				Playlist P1(command.substr(12));
			}
			else
			{
				Playlist P1(path, command.substr(7));
				P1.get_playlist();
			}
		}
	
		else
		{
			cout << "Error!" << endl;
		}
	} 


	system("pause");
}
