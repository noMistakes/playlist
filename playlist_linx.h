/*
 * playlist.h
 *
 *  Created on: Oct 22, 2017
 *      Author: vitalik
 */
#include <iostream>
#include <cstring>
#include <fstream>
#include "MP3tag.h"
using namespace std;
#ifndef PLAYLIST_H_
#define PLAYLIST_H_




class Playlist {
private:
	string name;
	int count;
	TAGdata* songs_arr=new TAGdata[999];

public:
	Playlist();
	Playlist(string playlist_name);
	void CreatePlaylist();
	int AddSong(string song_name);
	void DeleteSong();
	void PrintPlaylist();
	void PrintSongByArtist();
	void OutputToFile();
	string getname() const;
	~Playlist();

};


#endif /* PLAYLIST_H_ */
