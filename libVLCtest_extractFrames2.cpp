//To compile:
//cc vlcsms.c -o vlcsms -lvlc
//This source is by Tim Sheerman-Chase and it is released as public domain.

#include "stdafx.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <inttypes.h>
#include <Windows.h>
#include <vlc/vlc.h>


int done = 0;
libvlc_media_player_t *media_player = NULL;
uint8_t *videoBuffer = 0;
uint8_t *audioBuffer = 0;
unsigned int videoBufferSize = 0;
unsigned int audioBufferSize = 0;

// Audio prerender callback
void cbAudioPrerender(void* p_audio_data, uint8_t** pp_pcm_buffer, unsigned int size)
{
	// TODO: Lock the mutex
	//printf("cbAudioPrerender %i\n",size);
	//printf("atest: %lld\n",(long long int)p_audio_data);
	if (size > audioBufferSize || !audioBuffer)
	{
		printf("Reallocate raw audio buffer\n");
		if (audioBuffer) free(audioBuffer);
		audioBuffer = (uint8_t *)malloc(size);
		audioBufferSize = size;
	}
	*pp_pcm_buffer = audioBuffer;
}

// Audio postrender callback
void cbAudioPostrender(void* p_audio_data, uint8_t* p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, unsigned int size, int64_t pts)
{
	//printf("cbAudioPostrender %i\n", size);
	// TODO: explain how data should be handled
	// TODO: Unlock the mutex
}

void cbVideoPrerender(void *p_video_data, uint8_t **pp_pixel_buffer, unsigned int size) {

	// Locking
	//printf("cbVideoPrerender %i\n",size);
	//printf("vtest: %lld\n",(long long int)p_video_data);
	if (size > videoBufferSize || !videoBuffer)
	{
		printf("Reallocate raw video buffer\n");
		if (audioBuffer) free(videoBuffer);
		videoBuffer = (uint8_t *)malloc(size);
		videoBufferSize = size;
	}
	*pp_pixel_buffer = videoBuffer;
}

void cbVideoPostrender(void *p_video_data, uint8_t *p_pixel_buffer
	, int width, int height, int pixel_pitch, int size, int64_t pts) {
	//printf("cbVideoPostrender %i\n",size);
	// Unlocking

}

static void handleEvent(const libvlc_event_t* pEvt, void* pUserData)
{
	libvlc_time_t time;
	switch (pEvt->type)
	{
	case libvlc_MediaPlayerTimeChanged:
		time = libvlc_media_player_get_time(media_player);
		printf("MediaPlayerTimeChanged %lld ms\n", (long long)time);
		break;
	case libvlc_MediaPlayerEndReached:
		printf("MediaPlayerEndReached\n");
		done = 1;
		break;
	default:
		printf("%s\n", libvlc_event_type_name(pEvt->type));
	}
}

int main(int argc, char **argv)
{
	// VLC pointers
	libvlc_instance_t *vlcInstance;
	void *pUserData = 0;

	// VLC options
	char smem_options[1000];
	sprintf_s(smem_options, 
		1000,
		"#transcode{vcodec=h264,acodec=mpga}:smem{"
		"video-prerender-callback=%lld,"
		"video-postrender-callback=%lld,"
		"audio-prerender-callback=%lld,"
		"audio-postrender-callback=%lld,"
		"audio-data=%lld,"
		"video-data=%lld},"
		, (long long int)(intptr_t)(void*)&cbVideoPrerender
		, (long long int)(intptr_t)(void*)&cbVideoPostrender
		, (long long int)(intptr_t)(void*)&cbAudioPrerender
		, (long long int)(intptr_t)(void*)&cbAudioPostrender
		, (long long int)100 //This would normally be useful data, 100 is just test data
		, (long long int)200); //Test data

	const char * const vlc_args[] = {
		"-I", "dummy", // Don't use any interface
		"--ignore-config", // Don't use VLC's config
		"--extraintf=logger", // Log anything
		"--verbose=1", // Be verbose
		"--sout", smem_options // Stream to memory
	};

	// We launch VLC
	vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

	media_player = libvlc_media_player_new(vlcInstance);
	libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(media_player);
	libvlc_event_attach(eventManager, libvlc_MediaPlayerTimeChanged, handleEvent, pUserData);
	libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, handleEvent, pUserData);
	libvlc_event_attach(eventManager, libvlc_MediaPlayerPositionChanged, handleEvent, pUserData);

	//libvlc_media_t *media = libvlc_media_new_path(vlcInstance, "test.wav");
	char *fname = "rtsp://root:e9K4Hf0EBp1t@147.87.113.151/axis-media/media.amp?videocodec=h264&resolution=1280x800&duration=10&clock=1";
	libvlc_media_t* media = libvlc_media_new_location(vlcInstance, fname);
	libvlc_media_player_set_media(media_player, media);
	libvlc_media_player_play(media_player);

	while (!done)
	{
		Sleep(1);
		//libvlc_media_player_set_position(media_player, 0.);
	}
	libvlc_media_release(media);
	return 0;
}