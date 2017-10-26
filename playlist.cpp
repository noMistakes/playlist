#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;
class Playlist
{
public:
	Playlist::Playlist()
	{
		name.assign("");
		songs.assign("");
	}
	Playlist::Playlist(string _name)
	{
		songs = "";
		name.assign(_name + ".m3u");
		fstream file(name);
		file << "#EXTM3U \n" << endl;
		file.close();
	}
	Playlist::Playlist(string _directory, string _name)
	{
		name.assign(_name + ".m3u");
		fstream file;
		file.open(name, fstream::out);
		file << "#EXTM3U \n" << endl;

		WIN32_FIND_DATA ffd;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		_directory += "\\*";
		hFind = FindFirstFile(_directory.c_str(), &ffd);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			cout << "We couldn't find such directory" << endl;
		}
		else
		{
			char buffer[260];
			do
			{
				string FileName;
				strcpy_s(buffer, ffd.cFileName);
				FileName.assign(buffer);
				if (FileName.find(".mp3") != string::npos)
				{
					string help = _directory;
					file << "#EXTINF: ";
					file.close();
					char artist[30];
					help.pop_back();
					help.pop_back();
					fstream test(help += FileName);
					test.seekg(-95, test.end);
					test.read(artist, 30);
					test.close();
					file.open(name, fstream::app);
					(songs += help) += ";";
					file << "artist " << artist << endl << help << endl << endl;
				}

			} while (FindNextFile(hFind, &ffd) != 0);
		}

		file.close();

	}
	Playlist& Playlist:: operator-=(string _file)
	{

		if (songs.find(_file) != string::npos)
		{
			songs.erase(songs.find(_file), _file.length() + 1);
		}
		songsToFile(name);

		return *this;
	}
	Playlist& Playlist:: operator -(string _directory)
	{
		WIN32_FIND_DATA ffd;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		_directory += "\\*";
		hFind = FindFirstFile(_directory.c_str(), &ffd);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			cout << "We couldn't find such directory" << endl;
		}
		else
		{
			char buffer[260];
			do
			{
				string FileName;
				strcpy_s(buffer, ffd.cFileName);
				FileName.assign(buffer);
				if (FileName.find(".mp3") != string::npos)
				{
					string directoryBuffer = _directory;
					directoryBuffer.pop_back();
					if (songs.find(directoryBuffer += FileName) != string::npos)
					{
						songs.erase(songs.find(directoryBuffer), directoryBuffer.length() + 1);
					}
				}

			} while (FindNextFile(hFind, &ffd) != 0);
		}


		songsToFile(name);

		return *this;

	}
	Playlist& Playlist:: operator-=(const Playlist& ob)
	{
		int position = 0;
		do
		{
			string song = ob.songs.substr(position, songs.find(";", position) - position);

			if (songs.find(song) != string::npos)
			{
				songs.erase(songs.find(song), song.length() + 1);
			}

			position = ob.songs.find(";", position) + 1;
		} while (ob.songs.find(";", position) != string::npos);

		songsToFile(name);

		return *this;
	}
	Playlist& Playlist:: operator+=(string _file)
	{

		if (songs.find(_file) == string::npos)
		{
			(songs += _file) += ";";
		}
		songsToFile(name);

		return *this;
	}
	Playlist& Playlist:: operator+(string _directory)
	{
		WIN32_FIND_DATA ffd;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		_directory += "\\*";
		hFind = FindFirstFile(_directory.c_str(), &ffd);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			cout << "We couldn't find such directory" << endl;
		}
		else
		{
			char buffer[260];
			do
			{
				string FileName;
				strcpy_s(buffer, ffd.cFileName);
				FileName.assign(buffer);
				if (FileName.find(".mp3") != string::npos)
				{
					string directoryBuffer = _directory;
					directoryBuffer.pop_back();
					directoryBuffer.pop_back();
					if (songs.find(directoryBuffer += FileName) == string::npos)
					{
						(songs += directoryBuffer) += ";";
					}
				}

			} while (FindNextFile(hFind, &ffd) != 0);
		}


		songsToFile(name);

		return *this;
	}
	Playlist& Playlist:: operator+=(const Playlist& ob)
	{
		int position = 0;
		do
		{
			string song = ob.songs.substr(position, songs.find(";", position) - position);

			if (songs.find(song) == string::npos)
			{
				(songs += song) += ";";
			}

			position = ob.songs.find(";", position) + 1;
		} while (ob.songs.find(";", position) != string::npos);

		songsToFile(name);

		return *this;
	}
	void Playlist::songsToFile(string _name)
	{
		fstream file;
		file.open(_name, fstream::out);
		file << "#EXTM3U \n" << endl;

		int position = 0;
		do
		{
			string path_to_song = songs.substr(position, songs.find(";", position) - position);
			if (path_to_song.find(".mp3") != string::npos)
			{
				file << "#EXTINF: ";
				file.close();
				char artist[30];
				fstream test(path_to_song);
				test.seekg(-95, test.end);
				test.read(artist, 30);
				test.close();
				file.open(name, fstream::app);
				file << "artist " << artist << endl << path_to_song << endl << endl;
			}

			position = songs.find(";", position) + 1;
		} while (songs.find(";", position) != string::npos);

		file.close();
	}
	void Playlist::get_playlist()
	{
		int position = 0;
		do
		{
			cout << songs.substr(position, songs.find(";", position) - position) << endl;
			position = songs.find(";", position) + 1;
		} while (songs.find(";", position) != string::npos);
	}
	Playlist:: ~Playlist()
	{
		_fcloseall();
	}
private:
	ofstream file;
	string name;
	string songs;
};

