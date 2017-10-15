/*
 * MP3tag.h
 *
 *  Created on: Oct 13, 2017
 *      Author: vitalik
 */

#ifndef MP3TAG_H_
#define MP3TAG_H_
struct TAGdata {
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	int genre;
};

#endif /* MP3TAG_H_ */
