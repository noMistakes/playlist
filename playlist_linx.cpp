#include <iostream>
#include <dirent.h>
#include <sys/param.h>
#include <errno.h>
#include <vector>
#include <cstring>
#define MAX_PATH 260
#include <unistd.h>
#include <fstream>
#include "playlist_linx.h"
#include "MP3tag.h"
#include <stdlib.h>
using namespace std;
int readmp3(const char *path);
void ls();
void cd(string path);
string pwd();
vector<string> Find_file_mp3();
int Filter_mp3(const struct dirent * d);
Playlist::Playlist() :
		name(" ") {
}
string Playlist::getname() const {
	return this->name;
}
Playlist::Playlist(string name) {
	this->name = name.append(".m3u");

}
void Playlist::CreatePlaylist() {
	fstream file(this->name.c_str(), ios::app);
	vector<string> mp3list = Find_file_mp3();
	for (std::vector<string>::iterator it = mp3list.begin();
			it != mp3list.end(); ++it) {
		//cout<<"adding song "<<*it<<endl;
	}
	file << "#EXTM3U";
	//file.close();
}

void Playlist::DeleteSong(string song_name) {
	string line;
	fstream file(getname().c_str());

	int line_iter = 0;
	vector<string> strings;
	while (getline(file, line)) {
		if (line.find(song_name)== string::npos) {
			strings.push_back(line);
		} else {
			strings.pop_back();
		}
		line_iter++;
	}
	file.close();
	file.open(getname().c_str(), std::fstream::out | std::fstream::trunc);
	for(int i=0;i<strings.size();i++){
		file<<strings[i].c_str();
	}
	file.clear();
}
Playlist::~Playlist() {

}
void Playlist::DeletePlaylist(string playlist_name) {
	remove(playlist_name.c_str());
}
int Playlist::AddSong(string song_name) {
	vector<string> mp3list = Find_file_mp3();
	fstream file(getname().c_str(), ios::app);
	TAGdata tagStruct;
	bool ismp3 = false;
	/*for (int i = 0; i < mp3list.size(); i++) {
	 cout << mp3list[i] << endl;
	 if (mp3list[i] == song_name) {
	 ismp3 = true;
	 break;
	 }
	 }
	 if (ismp3 == true) {
	 */
	int mp3TagSize = 128;
	string path = pwd();
	path = path + "/" + song_name;
	const char* c = path.c_str();
	ifstream mp3File;
	char buffer[mp3TagSize + 1];
	mp3File.open(song_name.c_str());
	buffer[mp3TagSize] = '\0';
	if (mp3File) {
		cout << "Successful open of " << name.c_str() << endl;
	} else {
		cerr << " Could not open " << c << endl;
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
	file << "\n#EXTINF: ";

	file << tagStruct.title << " - ";
	file << tagStruct.artist << " - ";
	file << tagStruct.year << "\n";
	file << c;
	mp3File.close();
	file.close();
}
int Filter_mp3(const struct dirent * d) {
	string filename(d->d_name);
	if (filename.find(".mp3") == string::npos) {
		//	cout << "not  mp3file " << filename << endl;
		return 0; //not found
	} else {
		//cout << "is mp3file " << filename << endl;
		return 1;
	}

}
vector<string> Find_file_mp3() {
	struct dirent **namelist;
	vector<string> mp3list;
	int n;
	string path = pwd();
	const char* c = path.c_str();
	n = scandir(c, &namelist, Filter_mp3, alphasort);
	if (n < 0)
		perror("scandir :");
	else {

		while (n--) {
			//printf("%s\n", namelist[n]->d_name);
			string s(namelist[n]->d_name);
			mp3list.push_back(s);
			//	cout << "found mp3: " << s << endl;
			free(namelist[n]);
		}

		free(namelist);
	}
	return mp3list;
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

}
void cd(string path) {
	int rc = chdir(path.c_str());
	if (rc < 0) {
		cout << "error" << endl;
	}

}
string pwd() {
	char buffer[MAXPATHLEN];
	char *path = getcwd(buffer, MAXPATHLEN);
	if (!path) {
		cout << "error" << endl;
		return "";
	} else {
		string s(path);
		//cout << "pwd: " << s << endl;
		return s;
	}
}
int readmp3(const char *path) {
	ofstream fout("info.txt");
	int fileNameLength = 1024;
	int mp3TagSize = 128;
	TAGdata tagStruct;
	char* fileName = new char[fileNameLength + 1];
	fileName = (char*) path;
	ifstream mp3File;
	char buffer[mp3TagSize + 1];
	mp3File.open(path);
	buffer[mp3TagSize] = '\0';
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
	string playlist_name;
	while (command != "exit") {
		getline(cin, command);

		if (command.find("ls") == 0) {
			ls();
		}
		if (command == "pwd") {
			cout << pwd();
		} else if (command.find("cd") == 0) {
			cd(command.substr(command.find(" ") + 1));

		}
		if (command.find("info") == 0) {
			readmp3(command.substr(command.find(" ") + 1).c_str());
		}
		if (command.find("playlist_create") == 0) {
			playlist_name = command.substr(command.find(" ") + 1);
			Playlist obj1(playlist_name);
			obj1.CreatePlaylist();
		}
		if (command.find("addsong") == 0) {
			command = command.substr(command.find(" ") + 1);
			Playlist obj1(playlist_name);
			obj1.AddSong(command);
		}
		if (command.find("deletesong") == 0) {
			command = command.substr(command.find(" ") + 1);
			Playlist obj1(playlist_name);
			obj1.DeleteSong(command);
		}
		if (command.find("playlist_delete") == 0) {
			command = command.substr(command.find(" ") + 1);
			Playlist obj1(playlist_name);
			obj1.DeletePlaylist(command);
		}
	}
	return 0;
}
