/*
 * audio_and_song.h
 *
 *  Created on: Feb 3, 2021
 *      Author: joa-m
 */

#ifndef AUDIO_AND_SONG_H_
#define AUDIO_AND_SONG_H_

typedef enum
{
	AUDIO_PLAYING,
	AUDIO_READY_TO_SEND,
    AUDIO_STOP,
	AUDIO_ERROR
} audio_state_t;

typedef enum
{
	SONG_NO_SOUND,
	SONG_WELCOME,
	SONG_ECG_OUT_OF_RANGE,
	SONG_SPO2_OUT_OF_RANGE,
	SONG_TEMP_OUT_OF_RANGE
} song_state_t;

typedef struct {
	audio_state_t audio_state;
	song_state_t song_state;
}playing_state_t;

#endif /* AUDIO_AND_SONG_H_ */
